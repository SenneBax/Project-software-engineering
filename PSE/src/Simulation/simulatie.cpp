/**
 * @file simulatie.cpp
 * @brief Veilige implementatie van de simulatie klasse gebaseerd op bestaande code
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
 */
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie),
      tijdstap(tijdstap > 0.0 ? tijdstap : 0.0166),
      huidigeSimulatieTijd(0.0),
      autoGenereerVoertuigen(false),
      vertraagAfstand(250.0),
      stopAfstand(50.0),
      vertraagFactor(0.4),
      aantalVoertuigen(0),
      gemiddeldeSnelheid(0.0),
      verwijderdeVoertuigenTeller(0),
      totaalVerwijderdeVoertuigen(0),
      totaleTijd(0.0)
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}

/**
 * @brief Controleer of het simulatie object correct is geïnitialiseerd
 */
bool simulatie::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Voer één simulatiestap uit
 */
void simulatie::stap() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij stap");

    try {
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

    } catch (const std::exception& e) {
        std::cerr << "Fout in simulatiestap: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Onbekende fout in simulatiestap" << std::endl;
    }

    ENSURE(huidigeSimulatieTijd > 0.0, "huidigeSimulatieTijd moet toenemen na een stap");
}

/**
 * @brief Verwerk alle verkeerslichten in de simulatie
 */
void simulatie::verwerkVerkeerslichten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVerkeerslichten");

    try {
        // Update alle verkeerslichten
        for (auto& licht : verkeerssituatie.getVerkeerslichten()) {
            try {
                licht.update(tijdstap);
            } catch (...) {
                // Skip dit verkeerslicht bij fouten
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fout bij verwerken verkeerslichten: " << e.what() << std::endl;
    }
}

/**
 * @brief Controleer verkeerslicht nadering
 */
int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij controleerVerkeerslichtNadering");

    try {
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
    } catch (...) {
        return 0; // Bij fout geen effect
    }
}

/**
 * @brief Verwerk alle voertuigen in de simulatie
 */
void simulatie::verwerkVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkVoertuigen");

    try {
        const auto& banen = verkeerssituatie.getBanen();
        auto& voertuigen = verkeerssituatie.getVoertuigen();

        // Verwerk elke baan afzonderlijk
        for (const auto& baanPair : banen) {
            try {
                const std::string& baanNaam = baanPair.first;
                double baanLengte = baanPair.second.getLengte();

                // Verzamel alle voertuigen op deze baan
                std::vector<Voertuig*> voertuigenOpBaan;
                for (auto& voertuig : voertuigen) {
                    if (voertuig && voertuig->getBaanNaam() == baanNaam) {
                        voertuigenOpBaan.push_back(voertuig.get());
                    }
                }

                // Sorteer voertuigen op positie (hoogste positie eerst)
                std::sort(voertuigenOpBaan.begin(), voertuigenOpBaan.end(),
                          [](const Voertuig* a, const Voertuig* b) {
                              if (!a || !b) return false;
                              try {
                                  return a->getPositie() > b->getPositie();
                              } catch (...) {
                                  return false;
                              }
                          });

                // Verwerk elk voertuig op deze baan
                for (size_t i = 0; i < voertuigenOpBaan.size(); i++) {
                    try {
                        Voertuig* voertuig = voertuigenOpBaan[i];
                        if (!voertuig) continue;

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
                            }
                        }

                        // Bepaal of dit het eerste voertuig is
                        bool isEersteVoertuig = (i == 0);

                        // Bepaal voorliggend voertuig
                        Voertuig* voorliggendVoertuig = nullptr;
                        if (!isEersteVoertuig && i > 0) {
                            voorliggendVoertuig = voertuigenOpBaan[i-1];
                        }

                        // Bereken versnelling van het voertuig
                        voertuig->berekenVersnelling(voorliggendVoertuig, isEersteVoertuig, verkeersLichtVertraagFactor, -1.0);

                        // Update positie en snelheid
                        voertuig->updatePositieEnSnelheid(tijdstap);

                        // Controleer of het voertuig het einde van de baan heeft bereikt
                        if (voertuig->getPositie() >= baanLengte) {
                            // Markeer voor verwijdering
                            voertuig->setPositie(-1.0);
                        }

                    } catch (...) {
                        // Skip dit voertuig bij fouten
                        continue;
                    }
                }
            } catch (...) {
                // Skip deze baan bij fouten
                continue;
            }
        }

        // Verwijder voertuigen die de baan hebben verlaten - VEILIG
        try {
            auto beginIt = std::remove_if(voertuigen.begin(), voertuigen.end(),
                                         [](const std::unique_ptr<Voertuig>& voertuig) {
                                             if (!voertuig) return true;
                                             try {
                                                 return voertuig->getPositie() < 0;
                                             } catch (...) {
                                                 return true; // Verwijder beschadigde voertuigen
                                             }
                                         });

            // Tel aantal verwijderde voertuigen
            verwijderdeVoertuigenTeller = std::distance(beginIt, voertuigen.end());
            totaalVerwijderdeVoertuigen += verwijderdeVoertuigenTeller;

            // Voer daadwerkelijke verwijdering uit
            voertuigen.erase(beginIt, voertuigen.end());
        } catch (...) {
            std::cerr << "Fout bij verwijderen voertuigen" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fout bij verwerken voertuigen: " << e.what() << std::endl;
    }
}

