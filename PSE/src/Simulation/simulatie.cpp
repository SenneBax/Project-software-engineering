#include "../Simulation/simulatie.h"
#include <algorithm>
#include <cmath>
#include <random>
#include "DesignByContract.h"

/**
 * @brief Constructor voor de simulatieklasse
 * @param situatie Referentie naar de te simuleren verkeerssituatie
 * @param tijdstap De tijdstap voor de simulatie in seconden
 * @pre situatie.properlyInitialized() == true
 * @pre tijdstap > 0.0 of tijdstap wordt automatisch ingesteld op 0.0166
 * @post properlyInitialized() == true
 * @post getHuidigeSimulatieTijd() == 0.0
 * @post getTijdstap() == tijdstap (of 0.0166 als tijdstap <= 0)
 * ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie),
      tijdstap(tijdstap > 0.0 ? tijdstap : 0.0166),
      huidigeSimulatieTijd(0.0),
      vertraagAfstand(250.0),
      stopAfstand(50.0),
      vertraagFactor(0.4),
      aantalVoertuigen(0),
      gemiddeldeSnelheid(0.0),
      verwijderdeVoertuigenTeller(0),
      totaalVerwijderdeVoertuigen(0),
      autoGenereerVoertuigen(false),
      totaleTijd(0.0)
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Controleer of het simulatie object correct is geïnitialiseerd
 * @return true als het object correct is geïnitialiseerd, false anders
 */
bool simulatie::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Voer één simulatiestap uit
 * @pre properlyInitialized() == true
 * @post huidigeSimulatieTijd is verhoogd met tijdstap
 * @post alle verkeerselementen zijn bijgewerkt volgens hun regels
 * @post statistieken zijn bijgewerkt
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij stap");
 * ENSURE(huidigeSimulatieTijd > 0.0, "huidigeSimulatieTijd moet toenemen na een stap");
 */
void simulatie::stap() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij stap");

    // Update verkeerslichten
    verwerkVerkeerslichten();

    // Verwerk voertuigen
    verwerkVoertuigen();

    // Verwerk bushaltes
    verwerkBushaltes();

    // Verwerk kruispunten
    verwerkKruispunten();

    // Genereer nieuwe voertuigen indien nodig
    if (autoGenereerVoertuigen) {
        genereerNieuweVoertuigen();
    }

    // Verhoog tijd
    huidigeSimulatieTijd += tijdstap;
    totaleTijd += tijdstap;

    // Verzamel statistieken
    verzamelStatistieken();

    ENSURE(huidigeSimulatieTijd > 0.0, "huidigeSimulatieTijd moet toenemen na een stap");
}

/**
 * @brief Verwerk alle verkeerslichten in de simulatie
 * @pre properlyInitialized() == true
 * @post Alle verkeerslichten zijn bijgewerkt volgens hun cyclustijd
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVerkeerslichten");
 */
void simulatie::verwerkVerkeerslichten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVerkeerslichten");

    // Update alle verkeerslichten
    for (auto& licht : verkeerssituatie.getVerkeerslichten()) {
        licht.update(tijdstap);
    }
}

/**
 * @brief Controleer of een voertuig een verkeerslicht nadert en bepaal actie
 * @param voertuig Het te controleren voertuig
 * @param verkeerslicht Het te controleren verkeerslicht
 * @return Responscode: 0 = geen effect, 1 = vertragen, 2 = stoppen, 3 = doorrijden
 * @pre properlyInitialized() == true
 * @pre voertuig.properlyInitialized() == true
 * @pre verkeerslicht.properlyInitialized() == true
 * @post Prioriteitsvoertuigen worden geregistreerd bij slimme verkeerslichten
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");
 */
