//
// Created by senne on 22/03/2025.
//

/**
 * @file voertuiggenerator.h
 * @brief Header voor de VoertuigGenerator klasse
 */

#ifndef VOERTUIGGENERATOR_H
#define VOERTUIGGENERATOR_H

#include <string>

/**
 * @brief Klasse die voertuigen genereert op een baan
 */
class VoertuigGenerator {
public:
    /**
     * @brief Constructor
     * @param baan De naam van de baan waar voertuigen gegenereerd worden
     * @param frequentie De frequentie waarmee voertuigen gegenereerd worden
     * @param type Het type voertuigen dat gegenereerd wordt
     */
    VoertuigGenerator(const std::string& baan, int frequentie, const std::string& type = "auto");

    /**
     * @brief Geeft de naam van de baan terug
     * @return De naam van de baan
     */
    std::string getBaanNaam() const;

    /**
     * @brief Geeft de frequentie terug
     * @return De frequentie
     */
    int getFrequentie() const;

    /**
     * @brief Geeft het type voertuigen terug
     * @return Het type voertuigen
     */
    std::string getType() const;

    // Andere methoden...

private:
    std::string baanNaam;  ///< Naam van de baan waar voertuigen gegenereerd worden
    int frequentie;        ///< Frequentie waarmee voertuigen gegenereerd worden
    std::string type;      ///< Type voertuigen dat gegenereerd wordt
};

#endif // VOERTUIGGENERATOR_H