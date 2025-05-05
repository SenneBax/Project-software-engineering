/**
 * @file simulatie.cpp
 * @brief Implementatie van de simulatieklasse (Herzien voor betere modulariteit)
 */

#include "../Simulation/simulatie.h"
#include "../Situation/situatie.h"
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../TraficObjects/voertuiggenerator.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include "DesignByContract.h"


/**
 * @brief Constructor
 * @param situatie Referentie naar de te simuleren verkeerssituatie
 * @param tijdstap De tijdstap voor de simulatie in seconden
 */
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie), tijdstap(tijdstap), huidigeSimulatieTijd(0.0) {
    //REQUIRE(tijdstap >= 0.0, "tijdstap mag niet negatief zijn.");
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld.");
    // Controleer of de tijdstap geldig is
    if (tijdstap <= 0) {
        this->tijdstap = 0.0166; // Standaard tijdstap
    }

    // Initialiseer standaardwaarden uit B.6
    vertraagAfstand = 50.0;
    stopAfstand = 15.0;
    vertraagFactor = 0.4;

    _initCheck = this;
    REQUIRE(properlyInitialized(), "simulatie moet eindigen in een geldige toestand.");
}

bool simulatie::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Voer één simulatiestap uit
 */
void simulatie::stap() {
    REQUIRE(properlyInitialized(), "stap moet eindigen in een geldige toestand.");

    // Verwerk alle verkeerslichten
    verwerkVerkeerslichten();

    // Verwerk alle voertuigen
    verwerkVoertuigen();

    // Verwerk alle bushaltes
    verwerkBushaltes();

    // Verwerk alle kruispunten
    verwerkKruispunten();

    // Update de simulatietijd
    huidigeSimulatieTijd += tijdstap;

    // Voeg periodiek nieuwe voertuigen toe als de optie is ingeschakeld
    if (autoGenereerVoertuigen) {
        genereerNieuweVoertuigen();
    }

    // Verzamel statistieken
    verzamelStatistieken();

    REQUIRE(properlyInitialized(), "stap moet eindigen in een geldige toestand.");
}

/**
 * @brief Verwerk alle verkeerslichten in de simulatie
 */
void simulatie::verwerkVerkeerslichten() {
    REQUIRE(properlyInitialized(), "verwerkVerkeerslichten moet eindigen in een geldige toestand.");

    // Update alle verkeerslichten
    for (auto& verkeerslicht : verkeerssituatie.getVerkeerslichten()) {
        verkeerslicht.update(tijdstap);
    }
    REQUIRE(properlyInitialized(), "verwerkVerkeerslichten moet eindigen in een geldige toestand.");

}

/**
 * @brief Controleer of een voertuig een verkeerslicht nadert
 * @param voertuig Het te controleren voertuig
 * @param verkeerslicht Het te controleren verkeerslicht
 * @return Responscode: 0 = geen effect, 1 = vertragen, 2 = stoppen, 3 = doorrijden
 */
int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    REQUIRE(properlyInitialized(), "controleerVerkeerslichtNadering moet eindigen in een geldige toestand.");
    REQUIRE(voertuig.properlyInitialized(), "voertuig moet correct ingesteld zijn.");
    //REQUIRE(verkeerslicht.properlyInitialized(), "verkeerslicht moet correct ingesteld zijn.");
    // Als dit een prioriteitsvoertuig is (brandweerwagen, ziekenwagen, politie), kan het door rode lichten rijden
    if (voertuig.isPrioriteitsvoertuig()) {
        return 3; // Doorrijden
    }

    double afstand = verkeerslicht.getPositie() - voertuig.getPositie();

    // Als het voertuig heel dicht bij het licht is of het is voorbij, geen effect
    if (afstand <= 0.1) {
        return 0;
    }

    // Controleer verkeerslicht status
    if (verkeerslicht.isRood()) {
        if (afstand <= stopAfstand) {
            return 2; // Stoppen
        } else if (afstand <= vertraagAfstand) {
            return 1; // Vertragen
        }
    } else if (verkeerslicht.isOranje()) {
        // Voor oranje licht, als in stopafstand, doorrijden. Als in vertraagafstand, beginnen vertragen
        if (afstand <= stopAfstand) {
            return 0; // Doorrijden
        } else if (afstand <= vertraagAfstand) {
            return 1; // Vertragen
        }
    }
    // Voor groen licht, geen effect
    return 0;
}

/**
 * @brief Verwerk alle voertuigen in de simulatie
 */