int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");

    double positieLicht = verkeerslicht.getPositie();
    double positieVoertuig = voertuig.getPositie();
    double afstand = positieLicht - positieVoertuig;

    // Prioriteitsvoertuigen mogen door rood rijden
    if (voertuig.isPrioriteitsVoertuig()) {
        // Maar ze worden nog steeds geregistreerd bij slimme verkeerslichten
        if (verkeerslicht.getIsSlim() && verkeerslicht.isRood() && afstand < stopAfstand) {
            const_cast<Verkeerslicht&>(verkeerslicht).registerVoertuigVoorLicht();
        }
        return 0; // Geen reactie nodig voor prioriteitsvoertuigen
    }

    // Controleer of het verkeerslicht rood of oranje is
    if ((verkeerslicht.isRood() || verkeerslicht.isOranje()) && afstand > 0) {
        // Registreer voertuig bij slim verkeerslicht
        if (verkeerslicht.getIsSlim() && afstand < vertraagAfstand) {
            const_cast<Verkeerslicht&>(verkeerslicht).registerVoertuigVoorLicht();
        }

        // Als het voertuig binnen de stopafstand is, moet het stoppen
        if (afstand < stopAfstand) {
            return 2; // Stop
        }
        // Als het voertuig binnen de vertraagafstand is, moet het vertragen
        else if (afstand < vertraagAfstand) {
            return 1; // Vertragen
        }
    }
    // Verkeerslicht is groen of voertuig is er voorbij
    else if (verkeerslicht.isGroen() && afstand > 0 && afstand < stopAfstand) {
        return 3; // Doorrijden
    }

    return 0; // Geen effect
}

/**
 * @brief Verwerk alle voertuigen in de simulatie
 * @pre properlyInitialized() == true
 * @post Alle voertuigen hebben nieuwe posities en snelheden
 * @post Voertuigen die de baan verlaten zijn verwijderd
 * @post verwijderdeVoertuigenTeller is bijgewerkt
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVoertuigen");
 */
void simulatie::verwerkVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVoertuigen");

    const auto& banen = verkeerssituatie.getBanen();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Sorteer voertuigen per baan, van hoog naar laag positie
    std::map<std::string, std::vector<Voertuig*>> voertuigenPerBaan;

    for (auto& voertuig : voertuigen) {
        voertuigenPerBaan[voertuig->getBaanNaam()].push_back(voertuig.get());
    }

    // Sorteer voertuigen binnen elke baan
    for (auto& baanPaar : voertuigenPerBaan) {
        std::sort(baanPaar.second.begin(), baanPaar.second.end(),
                 [](const Voertuig* a, const Voertuig* b) {
                     return a->getPositie() > b->getPositie();
                 });
    }

    // Verwerk voertuigen per baan
    verwijderdeVoertuigenTeller = 0;

    for (auto& baanPaar : voertuigenPerBaan) {
        const std::string& baanNaam = baanPaar.first;
        std::vector<Voertuig*>& voertuigenOpBaan = baanPaar.second;

        // Controleer of de baan bestaat
        auto baanIt = banen.find(baanNaam);
        if (baanIt == banen.end()) {
            continue;
        }

        double baanLengte = baanIt->second.getLengte();

        // Verwerk elk voertuig op deze baan
        for (size_t i = 0; i < voertuigenOpBaan.size(); ++i) {
            Voertuig* voertuig = voertuigenOpBaan[i];

            // Sla voertuigen over die bij een bushalte wachten
            if (voertuig->isWaitingAtStop()) {
                continue;
            }

            // Bepaal versnelling op basis van voorliggend voertuig of andere factoren
            double verkeersLichtVertraagFactor = 0.4; // Default 0.4 (B.4)

            // Controleer op verkeerslicht nadering
            Verkeerslicht* eerstvolgendeVerkeerslicht = verkeerssituatie.zoekEerstvolgendeVerkeerslicht(*voertuig);

            if (eerstvolgendeVerkeerslicht) {
                int actie = controleerVerkeerslichtNadering(*voertuig, *eerstvolgendeVerkeerslicht);

                if (actie == 1) { // Vertragen
                    verkeersLichtVertraagFactor = this->vertraagFactor;
                } else if (actie == 2) { // Stoppen
                    voertuig->setSnelheid(0.0);
                    voertuig->setVersnelling(0.0);
                    continue; // Sla de normale versnellingsberekening over
                }
                // Anders normale versnelling berekenen
            }

            // Bepaal of dit het eerste voertuig op deze baan is
            bool isEersteVoertuig = (i == 0);

            // Bepaal voorliggend voertuig (indien niet eerste)
            Voertuig* voorliggendVoertuig = nullptr;
            if (!isEersteVoertuig) {
                voorliggendVoertuig = voertuigenOpBaan[i-1];
            }

            // Bereken versnelling van het voertuig (simplified version)
            // In plaats van een complexe berekenVersnelling methode, gebruiken we een simpele versie
            if (voorliggendVoertuig) {
                double afstandTotVoorligger = voorliggendVoertuig->getPositie() - voertuig->getPositie();
                if (afstandTotVoorligger < 10.0) { // Te dichtbij
                    voertuig->setVersnelling(-2.0 * verkeersLichtVertraagFactor);
                } else {
                    voertuig->setVersnelling(1.0 * verkeersLichtVertraagFactor);
                }
            } else {
                voertuig->setVersnelling(1.0 * verkeersLichtVertraagFactor);
            }

            // Update positie en snelheid
            voertuig->update(tijdstap);

            // Controleer of het voertuig het einde van de baan heeft bereikt
            if (voertuig->getPositie() >= baanLengte) {
                // Markeer voor verwijdering
                voertuig->setPositie(-1.0); // Speciale markering voor verwijdering
            }
        }
    }

    // Verwijder voertuigen die de baan hebben verlaten
    auto beginIt = std::remove_if(voertuigen.begin(), voertuigen.end(),
                                 [](const std::unique_ptr<Voertuig>& voertuig) {
                                     return voertuig->getPositie() < 0;
                                 });

    // Tel aantal verwijderde voertuigen
    verwijderdeVoertuigenTeller = std::distance(beginIt, voertuigen.end());
    totaalVerwijderdeVoertuigen += verwijderdeVoertuigenTeller;

    // Voer daadwerkelijke verwijdering uit
    voertuigen.erase(beginIt, voertuigen.end());
}

