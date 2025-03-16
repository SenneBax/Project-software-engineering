/**
 * @file Voertuig.cpp
 * @author senne
 * @date 27/02/2025
 */

#include "Voertuig.h"
#include <iostream>
using namespace std;

/**
 * @brief Constructor voor een voertuig
 * @param baan Naam van de baan
 * @param positie Positie op de baan in meters
 * @param snelheid Snelheid van het voertuig in m/s
 * @param versnelling Versnelling van het voertuig in m/s²
 */
Voertuig::Voertuig(const string& baan, const int positie, const double snelheid, const double versnelling)
    : baan(baan), positie(positie), snelheid(snelheid), versnelling(versnelling) {}

/**
 * @brief Getter voor de baan van het voertuig
 * @return De naam van de baan
 */
string Voertuig::getBaan() const {
    return baan;
}

/**
 * @brief Getter voor de positie van het voertuig
 * @return De positie in meters
 */
int Voertuig::getPositie() const {
    return positie;
}

/**
 * @brief Getter voor de snelheid van het voertuig
 * @return De snelheid in m/s
 */
double Voertuig::getSnelheid() const {
    return snelheid;
}

/**
 * @brief Getter voor de versnelling van het voertuig
 * @return De versnelling in m/s²
 */
double Voertuig::getVersnelling() const {
    return versnelling;
}

/**
 * @brief Setter voor de positie van het voertuig
 * @param nieuwePositie De nieuwe positie in meters
 */
void Voertuig::setPositie(const int nieuwePositie) {
    positie = nieuwePositie;
}

/**
 * @brief Setter voor de snelheid van het voertuig
 * @param nieuweSnelheid De nieuwe snelheid in m/s
 */
void Voertuig::setSnelheid(const double nieuweSnelheid) {
    snelheid = nieuweSnelheid;
}

/**
 * @brief Setter voor de versnelling van het voertuig
 * @param nieuweVersnelling De nieuwe versnelling in m/s²
 */
void Voertuig::setVersnelling(const double nieuweVersnelling) {
    versnelling = nieuweVersnelling;
}

/**
 * @brief Methode om de voertuigpositie te updaten en ook de snelheid op voorbije tijd
 * @param tijdstap De tijdstap in seconden
 */
void Voertuig::rijd(const double tijdstap) {
    // Update snelheid eerst: v = v0 + a * t
    double nieuweSnelheid = snelheid + versnelling * tijdstap;

    // Ensure speed is not negative
    if (nieuweSnelheid < 0) {
        nieuweSnelheid = 0;
    }

    // Update positie: x = x0 + v0 * t + 0.5 * a * t²
    // gebruikt de gemiddelde snelheid: (initialSpeed + finalSpeed) / 2
    const double gemiddeldeSnelheid = (snelheid + nieuweSnelheid) / 2;
    const int nieuwePositie = positie + static_cast<int>(gemiddeldeSnelheid * tijdstap);

    // Update de waardes
    snelheid = nieuweSnelheid;
    positie = nieuwePositie;
}