void simulatie::verwerkVoertuigen() {
    REQUIRE(properlyInitialized(), "verwerkVoertuigen moet eindigen in een geldige toestand.");
    REQUIRE(verkeerssituatie.properlyInitialized(), "voertuig moet correct ingesteld zijn.");
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

    // Tijdelijke vector om voertuigen op te slaan die verwijderd moeten worden
    std::vector<int> teVerwijderenVoertuigen;

    // Groepeer voertuigen per baan
    std::map<std::string, std::vector<size_t>> voertuigenPerBaan;

    // Indexeer voertuigen per baan
    for (size_t i = 0; i < voertuigen.size(); i++) {
        voertuigenPerBaan[voertuigen[i].getBaanNaam()].push_back(i);
    }

    // Voor elke baan, sorteer voertuigen op positie (aflopend)
    for (auto& paar : voertuigenPerBaan) {
        std::sort(paar.second.begin(), paar.second.end(), [&voertuigen](const size_t a, const size_t b) {
            return voertuigen[a].getPositie() > voertuigen[b].getPositie();
        });
    }

    // Verwerk elk voertuig
    for (auto& baanPaar : voertuigenPerBaan) {
        const std::string& baanNaam = baanPaar.first;
        const std::vector<size_t>& voertuigIndices = baanPaar.second;

        // Controleer of de baan bestaat
        auto baanIt = banen.find(baanNaam);
        if (baanIt == banen.end()) {
            continue; // Sla deze baan over als die niet bestaat
        }

        const Baan& baan = baanIt->second;

        // Verwerk voertuigen van achter naar voor (hoogste positie eerst)
        for (size_t i = 0; i < voertuigIndices.size(); i++) {
            const size_t voertuigIndex = voertuigIndices[i];
            Voertuig& voertuig = voertuigen[voertuigIndex];

            // Sla voertuigen over die bij bushaltes wachten
            if (voertuig.isWaitingAtBusStop()) {
                continue;
            }

            // Bepaal of dit het eerste voertuig op de weg is
            const bool isEersteVoertuig = (i == 0);

            // Bepaal het voorliggende voertuig (als dit niet het eerste voertuig is)
            Voertuig* voorgaandVoertuig = nullptr;
            if (!isEersteVoertuig) {
                voorgaandVoertuig = &voertuigen[voertuigIndices[i-1]];
            }

            // Controleer verkeerslichten alleen voor het eerste voertuig op de weg
            if (isEersteVoertuig) {
                // Zoek het volgende verkeerslicht
                Verkeerslicht* eerstvolgendeVerkeerslicht = verkeerssituatie.zoekEerstvolgendeVerkeerslicht(voertuig);

                if (eerstvolgendeVerkeerslicht != nullptr) {
                    double afstand = eerstvolgendeVerkeerslicht->getPositie() - voertuig.getPositie();

                    // Hou alleen rekening met verkeerslichten binnen de vertraagafstand
                    if (afstand <= vertraagAfstand) {
                        int actie = controleerVerkeerslichtNadering(voertuig, *eerstvolgendeVerkeerslicht);

                        if (actie == 1) {
                            // Vertragen
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig, vertraagFactor,
                                                       vertraagFactor * voertuig.getMaxSnelheid());
                        } else if (actie == 2) {
                            // Stoppen
                            voertuig.noodStop();
                        } else if (actie == 3) {
                            // Doorrijden (prioriteitsvoertuig)
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                        } else {
                            // Reguliere versnellingsberekening
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                        }
                    } else {
                        // Geen verkeerslicht in bereik, reguliere versnelling
                        voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                    }
                } else {
                    // Geen verkeerslicht vooruit, reguliere versnelling
                    voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                }
            } else {
                // Niet het eerste voertuig, volg gewoon het voertuig ervoor
                voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
            }

            // Update positie en snelheid
            voertuig.updatePositieEnSnelheid(tijdstap);

            // Controleer of het voertuig aan het einde van de weg is
            if (voertuig.getPositie() >= baan.getLengte()) {
                // Controleer of er een kruispunt aan het einde van de weg is
                for (auto& kruispunt : verkeerssituatie.getKruispunten()) {
                    if (kruispunt.bevatBaan(baanNaam) &&
                        std::abs(kruispunt.getPositieOpBaan(baanNaam) - baan.getLengte()) < 0.1) {
                        // Voertuig heeft het kruispunt aan het einde van de weg bereikt
                        if (verplaatsVoertuigNaKruispunt(voertuig, kruispunt)) {
                            // Voertuig is naar een nieuwe weg verplaatst, hoeft niet verwijderd te worden
                            break;
                        }
                    }
                }

                // Als het voertuig nog steeds aan het einde van de weg is, markeer het voor verwijdering
                if (voertuig.getPositie() >= baan.getLengte()) {
                    teVerwijderenVoertuigen.push_back(voertuigIndex);
                }
            }
        }
    }

    // Verwijder voertuigen die niet meer op een weg zijn (van hoog naar laag om indices geldig te houden)
    std::sort(teVerwijderenVoertuigen.begin(), teVerwijderenVoertuigen.end(), std::greater<int>());
    for (const int index : teVerwijderenVoertuigen) {
        verhoogVerwijderdeVoertuigenTeller();
        verkeerssituatie.verwijderVoertuig(index);
    }
}

