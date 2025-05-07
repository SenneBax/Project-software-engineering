/**
 * @file simulatie.cpp
 * @brief Implementatie van de simulatieklasse
 */

#include "../Simulation/simulatie.h"
#include <algorithm>
#include <cmath>
#include <random>
#include "DesignByContract.h"

// Constructor
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
      totaleTijd(0.0),
      autoGenereerVoertuigen(false)
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

bool simulatie::properlyInitialized() const
{
    return _initCheck == this;
}

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

void simulatie::verwerkVerkeerslichten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVerkeerslichten");

    // Update alle verkeerslichten
    for (auto& licht : verkeerssituatie.getVerkeerslichten()) {
        licht.update(tijdstap);
    }
}

int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");

    double positieLicht = verkeerslicht.getPositie();
    double positieVoertuig = voertuig.getPositie();
    double afstand = positieLicht - positieVoertuig;

    // Prioriteitsvoertuigen mogen door rood rijden
    if (voertuig.isPrioriteitsvoertuig()) {
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
            if (voertuig->isWaitingAtBusStop()) {
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
                    voertuig->noodStop();
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

            // Bereken versnelling van het voertuig
            voertuig->berekenVersnelling(voorliggendVoertuig, isEersteVoertuig, verkeersLichtVertraagFactor);

            // Update positie en snelheid
            voertuig->updatePositieEnSnelheid(tijdstap);

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
                        voertuig->isWaitingAtBusStop() &&
                        std::abs(voertuig->getPositie() - bushalte.getPositie()) < 1.0) {

                        voertuig->setIsWaitingAtBusStop(false);
                        break;
                    }
                }
            }
        } else {
            // Controleer of er een bus bij de halte is aangekomen
            for (auto& voertuig : voertuigen) {
                if (voertuig->isBus() &&
                    voertuig->getBaanNaam() == bushalte.getBaan() &&
                    !voertuig->isWaitingAtBusStop() &&
                    voertuig->getPositie() > bushalte.getPositie() - 1.0 &&
                    voertuig->getPositie() < bushalte.getPositie() + 1.0) {

                    // Bus is bij de halte aangekomen, laat de bus stoppen
                    voertuig->setSnelheid(0.0);
                    voertuig->setVersnelling(0.0);
                    voertuig->setIsWaitingAtBusStop(true);

                    // Markeer de bushalte als bezet
                    bushalte.setBusStopped();
                    bushalte.resetWachttijd();
                    break;
                }
            }
        }
    }
}

void simulatie::verwerkKruispunten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");

    const auto& kruispunten = verkeerssituatie.getKruispunten();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Doorloop alle voertuigen om te controleren of ze op een kruispunt zijn
    for (auto& voertuig : voertuigen) {
        // Sla voertuigen over die bij een bushalte wachten
        if (voertuig->isWaitingAtBusStop()) {
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

void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij setAutoGenereerVoertuigen");
    autoGenereerVoertuigen = genereer;
}

double simulatie::getHuidigeSimulatieTijd() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getHuidigeSimulatieTijd");
    return huidigeSimulatieTijd;
}

double simulatie::getTijdstap() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTijdstap");
    return tijdstap;
}

int simulatie::getAantalVoertuigen() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getAantalVoertuigen");
    return aantalVoertuigen;
}

double simulatie::getGemiddeldeSnelheid() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getGemiddeldeSnelheid");
    return gemiddeldeSnelheid;
}

int simulatie::getTotaalVerwijderdeVoertuigen() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaalVerwijderdeVoertuigen");
    return totaalVerwijderdeVoertuigen;
}

double simulatie::getTotaleTijd() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaleTijd");
    return totaleTijd;
}