/**
* @file Baan.cpp
 * @brief de manier hoe de klasse van de baan is geïmplementeerd
 */

#include "baan.h"
#include <iostream>
#include "DesignByContract.h"
/**
 * @brief Implementatie van de default constructor
 * ENSURE(properlyInitialized(), "Default moet eindigen in een geldige toestand");
 */
Baan::Baan()
    : naam(""), lengte(0)
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "Default moet eindigen in een geldige toestand");
}

/**
 * @brief Constructor met parameters
 * @param naam Naam van de baan
 * @param lengte Lengte van de baan in meters
 * @pre REQUIRE(!naam.empty(), "Naam van de baan mag niet leeg zijn");
 * @pre REQUIRE(lengte>0, "Baan lengte moet groter dan 0 zijn.");
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
Baan::Baan(const std::string& naam, const int lengte)
    : naam(naam), lengte(lengte) {
    _initCheck = this;
    REQUIRE(!naam.empty(), "Naam van de baan mag niet leeg zijn");
    REQUIRE(lengte>0, "Baan lengte moet groter dan 0 zijn.");
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");

}


bool Baan::properlyInitialized() const
{
    return _initCheck == this;
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