/**
 * @brief Verwerk alle bushaltes in de simulatie
 */
void simulatie::verwerkBushaltes() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkBushaltes");

    try {
        auto& bushaltes = verkeerssituatie.getBushaltes();
        auto& voertuigen = verkeerssituatie.getVoertuigen();

        // Doorloop alle bushaltes
        for (auto& bushalte : bushaltes) {
            try {
                // Controleer of er een bus bij de halte wacht
                if (bushalte.isBusGestopt()) {
                    // Update de wachttijd
                    if (bushalte.updateWachttijd(tijdstap)) {
                        // Wachttijd is verstreken, markeer de bushalte als vrij
                        bushalte.setBusLeft();

                        // Zoek de bijbehorende bus en laat hem doorrijden
                        for (auto& voertuig : voertuigen) {
                            if (voertuig && voertuig->isBus() &&
                                voertuig->getBaanNaam() == bushalte.getBaan() &&
                                std::abs(voertuig->getPositie() - bushalte.getPositie()) < 10.0) {
                                voertuig->setIsWaitingAtStop(false);
                                break;
                            }
                        }
                    }
                } else {
                    // Controleer of er een bus de halte nadert
                    for (auto& voertuig : voertuigen) {
                        if (voertuig && voertuig->isBus() &&
                            voertuig->getBaanNaam() == bushalte.getBaan()) {

                            double afstand = bushalte.getPositie() - voertuig->getPositie();

                            // Bus bereikt de halte
                            if (afstand >= 0 && afstand <= 5.0) {
                                // Stop de bus bij de halte
                                voertuig->setIsWaitingAtStop(true);
                                break;
                            }
                        }
                    }
                }
            } catch (...) {
                // Skip deze bushalte bij fouten
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fout bij verwerken bushaltes: " << e.what() << std::endl;
    }
}

/**
 * @brief Verwerk alle kruispunten in de simulatie
 */
void simulatie::verwerkKruispunten() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verwerkKruispunten");

    try {
        auto& kruispunten = verkeerssituatie.getKruispunten();
        auto& voertuigen = verkeerssituatie.getVoertuigen();

        // Random number generator voor willekeurige keuze van nieuwe baan
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // Doorloop alle kruispunten
        for (const auto& kruispunt : kruispunten) {
            try {
                // Doorloop alle voertuigen
                for (auto& voertuig : voertuigen) {
                    if (!voertuig) continue;

                    try {
                        // Zoek kruispunt voor dit voertuig
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
                                            voertuig->setPositie(0.0);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    } catch (...) {
                        // Skip dit voertuig bij fouten
                        continue;
                    }
                }
            } catch (...) {
                // Skip dit kruispunt bij fouten
                continue;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fout bij verwerken kruispunten: " << e.what() << std::endl;
    }
}

/**
 * @brief Genereer nieuwe voertuigen vanaf voertuiggeneratoren
 */
void simulatie::genereerNieuweVoertuigen() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereerNieuweVoertuigen");

    if (!autoGenereerVoertuigen) {
        return;
    }

    try {
        const auto& generators = verkeerssituatie.getVoertuigGenerators();

        if (generators.empty()) {
            return;
        }

        // Verwerk elke generator afzonderlijk met eigen timing
        for (size_t i = 0; i < generators.size(); ++i) {
            try {
                const auto& generator = generators[i];

                if (!generator.properlyInitialized()) {
                    continue;
                }

                std::string baanNaam = generator.getBaanNaam();
                if (baanNaam.empty()) {
                    continue;
                }

                int frequentie = generator.getFrequentie();
                if (frequentie <= 0) {
                    continue;
                }

                // Gebruik unieke key voor elke generator (baan + index)
                std::string generatorKey = baanNaam + "_" + std::to_string(i);

                // Initialiseer lastGenerationTime voor nieuwe generators
                if (lastGenerationTimes.find(generatorKey) == lastGenerationTimes.end()) {
                    lastGenerationTimes[generatorKey] = 0.0;
                }

                // Controleer of er genoeg tijd verstreken is
                double timeSinceLastGeneration = huidigeSimulatieTijd - lastGenerationTimes[generatorKey];
                double frequentieInSeconden = static_cast<double>(frequentie);

                if (timeSinceLastGeneration >= frequentieInSeconden) {
                    // Controleer of de baan nog bestaat
                    const auto& banen = verkeerssituatie.getBanen();
                    if (banen.find(baanNaam) == banen.end()) {
                        continue;
                    }

                    std::string voertuigType = generator.getType();
                    if (voertuigType.empty()) {
                        voertuigType = "auto";
                    }

                    // Probeer een nieuw voertuig te genereren
                    bool success = genereertVoertuig(baanNaam, 0.0, voertuigType);
                    if (success) {
                        lastGenerationTimes[generatorKey] = huidigeSimulatieTijd;
                    }
                }

            } catch (const std::exception& e) {
                std::cerr << "Fout bij generator " << i << ": " << e.what() << std::endl;
                continue;
            } catch (...) {
                std::cerr << "Onbekende fout bij generator " << i << std::endl;
                continue;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Fout in genereerNieuweVoertuigen: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Onbekende fout in genereerNieuweVoertuigen" << std::endl;
    }
}

/**
 * @brief Genereer een nieuw voertuig op een specifieke baan
 */
bool simulatie::genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij genereertVoertuig");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "positie moet groter of gelijk aan 0 zijn.");

    try {
        // Controleer of de baan bestaat
        const auto& banen = verkeerssituatie.getBanen();
        if (banen.find(baanNaam) == banen.end()) {
            return false;
        }

        // Controleer of er al een voertuig op deze positie staat
        const auto& voertuigen = verkeerssituatie.getVoertuigen();
        for (const auto& bestaandVoertuig : voertuigen) {
            if (!bestaandVoertuig) {
                continue;
            }

            try {
                if (bestaandVoertuig->getBaanNaam() == baanNaam &&
                    std::abs(bestaandVoertuig->getPositie() - positie) < 10.0) {
                    return false; // Er staat al een voertuig te dichtbij
                }
            } catch (...) {
                continue; // Skip voertuigen met problemen
            }
        }

        // Gebruik factory methode om het juiste type voertuig te maken
        std::unique_ptr<Voertuig> nieuwVoertuig = nullptr;

        try {
            nieuwVoertuig = Voertuig::maakVoertuig(baanNaam, positie, type);
        } catch (const std::exception& e) {
            std::cerr << "Fout bij aanmaken voertuig: " << e.what() << std::endl;
            return false;
        } catch (...) {
            std::cerr << "Onbekende fout bij aanmaken voertuig" << std::endl;
            return false;
        }

        if (!nieuwVoertuig) {
            return false;
        }

        // Controleer of voertuig correct geïnitialiseerd is
        try {
            if (!nieuwVoertuig->properlyInitialized()) {
                return false;
            }
        } catch (...) {
            return false;
        }

        // Voeg het voertuig toe aan de verkeerssituatie
        try {
            return verkeerssituatie.voegVoertuigToe(std::move(nieuwVoertuig));
        } catch (const std::exception& e) {
            std::cerr << "Fout bij toevoegen voertuig: " << e.what() << std::endl;
            return false;
        } catch (...) {
            return false;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fout in genereertVoertuig: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Onbekende fout in genereertVoertuig" << std::endl;
        return false;
    }
}

/**
 * @brief Verzamel simulatiestatistieken
 */
void simulatie::verzamelStatistieken() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verzamelStatistieken");

    try {
        // Update aantal voertuigen
        const auto& voertuigen = verkeerssituatie.getVoertuigen();
        aantalVoertuigen = static_cast<int>(voertuigen.size());

        // Bereken gemiddelde snelheid
        double totaleSnelheid = 0.0;
        int geldigeTeller = 0;

        for (const auto& voertuig : voertuigen) {
            if (!voertuig) {
                continue;
            }

            try {
                totaleSnelheid += voertuig->getSnelheid();
                geldigeTeller++;
            } catch (...) {
                continue; // Skip voertuigen met problemen
            }
        }

        // Voorkom deling door nul
        if (geldigeTeller > 0) {
            gemiddeldeSnelheid = totaleSnelheid / geldigeTeller;
        } else {
            gemiddeldeSnelheid = 0.0;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fout bij verzamelen statistieken: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Onbekende fout bij verzamelen statistieken" << std::endl;
    }
}

// =============================================================================
// ALLE ORIGINELE GETTER METHODES
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
// SETTER METHODES
// =============================================================================

void simulatie::setAutoGenereerVoertuigen(bool waarde) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij setAutoGenereerVoertuigen");
    autoGenereerVoertuigen = waarde;
    ENSURE(autoGenereerVoertuigen == waarde, "autoGenereerVoertuigen werd niet correct ingesteld");
}

void simulatie::verhoogVerwijderdeVoertuigenTeller() {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verhoogVerwijderdeVoertuigenTeller");

    try {
        verwijderdeVoertuigenTeller++;
        totaalVerwijderdeVoertuigen++;
    } catch (const std::exception& e) {
        std::cerr << "Fout bij verhogen verwijderde voertuigen teller: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Onbekende fout bij verhogen verwijderde voertuigen teller" << std::endl;
    }
}

bool simulatie::moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij moetNaarKruispunt");

    try {
        if (!voertuig.properlyInitialized() || !kruispunt.properlyInitialized()) {
            return false;
        }

        std::string voertuigBaan = voertuig.getBaanNaam();

        // Controleer of het voertuig op een baan van het kruispunt rijdt
        if (!kruispunt.bevatBaan(voertuigBaan)) {
            return false;
        }

        // Haal de kruispuntpositie op voor deze baan
        double kruispuntPositie = kruispunt.getPositieOpBaan(voertuigBaan);
        if (kruispuntPositie < 0) {
            return false; // Baan niet gevonden in kruispunt
        }

        // Controleer of het voertuig het kruispunt nadert (binnen 5 meter)
        double voertuigPositie = voertuig.getPositie();
        double afstandTotKruispunt = std::abs(voertuigPositie - kruispuntPositie);

        return afstandTotKruispunt <= 5.0;

    } catch (const std::exception& e) {
        std::cerr << "Fout bij controleren kruispunt nadering: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Onbekende fout bij controleren kruispunt nadering" << std::endl;
        return false;
    }
}

bool simulatie::verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt) {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij verplaatsVoertuigNaKruispunt");

    try {
        if (!voertuig.properlyInitialized() || !kruispunt.properlyInitialized()) {
            return false;
        }

        std::string huidigeWeg = voertuig.getBaanNaam();

        // Controleer of het voertuig daadwerkelijk bij een kruispunt is
        if (!moetNaarKruispunt(voertuig, kruispunt)) {
            return false;
        }

        // Krijg alle banen van het kruispunt
        auto banenInKruispunt = kruispunt.getBanen();
        if (banenInKruispunt.size() <= 1) {
            return false; // Geen alternatieve banen beschikbaar
        }

        // Filter banen om de huidige baan uit te sluiten
        std::vector<std::pair<std::string, double>> alternatieveBanen;
        for (const auto& baan : banenInKruispunt) {
            if (baan.first != huidigeWeg) {
                // Controleer of de baan nog bestaat in de verkeerssituatie
                const auto& alleBanen = verkeerssituatie.getBanen();
                if (alleBanen.find(baan.first) != alleBanen.end()) {
                    alternatieveBanen.push_back(baan);
                }
            }
        }

        if (alternatieveBanen.empty()) {
            return false; // Geen geldige alternatieve banen
        }

        // Kies willekeurig een nieuwe baan
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, alternatieveBanen.size() - 1);
        int index = dis(gen);

        const auto& nieuweBaan = alternatieveBanen[index];

        // Bewaar voertuig eigenschappen
        double oudeSnelheid = voertuig.getSnelheid();
        double oudeVersnelling = voertuig.getVersnelling();

        // Verplaats voertuig naar nieuwe baan
        voertuig.setBaanNaam(nieuweBaan.first);

        // Positioneer voertuig net na het kruispunt op de nieuwe baan
        double nieuwePositie = nieuweBaan.second + 2.0; // 2 meter na kruispunt
        voertuig.setPositie(nieuwePositie);

        // Behoud snelheid en versnelling
        voertuig.setSnelheid(oudeSnelheid);
        voertuig.setVersnelling(oudeVersnelling);

        return true;

    } catch (const std::exception& e) {
        std::cerr << "Fout bij verplaatsen voertuig na kruispunt: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Onbekende fout bij verplaatsen voertuig na kruispunt" << std::endl;
        return false;
    }
}

