/**
 * @file simulatie.cpp
 * @brief Implementatie van de simulatie klasse voor verkeerssimulatie
 */

#include "simulatie.h"
#include "DesignByContract.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>
#include <random>

/**
 * @brief Constructor voor simulatie klasse
 * @param situatie Referentie naar de verkeerssituatie
 * @param tijdstap Tijdstap voor de simulatie (default 0.0166s ≈ 60fps)
 * @pre tijdstap > 0.0
 * @post properlyInitialized() == true
 * @post getTijdstap() == tijdstap (or 0.0166 if tijdstap <= 0)
 * @post getHuidigeSimulatieTijd() == 0.0
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
// zijn de defaultwaardes
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
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij stap");
 * @post ENSURE(huidigeSimulatieTijd > 0.0, "huidigeSimulatieTijd moet toenemen na een stap");
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
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVerkeerslichten");
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
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");
 */
int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");

    double positieLicht = verkeerslicht.getPositie();
    double positieVoertuig = voertuig.getPositie();
    double afstand = positieLicht - positieVoertuig;

    // Prioriteitsvoertuigen mogen door rood rijden - GECORRIGEERD: functienaam
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

/**
 * @brief Verwerk alle voertuigen in de simulatie
 * @pre properlyInitialized() == true
 * @post Alle voertuigen hebben nieuwe posities en snelheden
 * @post Voertuigen die de baan verlaten zijn verwijderd
 * @post verwijderdeVoertuigenTeller is bijgewerkt
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVoertuigen");
 */
