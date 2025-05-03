/**
 * @file kruispunt.cpp
 * @brief Implementation of the intersection class
 */

#include "kruispunt.h"
#include <algorithm>
#include <random>
#include <cassert>

#include "DesignByContract.h"

/**
 * @brief Constructor for the intersection
 */
Kruispunt::Kruispunt()
{
    _initCheck = this;
    ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");

}

/**
 * @brief Add a road to the intersection
 * @param baanNaam Name of the road
 * @param positie Position on the road
 * @return True if the road was added successfully, false otherwise
 */
bool Kruispunt::voegBaanToe(const std::string& baanNaam, double positie) {
    REQUIRE(!baanNaam.empty(), "baanNaam is leeg");
    REQUIRE(positie >= 0.0, "positie moet positive zijn.");
    ENSURE(properlyInitialized(),"Constructor moet eindigen in een geldige toestand.");

    // Check if the road is already part of this intersection
    if (bevatBaan(baanNaam)) {
        return false;
    }

    banen.emplace_back(baanNaam, positie);
    return true;
}


bool Kruispunt::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Get all the roads connected to this intersection
 * @return Vector of pairs with road names and positions
 */
std::vector<std::pair<std::string, double>> Kruispunt::getBanen() const {
    //REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij getBanen");

    std::vector<std::pair<std::string, double>> result;

    for (const auto& baan : banen) {
        result.emplace_back(baan.naam, baan.positie);
    }

    return result;
}

/**
 * @brief Check if a road is part of this intersection
 * @param baanNaam Name of the road
 * @return True if the road is part of this intersection, false otherwise
 */
bool Kruispunt::bevatBaan(const std::string& baanNaam) const {
    //REQUIRE(properlyInitialized(), "Kruispunt niet correct geïnitialiseerd bij bevatBaan");
    REQUIRE(!baanNaam.empty(), "baanNaam is empty");
    return std::any_of(banen.begin(), banen.end(),
                       [&baanNaam](const BaanPositie& baan) {
                           return baan.naam == baanNaam;
                       });

    //ENSURE(properlyInitialized(), "Constructor moet eindigen in geen geldige staat.");
}

/**
 * @brief Get position on a specific road
 * @param baanNaam Name of the road
 * @return Position on the road, -1 if road is not found
 */
double Kruispunt::getPositieOpBaan(const std::string& baanNaam) const {
    REQUIRE(!baanNaam.empty(), "baanNaam is leeg");
    for (const auto& baan : banen) {
        if (baan.naam == baanNaam) {
            return baan.positie;
        }
    }

    return -1.0; // Road not found
}

/**
 * @brief Choose a random road to continue on from the intersection
 * @param huidigeWeg Current road name (to exclude it from possibilities)
 * @return Name of the chosen road, empty string if no valid road exists
 */
std::string Kruispunt::kiesRandomBaan(const std::string& huidigeWeg) const {
    REQUIRE(!huidigeWeg.empty(), "huidigeWeg is leeg");
    // Collect potential roads (excluding the current one)
    std::vector<std::string> mogelijkeBanen;

    for (const auto& baan : banen) {
        // Skip the current road and roads that end at this intersection (position equals road length)
        if (baan.naam != huidigeWeg) {
            mogelijkeBanen.push_back(baan.naam);
        }
    }

    // If there are no valid roads to continue on, return empty string
    if (mogelijkeBanen.empty()) {
        return "";
    }

    // Choose a random road
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, mogelijkeBanen.size() - 1);
    return mogelijkeBanen[distr(gen)];
}