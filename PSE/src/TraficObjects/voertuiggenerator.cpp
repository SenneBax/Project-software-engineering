/**
* @file voertuiggenerator.cpp
 * @brief Implementatie van de VoertuigGenerator klasse
 */

#include "voertuiggenerator.h"
#include "DesignByContract.h"

/**
 * @brief Constructor
 * @param baan De naam van de baan waar voertuigen gegenereerd worden
 * @param frequentie De frequentie waarmee voertuigen gegenereerd worden
 * @param type Het type voertuigen dat gegenereerd wordt
 * REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
 * REQUIRE(frequentie > 0, "frequentie moet positief zijn.");
 * ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
VoertuigGenerator::VoertuigGenerator(const std::string& baan, int frequentie, const std::string& type)
    : baanNaam(baan), frequentie(frequentie), type(type) {
    REQUIRE(!baanNaam.empty(), "baanNaam mag niet leeg zijn.");
    REQUIRE(frequentie > 0, "frequentie moet positief zijn.");

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");

}


bool VoertuigGenerator::properlyInitialized() const {
    return _initCheck == this;
}
/**
 * @brief Geeft de naam van de baan terug
 * @return De naam van de baan
 * REQUIRE(properlyInitialized(), "getBaanNaam moet eindigen in een geldige toestand.");
 */
std::string VoertuigGenerator::getBaanNaam() const {
    REQUIRE(properlyInitialized(), "getBaanNaam moet eindigen in een geldige toestand.");
    return baanNaam;
}

/**
 * @brief Geeft de frequentie terug
 * @return De frequentie
 * REQUIRE(properlyInitialized(), "getFrequentie moet eindigen in een geldige toestand.");
 */
int VoertuigGenerator::getFrequentie() const {
    REQUIRE(properlyInitialized(), "getFrequentie moet eindigen in een geldige toestand.");
    return frequentie;
}

/**
 * @brief Geeft het type voertuigen terug
 * @return Het type voertuigen
 * REQUIRE(properlyInitialized(), "getType moet eindigen in een geldige toestand.");
 */
std::string VoertuigGenerator::getType() const {
    REQUIRE(properlyInitialized(), "getType moet eindigen in een geldige toestand.");
    return type;
}