void simulatie::verwerkVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVoertuigen");

    const auto& banen = verkeerssituatie.getBanen();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Verwerk elke baan afzonderlijk
    for (const auto& baanPair : banen) {
        const std::string& baanNaam = baanPair.first;
        double baanLengte = baanPair.second.getLengte(); // baanPair.second is een Baan object

        // Verzamel alle voertuigen op deze baan
        std::vector<Voertuig*> voertuigenOpBaan;
        for (auto& voertuig : voertuigen) {
            if (voertuig->getBaanNaam() == baanNaam) {
                voertuigenOpBaan.push_back(voertuig.get());
            }
        }

        // Sorteer voertuigen op positie (hoogste positie eerst)
        std::sort(voertuigenOpBaan.begin(), voertuigenOpBaan.end(),
                  [](const Voertuig* a, const Voertuig* b) {
                      return a->getPositie() > b->getPositie();
                  });

        // Verwerk elk voertuig op deze baan
        for (size_t i = 0; i < voertuigenOpBaan.size(); i++) {
            Voertuig* voertuig = voertuigenOpBaan[i];

            // Bepaal verkeerslicht vertraagfactor
            double verkeersLichtVertraagFactor = 1.0;

            // Controleer verkeerslichten op deze baan
            for (const auto& verkeerslicht : verkeerssituatie.getVerkeerslichten()) {
                if (verkeerslicht.getBaan() == baanNaam) {
                    int response = controleerVerkeerslichtNadering(*voertuig, verkeerslicht);
                    if (response == 1) { // Vertragen
                        verkeersLichtVertraagFactor = vertraagFactor;
                    } else if (response == 2) { // Stoppen
                        verkeersLichtVertraagFactor = 0.0;
                    }
                    // Voor prioriteitsvoertuigen (response 0) of groen licht (response 3): geen aanpassing
                }
            }

            // Bepaal of dit het eerste voertuig is
            bool isEersteVoertuig = (i == 0);

            // Bepaal voorliggend voertuig (indien niet eerste)
            Voertuig* voorliggendVoertuig = nullptr;
            if (!isEersteVoertuig) {
                voorliggendVoertuig = voertuigenOpBaan[i-1];
            }

            // Bereken versnelling van het voertuig - GECORRIGEERD: 4 parameters
            voertuig->berekenVersnelling(voorliggendVoertuig, isEersteVoertuig, verkeersLichtVertraagFactor, -1.0);

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

/**
 * @brief Verwerk alle bushaltes in de simulatie
 * @pre properlyInitialized() == true
 * @post Bussen stoppen bij bushaltes en vertrekken na wachttijd
 * @post Bushalte bezettingsstatus is correct bijgewerkt
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkBushaltes");
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

                // Zoek de bijbehorende bus en laat hem doorrijden
                for (auto& voertuig : voertuigen) {
                    if (voertuig->isBus() &&
                        voertuig->getBaanNaam() == bushalte.getBaan() &&
                        std::abs(voertuig->getPositie() - bushalte.getPositie()) < 10.0) {
                        // GECORRIGEERD: functienaam
                        voertuig->setIsWaitingAtStop(false);
                        break;
                    }
                }
            }
        } else {
            // Controleer of er een bus de halte nadert
            for (auto& voertuig : voertuigen) {
                if (voertuig->isBus() &&
                    voertuig->getBaanNaam() == bushalte.getBaan()) {

                    double afstand = bushalte.getPositie() - voertuig->getPositie();

                    // Bus bereikt de halte
                    if (afstand >= 0 && afstand <= 5.0) {
                        // Stop de bus bij de halte
                        // Bushalte heeft geen setBusArrived methode, we gebruiken alleen voertuig status
                        // GECORRIGEERD: functienaam
                        voertuig->setIsWaitingAtStop(true);
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief Verwerk alle kruispunten in de simulatie
 * @pre properlyInitialized() == true
 * @post Voertuigen bij kruispunten worden verplaatst naar nieuwe banen
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");
 */
void simulatie::verwerkKruispunten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");

    auto& kruispunten = verkeerssituatie.getKruispunten();
    auto& voertuigen = verkeerssituatie.getVoertuigen();

    // Random number generator voor willekeurige keuze van nieuwe baan
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Doorloop alle kruispunten
    for (const auto& kruispunt : kruispunten) {
        // Doorloop alle voertuigen
        for (auto& voertuig : voertuigen) {
            // Zoek kruispunt voor dit voertuig (vereenvoudigde logica)
            for (const auto& baan : kruispunt.getBanen()) {
                if (voertuig->getBaanNaam() == baan.first) {
                    // Haal baanlengte op uit de verkeerssituatie
                    const auto& alleBanen = verkeerssituatie.getBanen();
                    auto baanIt = alleBanen.find(baan.first);
                    if (baanIt != alleBanen.end()) {
                        double baanLengte = baanIt->second.getLengte();

                        // Controleer of voertuig aan het einde van de baan is
                        if (voertuig->getPositie() >= baanLengte - 5.0) {

                            // Krijg aangesloten banen
                            const auto& aangeslotenBanen = kruispunt.getBanen();

                            if (!aangeslotenBanen.empty()) {
                                // Kies willekeurig een nieuwe baan
                                std::uniform_int_distribution<> dis(0, aangeslotenBanen.size() - 1);
                                int index = dis(gen);

                                auto it = aangeslotenBanen.begin();
                                std::advance(it, index);

                                // Verplaats voertuig naar nieuwe baan
                                voertuig->setBaanNaam(it->first);
                                voertuig->setPositie(0.0); // Start aan het begin van de nieuwe baan
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Genereer nieuwe voertuigen vanaf voertuiggeneratoren
 * @pre properlyInitialized() == true
 * @pre autoGenereerVoertuigen == true
 * @post Nieuwe voertuigen zijn toegevoegd volgens generator frequenties
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereerNieuweVoertuigen");
 */
void simulatie::genereerNieuweVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereerNieuweVoertuigen");

    auto& generators = verkeerssituatie.getVoertuigGenerators();

    for (auto& generator : generators) {
        // Eenvoudige timing logica - genereer elke X seconden
        static double lastGenerationTime = 0.0;
        if (huidigeSimulatieTijd - lastGenerationTime >= generator.getFrequentie()) {
            // Probeer een nieuw voertuig te genereren
            if (genereertVoertuig(generator.getBaanNaam(), 0.0, generator.getType())) {
                lastGenerationTime = huidigeSimulatieTijd;
            }
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
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereertVoertuig");
 * @pre REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
 * @pre REQUIRE(positie >= 0.0, "positie moet groter of gelijk aan 0 zijn.");
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
 * @pre REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verzamelStatistieken");
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

// =============================================================================
// Getter methodes
// =============================================================================

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
    // Rond af tot 2 decimalen
    return std::round(gemiddeldeSnelheid * 100.0) / 100.0;
}

int simulatie::getTotaalVerwijderdeVoertuigen() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaalVerwijderdeVoertuigen");
    return totaalVerwijderdeVoertuigen;
}

double simulatie::getTotaleTijd() const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij getTotaleTijd");
    return totaleTijd;
}

// =============================================================================
// Setter methodes
// =============================================================================

void simulatie::setAutoGenereerVoertuigen(bool waarde) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij setAutoGenereerVoertuigen");
    autoGenereerVoertuigen = waarde;
    ENSURE(autoGenereerVoertuigen == waarde, "autoGenereerVoertuigen werd niet correct ingesteld");
}