/**
 * @file simulatie.cpp
 * @brief Implementation of the simulation class (Revised for better modularity)
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

/**
 * @brief Constructor
 * @param situatie Reference to the traffic situation to simulate
 * @param tijdstap The time step for the simulation in seconds
 */
simulatie::simulatie(VerkeersSituatie& situatie, double tijdstap)
    : verkeerssituatie(situatie), tijdstap(tijdstap), huidigeSimulatieTijd(0.0) {
    // Check if the time step is valid
    if (tijdstap <= 0) {
        this->tijdstap = 0.0166; // Default time step
    }

    // Initialize default values from B.6
    vertraagAfstand = 50.0;
    stopAfstand = 15.0;
    vertraagFactor = 0.4;
}

/**
 * @brief Perform one simulation step
 */
void simulatie::stap() {
    // Process all traffic lights
    verwerkVerkeerslichten();

    // Process all vehicles
    verwerkVoertuigen();

    // Process all bus stops
    verwerkBushaltes();

    // Process all intersections
    verwerkKruispunten();

    // Update the simulation time
    huidigeSimulatieTijd += tijdstap;

    // Add new vehicles periodically if the option is enabled
    if (autoGenereerVoertuigen) {
        genereerNieuweVoertuigen();
    }

    // Collect statistics
    verzamelStatistieken();
}

/**
 * @brief Process all traffic lights in the simulation
 */
void simulatie::verwerkVerkeerslichten() {
    // Update all traffic lights
    for (auto& verkeerslicht : verkeerssituatie.getVerkeerslichten()) {
        verkeerslicht.update(tijdstap);
    }
}

/**
 * @brief Check if a vehicle is approaching a traffic light
 * @param voertuig The vehicle to check
 * @param verkeerslicht The traffic light to check
 * @return Response code: 0 = no effect, 1 = slow down, 2 = stop, 3 = pass through
 */
int simulatie::controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht) {
    // If this is a priority vehicle (fire truck, ambulance, police), it can pass through red lights
    if (voertuig.isPrioriteitsvoertuig()) {
        return 3; // Pass through
    }

    double afstand = verkeerslicht.getPositie() - voertuig.getPositie();

    // If the vehicle is very close to the light or has passed it, no effect
    if (afstand <= 0.1) {
        return 0;
    }

    // Check traffic light state
    if (verkeerslicht.isRood()) {
        if (afstand <= stopAfstand) {
            return 2; // Stop
        } else if (afstand <= vertraagAfstand) {
            return 1; // Slow down
        }
    } else if (verkeerslicht.isOranje()) {
        // For orange light, if in stop distance, keep going. If in slow-down distance, start slowing down
        if (afstand <= stopAfstand) {
            return 0; // Continue
        } else if (afstand <= vertraagAfstand) {
            return 1; // Slow down
        }
    }
    // For green light, no effect
    return 0;
}

/**
 * @brief Process all vehicles in the simulation
 */
