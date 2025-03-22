/**
 * @file voertuig.cpp
 * @author senne
 * @date 19/03/2025
 * @brief Implementatie van de Voertuig klasse
 */

#include "voertuig.h"
#include <algorithm>
#include <cmath>

/**
 * @brief Constructor
 * @param baan De naam van de baan waar het voertuig zich bevindt
 * @param positie De positie van het voertuig op de baan
 * @param type Het type van het voertuig
 */
Voertuig::Voertuig(const std::string& baan, double positie, const std::string& type)
    : baanNaam(baan), positie(positie), type(type) {
}

/**
 * @brief Copy constructor
 * @param other Het te kopiëren voertuig
 */
Voertuig::Voertuig(const Voertuig& other)
    : baanNaam(other.baanNaam), positie(other.positie), type(other.type) {
}

/**
 * @brief Destructor
 */
Voertuig::~Voertuig() {
    // Geen dynamisch geheugen om vrij te geven
}

/**
 * @brief Assignment operator
 * @param other Het voertuig waarvan de waarden worden overgenomen
 * @return Referentie naar dit voertuig
 */
Voertuig& Voertuig::operator=(const Voertuig& other) {
    if (this != &other) {
        baanNaam = other.baanNaam;
        positie = other.positie;
        type = other.type;
    }
    return *this;
}

/**
 * @brief Geeft de naam van de baan terug
 * @return De naam van de baan
 */
std::string Voertuig::getBaanNaam() const {
    return baanNaam;
}

/**
 * @brief Geeft de positie van het voertuig terug
 * @return De positie op de baan
 */
double Voertuig::getPositie() const {
    return positie;
}

/**
 * @brief Zet de positie van het voertuig
 * @param nieuwePositie De nieuwe positie
 */
void Voertuig::setPositie(double nieuwePositie) {
    positie = nieuwePositie;
}

/**
 * @brief Geeft het type van het voertuig terug
 * @return Het type van het voertuig
 */
std::string Voertuig::getType() const {
    return type;
}

/**
 * @brief Beweegt het voertuig vooruit
 * @param tijdstap De tijdstap in seconden
 * @param maxSnelheid De maximale snelheid in meter per seconde
 * @param maxPositie De maximale positie op de baan
 * @return De nieuwe positie
 */
double Voertuig::beweeg(double tijdstap, double maxSnelheid, double maxPositie) {
    // Bereken de aangepaste snelheid gebaseerd op het type voertuig
    double snelheid = berekenSnelheid(maxSnelheid);

    // Bereken de nieuwe positie
    positie += snelheid * tijdstap;

    // Begrens de positie
    positie = std::min(positie, maxPositie);

    return positie;
}

/**
 * @brief Controleert of dit voertuig botst met een ander voertuig
 * @param ander Het andere voertuig
 * @param veiligheidsAfstand De minimale veiligheidsafstand tussen voertuigen
 * @return true als er een botsing is, anders false
 */
bool Voertuig::botst(const Voertuig& ander, double veiligheidsAfstand) const {
    // Alleen voertuigen op dezelfde baan kunnen botsen
    if (baanNaam != ander.baanNaam) {
        return false;
    }

    // Bereken de afstand tussen de voertuigen
    double afstand = std::abs(positie - ander.positie);

    // Er is een botsing als de afstand kleiner is dan de veiligheidsafstand
    return afstand < veiligheidsAfstand;
}

/**
 * @brief Berekent de snelheid van het voertuig gebaseerd op het type
 * @param basisSnelheid De basissnelheid voor normale voertuigen
 * @return De aangepaste snelheid gebaseerd op het type voertuig
 */
double Voertuig::berekenSnelheid(double basisSnelheid) const {
    // Verschillende types hebben verschillende snelheidsaanpassingen
    if (type == "auto") {
        return basisSnelheid;
    } else if (type == "bus") {
        return basisSnelheid * 0.8; // Bussen zijn langzamer
    } else if (type == "brandweerwagen") {
        return basisSnelheid * 1.2; // Brandweerwagens zijn sneller in noodgevallen
    } else if (type == "ziekenwagen") {
        return basisSnelheid * 1.3; // Ziekenwagens zijn nog sneller in noodgevallen
    } else if (type == "politiecombi") {
        return basisSnelheid * 1.25; // Politiecombi's zijn ook sneller
    } else {
        // Voor onbekende types, gebruik de standaard snelheid
        return basisSnelheid;
    }
}