/**
 * @file verkeerslicht.cpp
 * @brief Implementation of the traffic light class (Revised with orange light support)
 */

#include "verkeerslicht.h"
#include <cmath>
#include <gtest/internal/gtest-port.h>
#include "DesignByContract.h"


/**
 * @brief Constructor for the traffic light
 * @param baan Name of the road
 * @param positie Position on the road in meters
 * @param cyclus Cycle time in seconds
 * @param heeftOranje Whether this traffic light has orange state
 * @param isSlim Whether this is a smart traffic light
 */
Verkeerslicht::Verkeerslicht(const std::string& baan, double positie, int cyclus, bool heeftOranje, bool isSlim)
    : baan(baan), positie(positie), cyclus(cyclus), tijdSindsLaatsteWissel(0.0),
      huidigeKleur(Kleur::ROOD), heeftOranje(heeftOranje), isSlim(isSlim), voertuigenVoorLicht(0)
{
    REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
    //REQUIRE(positie >= 0.0, "Positie moet positief zijn." );
    //REQUIRE(cyclus > 0, "Cyclus moet groter dan 0 zijn." );

    _initCheck = this;
    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}


bool Verkeerslicht::properlyInitialized() const
{
    return _initCheck == this;
}


/**
 * @brief Getter for the road name of the traffic light
 * @return The name of the road
 */
std::string Verkeerslicht::getBaan() const {
    //REQUIRE(properlyInitialized(), "getBaan moet eindigen in een geldige toestand.");
    ENSURE(!baan.empty(), "Baanaam mag niet leeg zijn.");

    return baan;
}


/**
 * @brief Getter for the position of the traffic light
 * @return The position in meters
 */
double Verkeerslicht::getPositie() const {
    //REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
    //ENSURE(positie >= 0.0, "Positie moet positief zijn");
    return positie;
}

/**
 * @brief Getter for the cycle time of the traffic light
 * @return The cycle time in seconds
 */
int Verkeerslicht::getCyclus() const {
    REQUIRE(properlyInitialized(), "getCyclus moet eindigen in een geldige toestand.");
    //ENSURE(cyclus >= 0, "Cyclus moet groter dan 0 zijn.");
    return cyclus;
}

/**
 * @brief Getter for the current color of the traffic light
 * @return The current color
 */
Verkeerslicht::Kleur Verkeerslicht::getKleur() const {
    REQUIRE(properlyInitialized(), "getKleur moet eindigen in een geldige toestand.");

    return huidigeKleur;
}

/**
 * @brief Setter for the current color of the traffic light
 * @param kleur The new color
 */
void Verkeerslicht::setKleur(Kleur kleur) {
    REQUIRE(properlyInitialized(), "setKleur moet eindigen in een geldige toestand.");

    huidigeKleur = kleur;
    tijdSindsLaatsteWissel = 0.0; // Reset timer when changing color
}

/**
 * @brief Get color as string
 * @return String representation of the current color
 */
std::string Verkeerslicht::getKleurAsString() const {
    REQUIRE(properlyInitialized(), "getKleurAsString moet eindigen in een geldige toestand.");

    switch (huidigeKleur) {
        case Kleur::GROEN: return "groen";
        case Kleur::ORANJE: return "oranje";
        case Kleur::ROOD: return "rood";
        default: return "onbekend";
    }
}

/**
 * @brief Checks if the traffic light is red
 * @return True if the light is red, false otherwise
 */
bool Verkeerslicht::isRood() const {
    //REQUIRE(properlyInitialized(), "isRood moet eindigen in een geldige toestand.");
    return huidigeKleur == Kleur::ROOD;
}

/**
 * @brief Checks if the traffic light is orange
 * @return True if the light is orange, false otherwise
 */
bool Verkeerslicht::isOranje() const {
    //REQUIRE(properlyInitialized(), "isOranje moet eindigen in een geldige toestand.");

    return huidigeKleur == Kleur::ORANJE;
}

/**
 * @brief Checks if the traffic light is green
 * @return True if the light is green, false otherwise
 */
bool Verkeerslicht::isGroen() const {
    //REQUIRE(properlyInitialized(), "isGroen moet eindigen in een geldige toestand.");

    return huidigeKleur == Kleur::GROEN;
}

/**
 * @brief Checks if this traffic light has orange state
 * @return True if the light has orange state, false otherwise
 */