/**
 * @brief Verwerk alle bushaltes in de simulatie
 */
void simulatie::verwerkBushaltes() {
    REQUIRE(properlyInitialized(), "verwerkVoertuigen moet eindigen in een geldige toestand.");
    REQUIRE(verkeerssituatie.properlyInitialized(), "voertuig moet correct ingesteld zijn.");
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    std::vector<Bushalte>& bushaltes = verkeerssituatie.getBushaltes();

    // Verwerk elk voertuig
    for (auto& voertuig : voertuigen) {
        // Verwerk alleen bussen
        if (!voertuig.isBus()) {
            continue;
        }

        // Sla bussen over die al bij een halte wachten
        if (voertuig.isWaitingAtBusStop()) {
            continue;
        }

        // Zoek de volgende bushalte voor dit voertuig
        Bushalte* eerstvolgendeBushalte = verkeerssituatie.zoekEerstvolgendeHalte(voertuig);

        if (eerstvolgendeBushalte != nullptr) {
            // Controleer of de bus bij deze bushalte moet stoppen
            if (moetStoppenBijHalte(voertuig, *eerstvolgendeBushalte)) {
                // Bus is aangekomen bij de halte, markeer als gestopt
                voertuig.setIsWaitingAtBusStop(true);
                eerstvolgendeBushalte->setBusStopped();
            }
        }
    }

    // Verwerk elke bushalte
    for (auto& bushalte : bushaltes) {
        // Sla bushaltes zonder bus over
        if (!bushalte.isBusGestopt()) {
            continue;
        }

        // Update wachttijd
        if (bushalte.updateWachttijd(tijdstap)) {
            // Wachttijd is voorbij, zoek de bus
            for (auto& voertuig : voertuigen) {
                if (voertuig.isBus() && voertuig.isWaitingAtBusStop() &&
                    voertuig.getBaanNaam() == bushalte.getBaan() &&
                    std::abs(voertuig.getPositie() - bushalte.getPositie()) < 0.5) {

                    // Bus kan verder
                    voertuig.setIsWaitingAtBusStop(false);
                    bushalte.setBusLeft();
                    break;
                }
            }
        }
    }
}

/**
 * @brief Verwerk alle kruispunten in de simulatie
 */
void simulatie::verwerkKruispunten() {
    REQUIRE(properlyInitialized(), "verwerkVoertuigen moet eindigen in een geldige toestand.");
    REQUIRE(verkeerssituatie.properlyInitialized(), "voertuig moet correct ingesteld zijn.");
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();

    // Verwerk elk voertuig
    for (auto& voertuig : voertuigen) {
        // Sla voertuigen over die bij bushaltes wachten
        if (voertuig.isWaitingAtBusStop()) {
            continue;
        }

        // Zoek het volgende kruispunt voor dit voertuig
        Kruispunt* eerstvolgendeKruispunt = verkeerssituatie.zoekEerstvolgendeKruispunt(voertuig);

        if (eerstvolgendeKruispunt != nullptr) {
            // Controleer of het voertuig naar het kruispunt moet gaan
            if (moetNaarKruispunt(voertuig, *eerstvolgendeKruispunt)) {
                verplaatsVoertuigNaKruispunt(voertuig, *eerstvolgendeKruispunt);
            }
        }
    }
}

/**
 * @brief Controleer of een voertuig bij een bushalte moet stoppen
 * @param voertuig Te controleren voertuig
 * @param halte Te controleren bushalte
 * @return true als de bus bij de bushalte moet stoppen, false indien niet
 */