/**
 * @brief Verwerk alle bushaltes in de simulatie
 * @pre properlyInitialized() == true
 * @post Bussen stoppen bij bushaltes en vertrekken na wachttijd
 * @post Bushalte bezettingsstatus is correct bijgewerkt
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkBushaltes");
 */
void simulatie::verwerkBushaltes() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkBushaltes");

    auto& bushaltes = verkeerssituatie.getBushaltes();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Doorloop alle bushaltes
    for (auto& bushalte : bushaltes) {
        // Controleer of er een bus bij de halte wacht
        if (bushalte.isBusGestopt()) {
            // Update de wachttijd
            if (bushalte.updateWachttijd(tijdstap)) {
                // Wachttijd is verstreken, markeer de bushalte als vrij
                bushalte.setBusLeft();

                // Zoek de bijbehorende bus en markeer dat deze weer mag rijden
                for (auto& voertuig : voertuigen) {
                    if (voertuig->isBus() &&
                        voertuig->getBaanNaam() == bushalte.getBaan() &&
                        voertuig->isWaitingAtStop() &&
                        std::abs(voertuig->getPositie() - bushalte.getPositie()) < 1.0) {

                        voertuig->setWaitingAtStop(false);
                        break;
                    }
                }
            }
        } else {
            // Controleer of er een bus bij de halte is aangekomen
            for (auto& voertuig : voertuigen) {
                if (voertuig->isBus() &&
                    voertuig->getBaanNaam() == bushalte.getBaan() &&
                    !voertuig->isWaitingAtStop() &&
                    voertuig->getPositie() > bushalte.getPositie() - 1.0 &&
                    voertuig->getPositie() < bushalte.getPositie() + 1.0) {

                    // Bus is bij de halte aangekomen, laat de bus stoppen
                    voertuig->setSnelheid(0.0);
                    voertuig->setVersnelling(0.0);
                    voertuig->setWaitingAtStop(true);

                    // Markeer de bushalte als bezet
                    bushalte.setBusStopped();
                    bushalte.resetWachttijd();
                    break;
                }
            }
        }
    }
}

/**
 * @brief Verwerk alle kruispunten in de simulatie
 * @pre properlyInitialized() == true
 * @post Voertuigen bij kruispunten worden verplaatst naar nieuwe banen
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");
 */
void simulatie::verwerkKruispunten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");

    const auto& kruispunten = verkeerssituatie.getKruispunten();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Doorloop alle voertuigen om te controleren of ze op een kruispunt zijn
    for (auto& voertuig : voertuigen) {
        // Sla voertuigen over die bij een bushalte wachten
        if (voertuig->isWaitingAtStop()) {
            continue;
        }

        // Controleer elk kruispunt
        for (const auto& kruispunt : kruispunten) {
            if (moetNaarKruispunt(*voertuig, kruispunt)) {
                // Als het voertuig naar een andere baan moet, verplaats het
                verplaatsVoertuigNaKruispunt(*voertuig, kruispunt);
                break;
            }
        }
    }
}

