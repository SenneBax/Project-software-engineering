/**
 * @file kruispunt.cpp
 * @brief Implementation of the intersection class
 */

#include "kruispunt.h"
#include <algorithm>
#include <random>

/**
 * @brief Constructor for the intersection
 */
Kruispunt::Kruispunt() {}

/**
 * @brief Add a road to the intersection
 * @param baanNaam Name of the road
 * @param positie Position on the road
 * @return True if the road was added successfully, false otherwise
 */
bool Kruispunt::voegBaanToe(const std::string& baanNaam, double positie) {
    // Check if the road is already part of this intersection
    if (bevatBaan(baanNaam)) {
        return false;
    }

    banen.emplace_back(baanNaam, positie);
    return true;
}

/**
 * @brief Get all the roads connected to this intersection
 * @return Vector of pairs with road names and positions
 */
std::vector<std::pair<std::string, double>> Kruispunt::getBanen() const {
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
    return std::any_of(banen.begin(), banen.end(),
                       [&baanNaam](const BaanPositie& baan) {
                           return baan.naam == baanNaam;
                       });
}

/**
 * @brief Get position on a specific road
 * @param baanNaam Name of the road
 * @return Position on the road, -1 if road is not found
 */
double Kruispunt::getPositieOpBaan(const std::string& baanNaam) const {
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