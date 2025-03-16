/**
* @file Baan.h
 * @author senne
 * @date 27/02/2025
 */

#ifndef BAAN_H
#define BAAN_H

#include <string>

using namespace std;

/**
 * @class Baan
 * @brief Klasse voor een baan in de verkeerssimulatie
 */
class Baan {
private:
    string naam;  /**< Naam van de baan */
    int lengte;   /**< Lengte van de baan in meters */

public:
    /**
     * @brief Default constructor toevoegen
     */
    Baan();

    /**
     * @brief Bestaande constructor behouden
     * @param naam Naam van de baan
     * @param lengte Lengte van de baan in meters
     */
    Baan(const string& naam, int lengte);

    /**
     * @brief Getter voor de naam van de baan
     * @return De naam van de baan
     */
    [[nodiscard]] string getNaam() const;

    /**
     * @brief Getter voor de lengte van de baan
     * @return De lengte van de baan in meters
     */
    [[nodiscard]] int getLengte() const;
};

#endif // BAAN_H