/**
 * @brief Controleer of een voertuig naar een kruispunt moet worden verplaatst
 * @param voertuig Te controleren voertuig
 * @param kruispunt Te controleren kruispunt
 * @return true als het voertuig naar een kruispunt moet worden verplaatst, false indien niet
 * @pre properlyInitialized() == true
 * @pre voertuig.properlyInitialized() == true
 * @pre kruispunt.properlyInitialized() == true
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij moetNaarKruispunt");
 */
bool simulatie::moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij moetNaarKruispunt");

    // Controleer of het voertuig op een baan zit die deel uitmaakt van het kruispunt
    if (!kruispunt.bevatBaan(voertuig.getBaanNaam())) {
        return false;
    }

    // Bepaal de positie van het kruispunt op deze baan
    double kruispuntPositie = kruispunt.getPositieOpBaan(voertuig.getBaanNaam());

    // Als het voertuig het kruispunt heeft bereikt of net voorbij is (0.5m marge)
    return voertuig.getPositie() >= kruispuntPositie &&
           voertuig.getPositie() <= kruispuntPositie + 0.5;
}

/**
 * @brief Verplaats een voertuig naar een nieuwe baan na een kruispunt
 * @param voertuig Het te verplaatsen voertuig
 * @param kruispunt Het kruispunt waar het voertuig zich bevindt
 * @return true als het voertuig werd verplaatst, false indien niet
 * @pre properlyInitialized() == true
 * @pre voertuig.properlyInitialized() == true
 * @pre kruispunt.properlyInitialized() == true
 * @post Bij succes: voertuig bevindt zich op nieuwe baan
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verplaatsVoertuigNaKruispunt");
 */
bool simulatie::verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verplaatsVoertuigNaKruispunt");

    // Kies willekeurig een nieuwe baan vanuit het kruispunt (behalve de huidige)
    std::string nieuweBaan = kruispunt.kiesRandomBaan(voertuig.getBaanNaam());

    // Als er geen geschikte baan is (bijv. als er slechts één baan is), doe niets
    if (nieuweBaan.empty()) {
        return false;
    }

    // Zet het voertuig op de nieuwe baan, direct na het kruispunt
    double oudeSnelheid = voertuig.getSnelheid();
    double oudeVersnelling = voertuig.getVersnelling();
    double nieuwePosOpKruispunt = 0.0;

    // Positioneer het voertuig op de nieuwe baan, net na het kruispunt
    nieuwePosOpKruispunt = kruispunt.getPositieOpBaan(nieuweBaan) + 0.5;

    // Update het voertuig met de nieuwe baan en positie
    voertuig.setBaanNaam(nieuweBaan);
    voertuig.setPositie(nieuwePosOpKruispunt);

    // Behoud de oude snelheid en versnelling
    voertuig.setSnelheid(oudeSnelheid);
    voertuig.setVersnelling(oudeVersnelling);

    return true;
}

/**
 * @brief Genereer nieuwe voertuigen vanaf voertuiggeneratoren
 * @pre properlyInitialized() == true
 * @pre autoGenereerVoertuigen == true
 * @post Nieuwe voertuigen zijn toegevoegd volgens generator frequenties
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereerNieuweVoertuigen");
 */
void simulatie::genereerNieuweVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereerNieuweVoertuigen");

    // Doorloop alle voertuiggeneratoren
    for (const auto& generator : verkeerssituatie.getVoertuigGenerators()) {
        // Bepaal of een voertuig moet worden gegenereerd op basis van frequentie
        double frequentieInSeconden = static_cast<double>(generator.getFrequentie());

        // Controleer of er een veelvoud van de frequentietijd is verstreken
        if (std::fmod(huidigeSimulatieTijd, frequentieInSeconden) < tijdstap) {
            // Genereer een voertuig van het juiste type aan het begin van de baan
            genereertVoertuig(generator.getBaanNaam(), 0.0, generator.getType());
        }
    }
}

/**
 * @brief Genereer een nieuw voertuig op een specifieke baan
 * @param baanNaam Naam van de baan waarop het voertuig wordt geplaatst
 * @param positie Startpositie op de baan in meters
 * @param type Type voertuig ("auto", "bus", "brandweerwagen", etc.)
 * @return true als het voertuig succesvol werd gegenereerd, false indien niet
 * @pre properlyInitialized() == true
 * @pre !baanNaam.empty()
 * @pre positie >= 0.0
 * @post Bij succes: nieuw voertuig is toegevoegd aan verkeerssituatie
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereertVoertuig");
 * REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
 * REQUIRE(positie >= 0.0, "positie moet groter of gelijk aan 0 zijn.");
 */