bool simulatie::moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const {
    REQUIRE(properlyInitialized(), "moetStoppenBijHalte moet eindigen in een geldige toestand.");

    // Alleen bussen moeten stoppen bij bushaltes
    if (!voertuig.isBus()) {
        return false;
    }

    // Controleer of de bus op dezelfde baan als de bushalte is
    if (voertuig.getBaanNaam() != halte.getBaan()) {
        return false;
    }

    // Afstand tot de bushalte
    double afstand = halte.getPositie() - voertuig.getPositie();

    // Als de bus erg dicht bij de halte is of is voorbijgereden
    if (afstand <= 0.5 && afstand >= -0.5) {
        return true;
    }

    return false;
}

/**
 * @brief Controleer of een voertuig naar een kruispunt kan gaan
 * @param voertuig Te controleren voertuig
 * @param kruispunt Te controleren kruispunt
 * @return true als het voertuig naar een kruispunt moet worden verplaatst, false indien niet
 */
bool simulatie::moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const {
    REQUIRE(properlyInitialized(), "moetNaarKruispunt moet eindigen in een geldige toestand.");

    // Controleer of het voertuig op een baan is die deel uitmaakt van het kruispunt
    if (!kruispunt.bevatBaan(voertuig.getBaanNaam())) {
        return false;
    }

    // Haal de positie van het kruispunt op de baan op
    double kruispuntPositie = kruispunt.getPositieOpBaan(voertuig.getBaanNaam());

    // Afstand tot het kruispunt
    double afstand = kruispuntPositie - voertuig.getPositie();

    // Als het voertuig erg dicht bij het kruispunt is of het voorbij is
    if (afstand <= 0.5 && afstand >= -0.5) {
        return true;
    }

    return false;
}

/**
 * @brief Verplaats een voertuig naar een nieuwe weg na een kruispunt
 * @param voertuig Het te verplaatsen voertuig
 * @param kruispunt Het kruispunt waar het voertuig zich bevindt
 * @return true als het voertuig werd verplaatst, false indien niet
 */
bool simulatie::verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt) {
    REQUIRE(properlyInitialized(), "verplaatsVoertuigNaKruispunt moet eindigen in een geldige toestand.");
    REQUIRE(voertuig.properlyInitialized(), "voertuig moet eindigen in een geldige toestand.");

    // Kies een willekeurige weg om verder te gaan
    std::string nieuweBaan = kruispunt.kiesRandomBaan(voertuig.getBaanNaam());

    // Als er geen geldige weg is gevonden, verwijder het voertuig
    if (nieuweBaan.empty()) {
        return false;
    }

    // Verplaats het voertuig naar de nieuwe weg op positie 0
    voertuig.setBaanNaam(nieuweBaan);
    voertuig.setPositie(0);

    ENSURE(!voertuig.getBaan().empty(), "voertuig moet na verplaatsing een geldige baan hebben.");
    ENSURE(voertuig.getPositie() == 0, "voertuig moet na verplaatsing op positie 0 starten.");
    return true;

}

/**
 * @brief Genereer nieuwe voertuigen in de simulatie vanaf generatoren
 */
void simulatie::genereerNieuweVoertuigen() {
    REQUIRE(properlyInitialized(), "genereerNieuweVoertuigen moet eindigen in een geldige toestand.");
    const std::vector<VoertuigGenerator>& generators = verkeerssituatie.getVoertuigGenerators();

    for (const auto& generator : generators) {
        static std::map<std::string, double> laatsteGeneratieTijd;

        // Als dit de eerste keer is, initialiseer de laatste generatietijd
        if (laatsteGeneratieTijd.find(generator.getBaanNaam()) == laatsteGeneratieTijd.end()) {
            laatsteGeneratieTijd[generator.getBaanNaam()] = 0.0;
        }

        // Controleer of het tijd is om een nieuw voertuig te genereren
        if (huidigeSimulatieTijd - laatsteGeneratieTijd[generator.getBaanNaam()] >= generator.getFrequentie()) {
            // Genereer een nieuw voertuig op positie 0 van de weg
            if (genereertVoertuig(generator.getBaanNaam(), 0.0, generator.getType())) {
                laatsteGeneratieTijd[generator.getBaanNaam()] = huidigeSimulatieTijd;
                ENSURE(laatsteGeneratieTijd[generator.getBaanNaam()] == huidigeSimulatieTijd, "Laatste generatie tijd moet gelijk zijn met huidige stimulatie tijd.");
            }
        }
    }
}

