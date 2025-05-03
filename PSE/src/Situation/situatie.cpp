/**
 * @file situatie.cpp
 * @brief Implementation of the VerkeersSituatie class (Revised with bus stops and intersections)
 */

#include "../Situation/situatie.h"
#include <algorithm>
#include <limits>
#include "DesignByContract.h"

/**
 * @brief Constructor
 */
VerkeersSituatie::VerkeersSituatie()
{
    _initCheck = this;
}

bool VerkeersSituatie::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Add a road to the traffic situation
 * @param baan The road to add
 * @return true if the road was added successfully, false otherwise
 */
bool VerkeersSituatie::voegBaanToe(const Baan& baan) {
    REQUIRE(properlyInitialized(), "voegBaanToe moet eindigen in een geldige toestand.");
    REQUIRE(!baan.getNaam().empty(), "Naam van toe te voegen baan mag niet leeg zijn.");

    std::string baanNaam = baan.getNaam();

    // Check if the road already exists
    if (banen.find(baanNaam) != banen.end()) {
        return false;
    }

    banen[baanNaam] = baan;

    ENSURE(banen.find(baanNaam) != banen.end(), "Baan werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Add a vehicle to the traffic situation
 * @param voertuig The vehicle to add
 * @return true if the vehicle was added successfully, false otherwise
 */
bool VerkeersSituatie::voegVoertuigToe(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "voegVoertuigToe moet eindigen in een geldige toestand.");
    std::string baanNaam = voertuig.getBaanNaam();

    // Check if the road exists
    auto it = banen.find(baanNaam);

    if (it == banen.end()) {
        return false;
    }

    // Check if the position is valid
    if (voertuig.getPositie() < 0 || voertuig.getPositie() > it->second.getLengte()) {
        return false;
    }

    voertuigen.push_back(voertuig);
    ENSURE(!voertuigen.empty(), "Voertuig werd niet correct toegevoed." );
    return true;
}

/**
 * @brief Add a traffic light to the traffic situation
 * @param licht The traffic light to add
 * @return true if the traffic light was added successfully, false otherwise
 */
bool VerkeersSituatie::voegVerkeerslichtToe(const Verkeerslicht& licht) {
    REQUIRE(properlyInitialized(), "voegVoertuigToe moet eindigen in een geldige toestand.");
    REQUIRE(licht.properlyInitialized(), "Verkeerslicht is niet correct ingesteld.");
    std::string baanNaam = licht.getBaan();

    // Check if the road exists
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    // Check if the position is valid
    if (licht.getPositie() < 0 || licht.getPositie() > it->second.getLengte()) {
        return false;
    }

    // Check if the cycle time is valid
    if (licht.getCyclus() <= 0) {
        return false;
    }

    // Check if there are no other traffic lights in the slow-down distance
    for (const auto& existingLight : verkeerslichten) {
        if (existingLight.getBaan() == baanNaam) {
            // Traffic lights should not be in each other's slow-down distance (50m according to spec)
            const double distance = std::abs(existingLight.getPositie() - licht.getPositie());
            if (distance < 50) {
                return false;
            }
        }
    }

    verkeerslichten.push_back(licht);
    ENSURE(!verkeerslichten.empty(), "Verkeerslicht werdt niet correct toegevoed aan situatie.");
    return true;
}

/**
 * @brief Add a vehicle generator to the traffic situation
 * @param generator The vehicle generator to add
 * @return true if the generator was added successfully, false otherwise
 */
bool VerkeersSituatie::voegVoertuigGeneratorToe(const VoertuigGenerator& generator) {
    REQUIRE(properlyInitialized(), "voegVoertuigGeneratorToe moet eindigen in een geldige toestand.");
    REQUIRE(generator.properlyInitialized(), "Generator is niet correct ingesteld");
    std::string baanNaam = generator.getBaanNaam();

    // Check if the road exists
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    generators.push_back(generator);
    ENSURE(!generators.empty(), "Generator werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Add a bus stop to the traffic situation
 * @param bushalte The bus stop to add
 * @return true if the bus stop was added successfully, false otherwise
 */
bool VerkeersSituatie::voegBushalteToe(const Bushalte& bushalte) {
    REQUIRE(properlyInitialized(), "voegBushalteToe moet eindigen in een geldige toestand.");
    REQUIRE(bushalte.getWachttijd(), "Bushalte is niet correct ingesteld");

    std::string baanNaam = bushalte.getBaan();

    // Check if the road exists
    auto it = banen.find(baanNaam);
    if (it == banen.end()) {
        return false;
    }

    // Check if the position is valid
    if (bushalte.getPositie() < 0 || bushalte.getPositie() > it->second.getLengte()) {
        return false;
    }

    // Check if the waiting time is valid
    if (bushalte.getWachttijd() <= 0) {
        return false;
    }

    bushaltes.push_back(bushalte);
    ENSURE(!bushaltes.empty(), "Bushalte werd niet correct toegevoed.");
    return true;
}

/**
 * @brief Add an intersection to the traffic situation
 * @param kruispunt The intersection to add
 * @return true if the intersection was added successfully, false otherwise
 */
bool VerkeersSituatie::voegKruispuntToe(const Kruispunt& kruispunt) {
    REQUIRE(properlyInitialized(), "voegKruispuntToe moet eindigen in een geldige toestand.");

    // Get all roads in the intersection
    auto baanParen = kruispunt.getBanen();

    // Check if all roads exist and positions are valid
    for (const auto& paar : baanParen) {
        const std::string& baanNaam = paar.first;
        double positie = paar.second;

        // Check if the road exists
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        // Check if the position is valid
        if (positie < 0 || positie > it->second.getLengte()) {
            return false;
        }
    }

    // All roads are valid, add the intersection
    kruispunten.push_back(kruispunt);
    ENSURE(!kruispunten.empty(), "Kruispunt werd niet correct toegevoegd.");
    return true;
}

/**
 * @brief Verify the consistency of the traffic situation
 * @return true if the traffic situation is consistent, false otherwise
 */
bool VerkeersSituatie::verificeerConsistentie() const {
    REQUIRE(properlyInitialized(), "verificeerConsistentie moet eindigen in een geldige toestand.");

    // Check if there is at least one road
    if (banen.empty()) {
        return false;
    }

    // Check if all vehicles are on valid roads with valid positions
    for (const auto& voertuig : voertuigen) {
        std::string baanNaam = voertuig.getBaanNaam();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (voertuig.getPositie() < 0 || voertuig.getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Check if all traffic lights are on valid roads with valid positions
    for (const auto& licht : verkeerslichten) {
        std::string baanNaam = licht.getBaan();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (licht.getPositie() < 0 || licht.getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Check if all generators are on valid roads
    for (const auto& generator : generators) {
        std::string baanNaam = generator.getBaanNaam();
        if (banen.find(baanNaam) == banen.end()) {
            return false;
        }
    }

    // Check if all bus stops are on valid roads with valid positions
    for (const auto& bushalte : bushaltes) {
        std::string baanNaam = bushalte.getBaan();
        auto it = banen.find(baanNaam);
        if (it == banen.end()) {
            return false;
        }

        if (bushalte.getPositie() < 0 || bushalte.getPositie() > it->second.getLengte()) {
            return false;
        }
    }

    // Check if all intersections have valid roads and positions
    for (const auto& kruispunt : kruispunten) {
        auto baanParen = kruispunt.getBanen();
        for (const auto& paar : baanParen) {
            const std::string& baanNaam = paar.first;
            double positie = paar.second;

            auto it = banen.find(baanNaam);
            if (it == banen.end()) {
                return false;
            }

            if (positie < 0 || positie > it->second.getLengte()) {
                return false;
            }
        }
    }
    ENSURE(true, "Constistentie is goed beëindigd.");
    return true;
}

/**
 * @brief Remove a vehicle from the simulation
 * @param index Index of the vehicle to remove
 * @return true if the vehicle was removed successfully, false otherwise
 */
bool VerkeersSituatie::verwijderVoertuig(int index) {
    REQUIRE(properlyInitialized(), "verwijderVoertuig moet eindigen in een geldige toestand.");
    if (index < 0 || index >= static_cast<int>(voertuigen.size())) {
        return false;
    }

    voertuigen.erase(voertuigen.begin() + index);
    ENSURE(static_cast<int>(voertuigen.size()) >= 0, "Aantal Voertuig out of range.");
    return true;
}

/**
 * @brief Get a map of all roads
 * @return Map with road names as keys and road objects as values
 */
const std::map<std::string, Baan>& VerkeersSituatie::getBanen() const {
    REQUIRE(properlyInitialized(), "getBanen moet eindigen in een geldige toestand.");
    return banen;
}

/**
 * @brief Get mutable reference to all vehicles
 * @return Vector with all vehicles
 */
std::vector<Voertuig>& VerkeersSituatie::getVoertuigen() {
    REQUIRE(properlyInitialized(), "getVoertuigen moet eindigen in een geldige toestand.");

    return voertuigen;
}

/**
 * @brief Get constant reference to all vehicles
 * @return Vector with all vehicles
 */
const std::vector<Voertuig>& VerkeersSituatie::getVoertuigen() const {
    REQUIRE(properlyInitialized(), "getVoertuigen moet eindigen in een geldige toestand.");

    return voertuigen;
}

/**
 * @brief Get all traffic lights
 * @return Vector with all traffic lights
 */
const std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() const {
    REQUIRE(properlyInitialized(), "getVerkeerslichten moet eindigen in een geldige toestand.");

    return verkeerslichten;
}

/**
 * @brief Get mutable reference to all traffic lights
 * @return Vector with all traffic lights
 */
std::vector<Verkeerslicht>& VerkeersSituatie::getVerkeerslichten() {
    REQUIRE(properlyInitialized(), "getVerkeerslichten moet eindigen in een geldige toestand.");

    return verkeerslichten;
}

/**
 * @brief Get all vehicle generators
 * @return Vector with all vehicle generators
 */
const std::vector<VoertuigGenerator>& VerkeersSituatie::getVoertuigGenerators() const {
    REQUIRE(properlyInitialized(), "getVoertuigGenerators moet eindigen in een geldige toestand.");
    return generators;
}

/**
 * @brief Get all bus stops
 * @return Vector with all bus stops
 */
const std::vector<Bushalte>& VerkeersSituatie::getBushaltes() const {
    REQUIRE(properlyInitialized(), "getBushaltes moet eindigen in een geldige toestand.");
    return bushaltes;
}

/**
 * @brief Get mutable reference to all bus stops
 * @return Vector with all bus stops
 */
std::vector<Bushalte>& VerkeersSituatie::getBushaltes() {
    REQUIRE(properlyInitialized(), "getBushaltes moet eindigen in een geldige toestand.");
    return bushaltes;
}

/**
 * @brief Get all intersections
 * @return Vector with all intersections
 */
const std::vector<Kruispunt>& VerkeersSituatie::getKruispunten() const {
    REQUIRE(properlyInitialized(), "getKruispunten moet eindigen in een geldige toestand.");
    return kruispunten;
}

/**
 * @brief Find bus stops on a specific road
 * @param baanNaam Name of the road
 * @return Vector with pointers to bus stops on the road
 */
std::vector<Bushalte*> VerkeersSituatie::zoekBushaltesOpBaan(const std::string& baanNaam) {
    REQUIRE(properlyInitialized(), "zoekBushaltesOpBaan moet eindigen in een geldige toestand.");
    REQUIRE(!baanNaam.empty(), "Baannaam mag niet leeg zijn.");
    std::vector<Bushalte*> result;

    for (auto& bushalte : bushaltes) {
        if (bushalte.getBaan() == baanNaam) {
            result.push_back(&bushalte);
        }
    }

    // Sort by position
    std::sort(result.begin(), result.end(), [](const Bushalte* a, const Bushalte* b) {
        return a->getPositie() < b->getPositie();
    });

    return result;
}

/**
 * @brief Find the next bus stop for a vehicle
 * @param voertuig The vehicle to find the next bus stop for
 * @return Pointer to the next bus stop, nullptr if none
 */
Bushalte* VerkeersSituatie::zoekEerstvolgendeHalte(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeHalte moet eindigen in een geldige toestand.");
    Bushalte* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    // Only search for bus stops if the vehicle is a bus
    if (!voertuig.isBus()) {
        return nullptr;
    }

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& bushalte : bushaltes) {
        if (bushalte.getBaan() == baanNaam && bushalte.getPositie() > voertuigPositie) {
            double afstand = bushalte.getPositie() - voertuigPositie;

            if (afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
                eerstvolgend = &bushalte;
            }
        }
    }

    return eerstvolgend;
}

/**
 * @brief Find the next traffic light for a vehicle
 * @param voertuig The vehicle to find the next traffic light for
 * @return Pointer to the next traffic light, nullptr if none
 */
Verkeerslicht* VerkeersSituatie::zoekEerstvolgendeVerkeerslicht(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeVerkeerslicht moet eindigen in een geldige toestand.");
    Verkeerslicht* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& verkeerslicht : verkeerslichten) {
        if (verkeerslicht.getBaan() == baanNaam && verkeerslicht.getPositie() > voertuigPositie) {
            double afstand = verkeerslicht.getPositie() - voertuigPositie;

            if (afstand < kortsteAfstand) {
                kortsteAfstand = afstand;
                eerstvolgend = &verkeerslicht;
            }
        }
    }

    return eerstvolgend;
}

/**
 * @brief Find intersections that involve a specific road
 * @param baanNaam Name of the road
 * @return Vector with pointers to intersections involving the road
 */
std::vector<Kruispunt*> VerkeersSituatie::zoekKruispuntenOpBaan(const std::string& baanNaam) {
    REQUIRE(properlyInitialized(), "zoekKruispuntenOpBaan moet eindigen in een geldige toestand.");
    REQUIRE(!baanNaam.empty(), "BaanNaam mag niet leeg zijn.");
    std::vector<Kruispunt*> result;

    for (auto& kruispunt : kruispunten) {
        if (kruispunt.bevatBaan(baanNaam)) {
            result.push_back(&kruispunt);
        }
    }

    return result;
}

/**
 * @brief Find the next intersection for a vehicle
 * @param voertuig The vehicle to find the next intersection for
 * @return Pointer to the next intersection, nullptr if none
 */
Kruispunt* VerkeersSituatie::zoekEerstvolgendeKruispunt(const Voertuig& voertuig) {
    REQUIRE(properlyInitialized(), "zoekEerstvolgendeKruispunt moet eindigen in een geldige toestand.");
    Kruispunt* eerstvolgend = nullptr;
    double kortsteAfstand = std::numeric_limits<double>::max();

    std::string baanNaam = voertuig.getBaanNaam();
    double voertuigPositie = voertuig.getPositie();

    for (auto& kruispunt : kruispunten) {
        if (kruispunt.bevatBaan(baanNaam)) {
            double kruispuntPositie = kruispunt.getPositieOpBaan(baanNaam);

            if (kruispuntPositie > voertuigPositie) {
                double afstand = kruispuntPositie - voertuigPositie;

                if (afstand < kortsteAfstand) {
                    kortsteAfstand = afstand;
                    eerstvolgend = &kruispunt;
                }
            }
        }
    }

    return eerstvolgend;
}