void simulatie::verwerkVoertuigen() {
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    const std::map<std::string, Baan>& banen = verkeerssituatie.getBanen();

    // Temporary vector to store vehicles that need to be removed
    std::vector<int> teVerwijderenVoertuigen;

    // Group vehicles by road
    std::map<std::string, std::vector<size_t>> voertuigenPerBaan;

    // Index vehicles by road
    for (size_t i = 0; i < voertuigen.size(); i++) {
        voertuigenPerBaan[voertuigen[i].getBaanNaam()].push_back(i);
    }

    // For each road, sort vehicles by position (descending)
    for (auto& paar : voertuigenPerBaan) {
        std::sort(paar.second.begin(), paar.second.end(), [&voertuigen](const size_t a, const size_t b) {
            return voertuigen[a].getPositie() > voertuigen[b].getPositie();
        });
    }

    // Process each vehicle
    for (auto& baanPaar : voertuigenPerBaan) {
        const std::string& baanNaam = baanPaar.first;
        const std::vector<size_t>& voertuigIndices = baanPaar.second;

        // Check if the road exists
        auto baanIt = banen.find(baanNaam);
        if (baanIt == banen.end()) {
            continue; // Skip this road if it doesn't exist
        }

        const Baan& baan = baanIt->second;

        // Process vehicles from back to front (highest position first)
        for (size_t i = 0; i < voertuigIndices.size(); i++) {
            const size_t voertuigIndex = voertuigIndices[i];
            Voertuig& voertuig = voertuigen[voertuigIndex];

            // Skip vehicles waiting at bus stops
            if (voertuig.isWaitingAtBusStop()) {
                continue;
            }

            // Determine if this is the first vehicle on the road
            const bool isEersteVoertuig = (i == 0);

            // Determine the preceding vehicle (if this is not the first vehicle)
            Voertuig* voorgaandVoertuig = nullptr;
            if (!isEersteVoertuig) {
                voorgaandVoertuig = &voertuigen[voertuigIndices[i-1]];
            }

            // Check traffic lights for the first vehicle on the road only
            if (isEersteVoertuig) {
                // Find the next traffic light
                Verkeerslicht* eerstvolgendeVerkeerslicht = verkeerssituatie.zoekEerstvolgendeVerkeerslicht(voertuig);

                if (eerstvolgendeVerkeerslicht != nullptr) {
                    double afstand = eerstvolgendeVerkeerslicht->getPositie() - voertuig.getPositie();

                    // Only consider traffic lights that are within the slow-down distance
                    if (afstand <= vertraagAfstand) {
                        int actie = controleerVerkeerslichtNadering(voertuig, *eerstvolgendeVerkeerslicht);

                        if (actie == 1) {
                            // Slow down
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig, vertraagFactor,
                                                       vertraagFactor * voertuig.getMaxSnelheid());
                        } else if (actie == 2) {
                            // Stop
                            voertuig.noodStop();
                        } else if (actie == 3) {
                            // Pass through (priority vehicle)
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                        } else {
                            // Regular acceleration calculation
                            voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                        }
                    } else {
                        // No traffic light in range, regular acceleration
                        voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                    }
                } else {
                    // No traffic light ahead, regular acceleration
                    voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
                }
            } else {
                // Not the first vehicle, just follow the vehicle in front
                voertuig.berekenVersnelling(voorgaandVoertuig, isEersteVoertuig);
            }

            // Update position and speed
            voertuig.updatePositieEnSnelheid(tijdstap);

            // Check if the vehicle is at the end of the road
            if (voertuig.getPositie() >= baan.getLengte()) {
                // Check if there is an intersection at the end of the road
                for (auto& kruispunt : verkeerssituatie.getKruispunten()) {
                    if (kruispunt.bevatBaan(baanNaam) &&
                        std::abs(kruispunt.getPositieOpBaan(baanNaam) - baan.getLengte()) < 0.1) {
                        // Vehicle reached the intersection at the end of the road
                        if (verplaatsVoertuigNaKruispunt(voertuig, kruispunt)) {
                            // Vehicle has been moved to a new road, no need to remove it
                            break;
                        }
                    }
                }

                // If the vehicle is still at the end of the road, mark it for removal
                if (voertuig.getPositie() >= baan.getLengte()) {
                    teVerwijderenVoertuigen.push_back(voertuigIndex);
                }
            }
        }
    }

    // Remove vehicles that are no longer on a road (from high to low to keep indices valid)
    std::sort(teVerwijderenVoertuigen.begin(), teVerwijderenVoertuigen.end(), std::greater<int>());
    for (const int index : teVerwijderenVoertuigen) {
        verhoogVerwijderdeVoertuigenTeller();
        verkeerssituatie.verwijderVoertuig(index);
    }
}

/**
 * @brief Process all bus stops in the simulation
 */