bool simulatie::genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereertVoertuig");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "positie moet groter of gelijk aan 0 zijn.");

    // Controleer of de baan bestaat
    if (verkeerssituatie.getBanen().find(baanNaam) == verkeerssituatie.getBanen().end()) {
        return false;
    }

    // Controleer of er al een voertuig op deze positie staat
    for (const auto& bestaandVoertuig : verkeerssituatie.getVoertuigen()) {
        if (bestaandVoertuig->getBaanNaam() == baanNaam &&
            std::abs(bestaandVoertuig->getPositie() - positie) < 5.0) {
            return false; // Er staat al een voertuig te dichtbij
        }
    }

    // Gebruik factory methode om het juiste type voertuig te maken
    auto nieuwVoertuig = Voertuig::maakVoertuig(baanNaam, positie, type);

    // Voeg het voertuig toe aan de verkeerssituatie
    return verkeerssituatie.voegVoertuigToe(std::move(nieuwVoertuig));
}

/**
 * @brief Verzamel simulatiestatistieken
 * @pre properlyInitialized() == true
 * @post aantalVoertuigen en gemiddeldeSnelheid zijn bijgewerkt
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verzamelStatistieken");
 */
void simulatie::verzamelStatistieken() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verzamelStatistieken");

    // Update aantal voertuigen
    aantalVoertuigen = static_cast<int>(verkeerssituatie.getVoertuigen().size());

    // Bereken gemiddelde snelheid
    double totaleSnelheid = 0.0;
    for (const auto& voertuig : verkeerssituatie.getVoertuigen()) {
        totaleSnelheid += voertuig->getSnelheid();
    }

    // Voorkom deling door nul
    if (aantalVoertuigen > 0) {
        gemiddeldeSnelheid = totaleSnelheid / aantalVoertuigen;
    } else {
        gemiddeldeSnelheid = 0.0;
    }
}

/**
 * @brief Schakel automatische voertuiggeneratie in of uit
 * @param genereer Of voertuigen automatisch moeten worden gegenereerd
 * @pre properlyInitialized() == true
 * @post autoGenereerVoertuigen == genereer
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij setAutoGenereerVoertuigen");
 */
void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij setAutoGenereerVoertuigen");
    autoGenereerVoertuigen = genereer;
}

/**
 * @brief Haal de huidige simulatietijd op
 * @return De huidige simulatietijd in seconden
 * @pre properlyInitialized() == true
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getHuidigeSimulatieTijd");
 */
double simulatie::getHuidigeSimulatieTijd() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getHuidigeSimulatieTijd");
    return huidigeSimulatieTijd;
}

/**
 * @brief Haal de tijdstap op
 * @return De tijdstap in seconden
 * @pre properlyInitialized() == true
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTijdstap");
 */
double simulatie::getTijdstap() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTijdstap");
    return tijdstap;
}

/**
 * @brief Haal het huidige aantal voertuigen op
 * @return Het aantal voertuigen in de simulatie
 * @pre properlyInitialized() == true
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getAantalVoertuigen");
 */
int simulatie::getAantalVoertuigen() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getAantalVoertuigen");
    return aantalVoertuigen;
}

/**
 * @brief Haal de gemiddelde snelheid van alle voertuigen op
 * @return De gemiddelde snelheid in m/s
 * @pre properlyInitialized() == true
 * @post result >= 0.0
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getGemiddeldeSnelheid");
 */
double simulatie::getGemiddeldeSnelheid() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getGemiddeldeSnelheid");
    return gemiddeldeSnelheid;
}

/**
 * @brief Haal het totale aantal verwijderde voertuigen op
 * @return Het totale aantal verwijderde voertuigen sinds start simulatie
 * @pre properlyInitialized() == true
 * @post result >= 0
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaalVerwijderdeVoertuigen");
 */
int simulatie::getTotaalVerwijderdeVoertuigen() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaalVerwijderdeVoertuigen");
    return totaalVerwijderdeVoertuigen;
}

/**
 * @brief Haal de totale gesimuleerde tijd op
 * @return De totale tijd sinds start van de simulatie in seconden
 * @pre properlyInitialized() == true
 * @post result >= 0.0
 * REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaleTijd");
 */
double simulatie::getTotaleTijd() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaleTijd");
    return totaleTijd;
}