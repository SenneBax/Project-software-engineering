/**
* @file Verkeerslicht.h
 * @author senne
 * @date 27/02/2025
 * @brief definitie van de verkeerslichtklasse
 */

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H

#include <string>

/**
 * @class Verkeerslicht
 * @brief Klasse die een verkeerslicht representeert in de verkeerssimulatie
 */
class Verkeerslicht {
private:
    std::string baan;  /**< Naam van de baan waarop het verkeerslicht staat */
    int positie;       /**< Positie van het verkeerslicht op de baan in meters */
    int cyclus;        /**< Cyclustijd van het verkeerslicht in seconden */

public:
    /**
     * @brief Constructor voor het verkeerslicht
     * @param baan Naam van de baan
     * @param positie Positie op de baan in meters
     * @param cyclus Cyclustijd in seconden
     */
    Verkeerslicht(const std::string& baan, int positie, int cyclus);

    /**
     * @brief Getter voor de baan van het verkeerslicht
     * @return De naam van de baan
     */
    std::string getBaan() const;

    /**
     * @brief Getter voor de positie van het verkeerslicht
     * @return De positie in meters
     */
    int getPositie() const;

    /**
     * @brief Getter voor de cyclustijd van het verkeerslicht
     * @return De cyclustijd in seconden
     */
    int getCyclus() const;
};

#endif // VERKEERSLICHT_H