void simulatie::verwerkBushaltes() {
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();
    std::vector<Bushalte>& bushaltes = verkeerssituatie.getBushaltes();

    // Process each vehicle
    for (auto& voertuig : voertuigen) {
        // Only process buses
        if (!voertuig.isBus()) {
            continue;
        }

        // Skip buses that are already waiting at a stop
        if (voertuig.isWaitingAtBusStop()) {
            continue;
        }

        // Find the next bus stop for this vehicle
        Bushalte* eerstvolgendeBushalte = verkeerssituatie.zoekEerstvolgendeHalte(voertuig);

        if (eerstvolgendeBushalte != nullptr) {
            // Check if the bus should stop at this bus stop
            if (moetStoppenBijHalte(voertuig, *eerstvolgendeBushalte)) {
                // Bus has arrived at the stop, mark as stopped
                voertuig.setIsWaitingAtBusStop(true);
                eerstvolgendeBushalte->setBusStopped();
            }
        }
    }

    // Process each bus stop
    for (auto& bushalte : bushaltes) {
        // Skip bus stops with no bus
        if (!bushalte.isBusGestopt()) {
            continue;
        }

        // Update waiting time
        if (bushalte.updateWachttijd(tijdstap)) {
            // Waiting time is over, find the bus
            for (auto& voertuig : voertuigen) {
                if (voertuig.isBus() && voertuig.isWaitingAtBusStop() &&
                    voertuig.getBaanNaam() == bushalte.getBaan() &&
                    std::abs(voertuig.getPositie() - bushalte.getPositie()) < 0.5) {

                    // Bus can continue
                    voertuig.setIsWaitingAtBusStop(false);
                    bushalte.setBusLeft();
                    break;
                }
            }
        }
    }
}

/**
 * @brief Process all intersections in the simulation
 */
void simulatie::verwerkKruispunten() {
    std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();

    // Process each vehicle
    for (auto& voertuig : voertuigen) {
        // Skip vehicles waiting at bus stops
        if (voertuig.isWaitingAtBusStop()) {
            continue;
        }

        // Find the next intersection for this vehicle
        Kruispunt* eerstvolgendeKruispunt = verkeerssituatie.zoekEerstvolgendeKruispunt(voertuig);

        if (eerstvolgendeKruispunt != nullptr) {
            // Check if the vehicle should move to the intersection
            if (moetNaarKruispunt(voertuig, *eerstvolgendeKruispunt)) {
                verplaatsVoertuigNaKruispunt(voertuig, *eerstvolgendeKruispunt);
            }
        }
    }
}

/**
 * @brief Check if a vehicle should stop at a bus stop
 * @param voertuig Vehicle to check
 * @param halte Bus stop to check
 * @return true if the bus should stop at the bus stop, false otherwise
 */
bool simulatie::moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const {
    // Only buses should stop at bus stops
    if (!voertuig.isBus()) {
        return false;
    }

    // Check if the bus is on the same road as the bus stop
    if (voertuig.getBaanNaam() != halte.getBaan()) {
        return false;
    }

    // Distance to the bus stop
    double afstand = halte.getPositie() - voertuig.getPositie();

    // If the bus is very close to the stop or has passed it
    if (afstand <= 0.5 && afstand >= -0.5) {
        return true;
    }

    return false;
}

/**
 * @brief Check if a vehicle can move to an intersection
 * @param voertuig Vehicle to check
 * @param kruispunt Intersection to check
 * @return true if the vehicle should be moved to an intersection, false otherwise
 */
bool simulatie::moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const {
    // Check if the vehicle is on a road that is part of the intersection
    if (!kruispunt.bevatBaan(voertuig.getBaanNaam())) {
        return false;
    }

    // Get the position of the intersection on the road
    double kruispuntPositie = kruispunt.getPositieOpBaan(voertuig.getBaanNaam());

    // Distance to the intersection
    double afstand = kruispuntPositie - voertuig.getPositie();

    // If the vehicle is very close to the intersection or has passed it
    if (afstand <= 0.5 && afstand >= -0.5) {
        return true;
    }

    return false;
}

/**
 * @brief Move a vehicle to a new road after an intersection
 * @param voertuig The vehicle to move
 * @param kruispunt The intersection where the vehicle is
 * @return true if the vehicle was moved, false otherwise
 */
bool simulatie::verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt) {
    // Choose a random road to continue on
    std::string nieuweBaan = kruispunt.kiesRandomBaan(voertuig.getBaanNaam());

    // If no valid road was found, remove the vehicle
    if (nieuweBaan.empty()) {
        return false;
    }

    // Move the vehicle to the new road at position 0
    voertuig.setBaanNaam(nieuweBaan);
    voertuig.setPositie(0);

    return true;
}

