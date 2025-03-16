/**
* @file Baan.cpp
 * @author senne
 * @date 27/02/2025
 */

#include "Baan.h"
#include <iostream>

/**
 * @brief Implementatie van de default constructor
 */
Baan::Baan()
    : naam(""), lengte(0) {}

/**
 * @brief Constructor met parameters
 * @param naam Naam van de baan
 * @param lengte Lengte van de baan in meters
 */
Baan::Baan(const std::string& naam, const int lengte)
    : naam(naam), lengte(lengte) {

}

/**
 * @brief Getter voor de naam van de baan
 * @return De naam van de baan
 */
std::string Baan::getNaam() const {
    return naam;
}

/**
 * @brief Getter voor de lengte van de baan
 * @return De lengte van de baan in meters
 */
int Baan::getLengte() const {
    return lengte;
}