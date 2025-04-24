/**
 * @file simulatie.h
 * @brief Definition of the simulation class (Revised for better modularity)
 */

#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "situatie.h"

/**
 * @class simulatie
 * @brief Class for simulating a traffic situation
 */
class simulatie {
private:
    VerkeersSituatie& verkeerssituatie;
    double tijdstap;
    double huidigeSimulatieTijd;
    bool autoGenereerVoertuigen = false;

    // Default values from section B.6
    double vertraagAfstand;      /**< Δxs - slow-down distance */
    double stopAfstand;          /**< Δxs0 - stop distance */
    double vertraagFactor;       /**< s - slow-down factor */

    // Statistics
    int aantalVoertuigen = 0;
    double gemiddeldeSnelheid = 0.0;
    int verwijderdeVoertuigenTeller = 0;
    int totaalVerwijderdeVoertuigen = 0;
    double totaleTijd = 0.0;

    /**
     * @brief Process all traffic lights in the simulation
     */
    void verwerkVerkeerslichten();

    /**
     * @brief Check if a vehicle is approaching a traffic light
     * @param voertuig The vehicle to check
     * @param verkeerslicht The traffic light to check
     * @return Response code: 0 = no effect, 1 = slow down, 2 = stop, 3 = pass through
     */
    int controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht);

    /**
     * @brief Process all vehicles in the simulation
     */
    void verwerkVoertuigen();

    /**
     * @brief Process all bus stops in the simulation
     */
    void verwerkBushaltes();

    /**
     * @brief Process all intersections in the simulation
     */
    void verwerkKruispunten();

    /**
     * @brief Generate new vehicles in the simulation from generators
     */
    void genereerNieuweVoertuigen();

    /**
     * @brief Generate a new vehicle on a road
     * @param baanNaam Road name
     * @param positie Position
     * @param type Vehicle type
     * @return true if the vehicle was generated successfully
     */
    bool genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type = "auto");

    /**
     * @brief Increment the counter for removed vehicles
     */
    void verhoogVerwijderdeVoertuigenTeller();

    /**
     * @brief Collect simulation statistics
     */
    void verzamelStatistieken();

    /**
     * @brief Check if a vehicle can move to an intersection
     * @param voertuig Vehicle to check
     * @param kruispunt Intersection to check
     * @return true if the vehicle should be moved to an intersection, false otherwise
     */
    bool moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const;

    /**
     * @brief Move a vehicle to a new road after an intersection
     * @param voertuig The vehicle to move
     * @param kruispunt The intersection where the vehicle is
     * @return true if the vehicle was moved, false otherwise
     */
    bool verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt);

    /**
     * @brief Check if a vehicle should stop at a bus stop
     * @param voertuig Vehicle to check
     * @param halte Bus stop to check
     * @return true if the bus should stop at the bus stop, false otherwise
     */
    bool moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const;

public:
    /**
     * @brief Constructor
     * @param situatie Reference to the traffic situation to simulate
     * @param tijdstap The time step for the simulation in seconds
     */
    simulatie(VerkeersSituatie& situatie, double tijdstap);

    /**
     * @brief Perform one simulation step
     */
    void stap();

    /**
     * @brief Enable or disable automatic vehicle generation
     * @param genereer Whether to automatically generate vehicles
     */
    void setAutoGenereerVoertuigen(bool genereer);

    /**
     * @brief Get the current simulation time
     * @return The current simulation time
     */
    [[nodiscard]] double getHuidigeSimulatieTijd() const;

    /**
     * @brief Get the time step
     * @return The time step
     */
    [[nodiscard]] double getTijdstap() const;

    /**
     * @brief Get the number of vehicles
     * @return The number of vehicles
     */
    [[nodiscard]] int getAantalVoertuigen() const;

    /**
     * @brief Get the average speed
     * @return The average speed
     */
    [[nodiscard]] double getGemiddeldeSnelheid() const;

    /**
     * @brief Get the total number of removed vehicles
     * @return The total number of removed vehicles
     */
    [[nodiscard]] int getTotaalVerwijderdeVoertuigen() const;

    /**
     * @brief Get the total time
     * @return The total time
     */
    [[nodiscard]] double getTotaleTijd() const;
};

#endif //SIMULATIE_H