/**
 * @file situatie.h
 * @brief Header for the VerkeersSituatie class (Revised with bus stops and intersections)
 */

#ifndef SITUATIE_H
#define SITUATIE_H

#include <vector>
#include <map>
#include <string>
#include "baan.h"
#include "voertuig.h"
#include "verkeerslicht.h"
#include "voertuiggenerator.h"
#include "bushalte.h"
#include "kruispunt.h"

/**
 * @brief Class that represents a traffic situation
 */
class VerkeersSituatie {
public:
    /**
     * @brief Constructor
     */
    VerkeersSituatie();

    /**
     * @brief Add a road to the traffic situation
     * @param baan The road to add
     * @return true if the road was added successfully, false otherwise
     */
    bool voegBaanToe(const Baan& baan);

    /**
     * @brief Add a vehicle to the traffic situation
     * @param voertuig The vehicle to add
     * @return true if the vehicle was added successfully, false otherwise
     */
    bool voegVoertuigToe(const Voertuig& voertuig);

    /**
     * @brief Add a traffic light to the traffic situation
     * @param licht The traffic light to add
     * @return true if the traffic light was added successfully, false otherwise
     */
    bool voegVerkeerslichtToe(const Verkeerslicht& licht);

    /**
     * @brief Add a vehicle generator to the traffic situation
     * @param generator The vehicle generator to add
     * @return true if the generator was added successfully, false otherwise
     */
    bool voegVoertuigGeneratorToe(const VoertuigGenerator& generator);

    /**
     * @brief Add a bus stop to the traffic situation
     * @param bushalte The bus stop to add
     * @return true if the bus stop was added successfully, false otherwise
     */
    bool voegBushalteToe(const Bushalte& bushalte);

    /**
     * @brief Add an intersection to the traffic situation
     * @param kruispunt The intersection to add
     * @return true if the intersection was added successfully, false otherwise
     */
    bool voegKruispuntToe(const Kruispunt& kruispunt);

    /**
     * @brief Verify the consistency of the traffic situation
     * @return true if the traffic situation is consistent, false otherwise
     */
    bool verificeerConsistentie() const;

    /**
     * @brief Remove a vehicle from the simulation
     * @param index Index of the vehicle to remove
     * @return true if the vehicle was removed successfully, false otherwise
     */
    bool verwijderVoertuig(int index);

    /**
     * @brief Get a map of all roads
     * @return Map with road names as keys and road objects as values
     */
    const std::map<std::string, Baan>& getBanen() const;

    /**
     * @brief Get mutable reference to all vehicles
     * @return Vector with all vehicles
     */
    std::vector<Voertuig>& getVoertuigen();

    /**
     * @brief Get constant reference to all vehicles
     * @return Vector with all vehicles
     */
    const std::vector<Voertuig>& getVoertuigen() const;

    /**
     * @brief Get all traffic lights
     * @return Vector with all traffic lights
     */
    const std::vector<Verkeerslicht>& getVerkeerslichten() const;

    /**
     * @brief Get mutable reference to all traffic lights
     * @return Vector with all traffic lights
     */
    std::vector<Verkeerslicht>& getVerkeerslichten();

    /**
     * @brief Get all vehicle generators
     * @return Vector with all vehicle generators
     */
    const std::vector<VoertuigGenerator>& getVoertuigGenerators() const;

    /**
     * @brief Get all bus stops
     * @return Vector with all bus stops
     */
    const std::vector<Bushalte>& getBushaltes() const;

    /**
     * @brief Get mutable reference to all bus stops
     * @return Vector with all bus stops
     */
    std::vector<Bushalte>& getBushaltes();

    /**
     * @brief Get all intersections
     * @return Vector with all intersections
     */
    const std::vector<Kruispunt>& getKruispunten() const;

    /**
     * @brief Find bus stops on a specific road
     * @param baanNaam Name of the road
     * @return Vector with pointers to bus stops on the road
     */
    std::vector<Bushalte*> zoekBushaltesOpBaan(const std::string& baanNaam);

    /**
     * @brief Find the next bus stop for a vehicle
     * @param voertuig The vehicle to find the next bus stop for
     * @return Pointer to the next bus stop, nullptr if none
     */
    Bushalte* zoekEerstvolgendeHalte(const Voertuig& voertuig);

    /**
     * @brief Find the next traffic light for a vehicle
     * @param voertuig The vehicle to find the next traffic light for
     * @return Pointer to the next traffic light, nullptr if none
     */
    Verkeerslicht* zoekEerstvolgendeVerkeerslicht(const Voertuig& voertuig);

    /**
     * @brief Find intersections that involve a specific road
     * @param baanNaam Name of the road
     * @return Vector with pointers to intersections involving the road
     */
    std::vector<Kruispunt*> zoekKruispuntenOpBaan(const std::string& baanNaam);

    /**
     * @brief Find the next intersection for a vehicle
     * @param voertuig The vehicle to find the next intersection for
     * @return Pointer to the next intersection, nullptr if none
     */
    Kruispunt* zoekEerstvolgendeKruispunt(const Voertuig& voertuig);

private:
    std::map<std::string, Baan> banen;        ///< The roads in the traffic situation, indexed by name
    std::vector<Voertuig> voertuigen;         ///< The vehicles in the traffic situation
    std::vector<Verkeerslicht> verkeerslichten; ///< The traffic lights in the traffic situation
    std::vector<VoertuigGenerator> generators;  ///< The vehicle generators in the traffic situation
    std::vector<Bushalte> bushaltes;          ///< The bus stops in the traffic situation
    std::vector<Kruispunt> kruispunten;       ///< The intersections in the traffic situation
};

#endif // SITUATIE_H