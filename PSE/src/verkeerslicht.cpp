/**
* @file Verkeerslicht.cpp
 * @author senne
 * @date 27/02/2025
 * @brief de implementatie van de verkeerslichten
 */

#include "Verkeerslicht.h"
#include <iostream>

/**
 * @brief Constructor voor het verkeerslicht
 * @param baan Naam van de baan
 * @param positie Positie op de baan in meters
 * @param cyclus Cyclustijd in seconden
 */
Verkeerslicht::Verkeerslicht(const std::string& baan, const int positie, const int cyclus)
    : baan(baan), positie(positie), cyclus(cyclus) {}

/**
 * @brief Getter voor de baan van het verkeerslicht
 * @return De naam van de baan
 */
std::string Verkeerslicht::getBaan() const {
    return baan;
}

/**
 * @brief Getter voor de positie van het verkeerslicht
 * @return De positie in meters
 */
int Verkeerslicht::getPositie() const {
    return positie;
}

/**
 * @brief Getter voor de cyclustijd van het verkeerslicht
 * @return De cyclustijd in seconden
 */
int Verkeerslicht::getCyclus() const {
    return cyclus;
}