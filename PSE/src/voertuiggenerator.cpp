/**
* @file voertuiggenerator.cpp
 * @brief Implementatie van de VoertuigGenerator klasse
 */

#include "voertuiggenerator.h"

/**
 * @brief Constructor
 * @param baan De naam van de baan waar voertuigen gegenereerd worden
 * @param frequentie De frequentie waarmee voertuigen gegenereerd worden
 * @param type Het type voertuigen dat gegenereerd wordt
 */
VoertuigGenerator::VoertuigGenerator(const std::string& baan, int frequentie, const std::string& type)
    : baanNaam(baan), frequentie(frequentie), type(type) {
}

/**
 * @brief Geeft de naam van de baan terug
 * @return De naam van de baan
 */
std::string VoertuigGenerator::getBaanNaam() const {
    return baanNaam;
}

/**
 * @brief Geeft de frequentie terug
 * @return De frequentie
 */
int VoertuigGenerator::getFrequentie() const {
    return frequentie;
}

/**
 * @brief Geeft het type voertuigen terug
 * @return Het type voertuigen
 */
std::string VoertuigGenerator::getType() const {
    return type;
}