/**
 * @brief Generate new vehicles in the simulation from generators
 */
void simulatie::genereerNieuweVoertuigen() {
    const std::vector<VoertuigGenerator>& generators = verkeerssituatie.getVoertuigGenerators();

    for (const auto& generator : generators) {
        static std::map<std::string, double> laatsteGeneratieTijd;

        // If this is the first time, initialize the last generation time
        if (laatsteGeneratieTijd.find(generator.getBaanNaam()) == laatsteGeneratieTijd.end()) {
            laatsteGeneratieTijd[generator.getBaanNaam()] = 0.0;
        }

        // Check if it's time to generate a new vehicle
        if (huidigeSimulatieTijd - laatsteGeneratieTijd[generator.getBaanNaam()] >= generator.getFrequentie()) {
            // Generate a new vehicle at position 0 of the road
            if (genereertVoertuig(generator.getBaanNaam(), 0.0, generator.getType())) {
                laatsteGeneratieTijd[generator.getBaanNaam()] = huidigeSimulatieTijd;
            }
        }
    }
}

/**
 * @brief Generate a new vehicle on a road
 * @param baanNaam Road name
 * @param positie Position
 * @param type Vehicle type
 * @return true if the vehicle was generated successfully
 */
bool simulatie::genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type) {
    // Check if the road exists
    auto baanIt = verkeerssituatie.getBanen().find(baanNaam);
    if (baanIt == verkeerssituatie.getBanen().end()) {
        return false;
    }

    // Check if there is already a vehicle near the position
    for (const auto& voertuig : verkeerssituatie.getVoertuigen()) {
        if (voertuig.getBaanNaam() == baanNaam) {
            double afstand = std::abs(voertuig.getPositie() - positie);

            // If there is already a vehicle too close, don't generate a new one
            // Use 2 * vehicle length as minimum distance
            if (afstand < 2 * voertuig.getLengte()) {
                return false;
            }
        }
    }

    // Create and add the new vehicle
    Voertuig nieuwVoertuig(baanNaam, positie, type);
    return verkeerssituatie.voegVoertuigToe(nieuwVoertuig);
}

/**
 * @brief Enable or disable automatic vehicle generation
 * @param genereer Whether to automatically generate vehicles
 */
void simulatie::setAutoGenereerVoertuigen(bool genereer) {
    autoGenereerVoertuigen = genereer;
}

/**
 * @brief Collect simulation statistics
 */
void simulatie::verzamelStatistieken() {
    const std::vector<Voertuig>& voertuigen = verkeerssituatie.getVoertuigen();

    totaleTijd += tijdstap;

    // Count the number of vehicles currently in the simulation
    aantalVoertuigen = voertuigen.size();

    // Calculate average speed of all vehicles
    double totaleSnelheid = 0.0;
    for (const Voertuig& voertuig : voertuigen) {
        totaleSnelheid += voertuig.getSnelheid();
    }

    if (!voertuigen.empty()) {
        gemiddeldeSnelheid = totaleSnelheid / voertuigen.size();
    } else {
        gemiddeldeSnelheid = 0.0;
    }

    // Increment counter for removed vehicles
    totaalVerwijderdeVoertuigen += verwijderdeVoertuigenTeller;
    verwijderdeVoertuigenTeller = 0;
}

/**
 * @brief Increment the counter for removed vehicles
 */
void simulatie::verhoogVerwijderdeVoertuigenTeller() {
    verwijderdeVoertuigenTeller++;
}

// Getters for simulation properties and statistics
double simulatie::getHuidigeSimulatieTijd() const {
    return huidigeSimulatieTijd;
}

double simulatie::getTijdstap() const {
    return tijdstap;
}

int simulatie::getAantalVoertuigen() const {
    return aantalVoertuigen;
}

double simulatie::getGemiddeldeSnelheid() const {
    return gemiddeldeSnelheid;
}

int simulatie::getTotaalVerwijderdeVoertuigen() const {
    return totaalVerwijderdeVoertuigen;
}

double simulatie::getTotaleTijd() const {
    return totaleTijd;
}