bool simulatie::moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const {
    REQUIRE(properlyInitialized(), "simulatie is niet correct geïnitialiseerd bij moetStoppenBijHalte");

    try {
        if (!voertuig.properlyInitialized() || !halte.properlyInitialized()) {
            return false;
        }

        // Alleen bussen stoppen bij bushaltes
        if (!voertuig.isBus()) {
            return false;
        }

        // Controleer of voertuig op dezelfde baan is als de bushalte
        if (voertuig.getBaanNaam() != halte.getBaan()) {
            return false;
        }

        // Controleer of er al een bus bij de halte staat
        if (halte.isBusGestopt()) {
            return false; // Halte is bezet
        }

        // Controleer of de bus al aan het wachten is bij een halte
        if (voertuig.isWaitingAtStop()) {
            return false; // Bus staat al bij een halte
        }

        // Bereken afstand tot bushalte
        double voertuigPositie = voertuig.getPositie();
        double haltePositie = halte.getPositie();
        double afstand = haltePositie - voertuigPositie;

        // Bus moet de halte naderen (binnen 10 meter en nog niet voorbij)
        if (afstand >= 0 && afstand <= 10.0) {
            return true;
        }

        return false;

    } catch (const std::exception& e) {
        std::cerr << "Fout bij controleren bushalte stop: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Onbekende fout bij controleren bushalte stop" << std::endl;
        return false;
    }
}