bool Verkeerslicht::getHeeftOranje() const {
    //REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand.");
    return heeftOranje;
}

/**
 * @brief Checks if this is a smart traffic light
 * @return True if the light is smart, false otherwise
 */
bool Verkeerslicht::getIsSlim() const {
    REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand.");
    return isSlim;
}

/**
 * @brief Update the traffic light based on elapsed time
 * @param tijdstap The current simulation time step
 */
void Verkeerslicht::update(double tijdstap) {
    //REQUIRE(properlyInitialized(), "update moet eindigen in een geldige toestand.");

    tijdSindsLaatsteWissel += tijdstap;

    // Smart traffic light logic
    if (isSlim) {
        // Minimum time in each state is 10 seconds, maximum is 60 seconds
        if (tijdSindsLaatsteWissel >= 10.0) {
            // If there are vehicles waiting and the light is red, switch to green
            if (isRood() && voertuigenVoorLicht > 0) {
                setKleur(Kleur::GROEN);
                resetVoertuigenVoorLicht();
            }
            // If no vehicles for a while and the light is green, switch to orange/red
            else if (isGroen() && voertuigenVoorLicht == 0 && tijdSindsLaatsteWissel >= 20.0) {
                if (heeftOranje) {
                    setKleur(Kleur::ORANJE);
                } else {
                    setKleur(Kleur::ROOD);
                }
            }
            // Maximum time in any state is 60 seconds
            else if (tijdSindsLaatsteWissel >= 60.0) {
                if (isGroen()) {
                    if (heeftOranje) {
                        setKleur(Kleur::ORANJE);
                    } else {
                        setKleur(Kleur::ROOD);
                    }
                } else if (isRood()) {
                    setKleur(Kleur::GROEN);
                    resetVoertuigenVoorLicht();
                }
            }
            // Handle orange state (only stays orange for 10% of the cycle)
            else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                setKleur(Kleur::ROOD);
            }
        }
    }
    // Regular traffic light logic
    else {
        // The traffic light cycle
        if (heeftOranje) {
            // With orange light:
            // Green -> Orange -> Red -> Green -> ...
            // Orange duration is 10% of the cycle
            if (isGroen() && tijdSindsLaatsteWissel >= cyclus) {
                setKleur(Kleur::ORANJE);
            } else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                setKleur(Kleur::ROOD);
            } else if (isRood() && tijdSindsLaatsteWissel >= cyclus) {
                setKleur(Kleur::GROEN);
            }
        } else {
            // Without orange light: just toggle between red and green
            if (tijdSindsLaatsteWissel >= cyclus) {
                if (isGroen()) {
                    setKleur(Kleur::ROOD);
                } else {
                    setKleur(Kleur::GROEN);
                }
            }
        }
    }
}

/**
 * @brief Register a vehicle waiting at this light (for smart traffic lights)
 */
void Verkeerslicht::registerVoertuigVoorLicht() {
    REQUIRE(properlyInitialized(), "registerVoertuigVoorLicht moet eindigen in een geldige toestand.");

    if (isSlim) {
        voertuigenVoorLicht++;
    }
}

/**
 * @brief Unregister a vehicle that was waiting at this light (for smart traffic lights)
 */
void Verkeerslicht::unregisterVoertuigVoorLicht() {
    REQUIRE(properlyInitialized(), "unregisterVoertuigVoorLicht moet eindigen in een geldige toestand.");

    if (isSlim && voertuigenVoorLicht > 0) {
        voertuigenVoorLicht--;
    }
}

/**
 * @brief Get the number of vehicles waiting at this light
 * @return The number of vehicles
 */
int Verkeerslicht::getVoertuigenVoorLicht() const {
    REQUIRE(properlyInitialized(), "getVoertuigenVoorLicht moet eindigen in een geldige toestand");
    ENSURE(voertuigenVoorLicht >= 0, "Aantal voertuigen mag niet negatief zijn.");
    return voertuigenVoorLicht;
}

/**
 * @brief Reset the number of vehicles waiting at this light
 */
void Verkeerslicht::resetVoertuigenVoorLicht() {
    REQUIRE(properlyInitialized(), "resetVoertuigenVoorLicht moet eindigen in een geldige toestand");
    voertuigenVoorLicht = 0;
    ENSURE(voertuigenVoorLicht == 0, "Aantal voertuigen moet op 0 staan.");
}