/**
 * @brief Genereer een nieuw voertuig op een weg
 * @param baanNaam Wegnaam
 * @param positie Positie
 * @param type Voertuigtype
 * @return true als het voertuig succesvol werd gegenereerd
 */
bool simulatie::genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type) {
    REQUIRE(properlyInitialized(), "Simutaltie moet eindigen in een geldige toestand.");
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "positie moet positief zijn.");
    REQUIRE(!type.empty(), "Voertuigtype mag niet leeg zijn.");
    // Controleer of de weg bestaat
    auto baanIt = verkeerssituatie.getBanen().find(baanNaam);
    if (baanIt == verkeerssituatie.getBanen().end()) {
        return false;
    }

    // Controleer of er al een voertuig in de buurt van de positie is
    for (const auto& voertuig : verkeerssituatie.getVoertuigen()) {
        if (voertuig.getBaanNaam() == baanNaam) {
            double afstand = std::abs(voertuig.getPositie() - positie);

            // Als er al een voertuig te dicht in de buurt is, genereer geen nieuw voertuig
            // Gebruik 2 * voertuiglengte als minimale afstand
            if (afstand < 2 * voertuig.getLengte()) {
                return false;
            }
        }
    }

    // Maak en voeg het nieuwe voertuig toe
    Voertuig nieuwVoertuig(baanNaam, positie, type);
    return verkeerssituatie.voegVoertuigToe(nieuwVoertuig);
}

/**
 * @brief Schakel automatische voertuiggeneratie in of uit
 * @param genereer Of voertuigen automatisch moeten worden gegenereerd
 */
void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    autoGenereerVoertuigen = genereer;
}

/**
 * @brief Verzamel simulatiestatistieken
 */
void simulatie::verzamelStatistieken() {
    REQUIRE(properlyInitialized(), "verzamelStatistieken moet eindigen in een geldige toestand.");

    const std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();

    totaleTijd += tijdstap;

    // Tel het aantal voertuigen momenteel in de simulatie
    aantalVoertuigen = voertuigen.size();

    // Bereken gemiddelde snelheid van alle voertuigen
    double totaleSnelheid = 0.0;
    for (const Voertuig& voertuig : voertuigen) {
        totaleSnelheid += voertuig.getSnelheid();
    }

    if (!voertuigen.empty()) {
        gemiddeldeSnelheid = totaleSnelheid / voertuigen.size();
    } else {
        gemiddeldeSnelheid = 0.0;
    }

    // Verhoog teller voor verwijderde voertuigen
    totaalVerwijderdeVoertuigen += verwijderdeVoertuigenTeller;
    verwijderdeVoertuigenTeller = 0;

    ENSURE(gemiddeldeSnelheid >= 0.0, "gemiddelde snelheid moet positief zijn.");
}

/**
 * @brief Verhoog de teller voor verwijderde voertuigen
 */
void simulatie::verhoogVerwijderdeVoertuigenTeller() {
    REQUIRE(properlyInitialized(), "verhoogVerwijderdeVoertuigenTeller moet eindigen in een geldige toestand.");
    verwijderdeVoertuigenTeller++;
}

// Getters voor simulatie-eigenschappen en statistieken
double simulatie::getHuidigeSimulatieTijd() const {
    REQUIRE(properlyInitialized(), "getHuidigeSimulatieTijd moet eindigen in een geldige toestand.");
    return huidigeSimulatieTijd;
}

double simulatie::getTijdstap() const {
    REQUIRE(properlyInitialized(), "getTijdstap moet eindigen in een geldige toestand.");
    return tijdstap;
}

int simulatie::getAantalVoertuigen() const {
    REQUIRE(properlyInitialized(), "getAantalVoertuigen moet eindigen in een geldige toestand.");
    return aantalVoertuigen;
}

double simulatie::getGemiddeldeSnelheid() const {
    REQUIRE(properlyInitialized(), "getGemiddeldeSnelheid moet eindigen in een geldige toestand.");
    return gemiddeldeSnelheid;
}

int simulatie::getTotaalVerwijderdeVoertuigen() const {
    REQUIRE(properlyInitialized(), "getTotaalVerwijderdeVoertuigen moet eindigen in een geldige toestand.");
    return totaalVerwijderdeVoertuigen;
}

double simulatie::getTotaleTijd() const {
    REQUIRE(properlyInitialized(), "getTotaleTijd moet eindigen in een geldige toestand.");
    return totaleTijd;
}