/**
 * @file verkeerslicht.cpp
 * @brief Implementatie van de verkeerslicht klasse (Herzien met ondersteuning voor oranje licht)
 */

#include "verkeerslicht.h"
#include <cmath>
#include <gtest/internal/gtest-port.h>
#include "DesignByContract.h"


/**
 * @brief Constructor voor het verkeerslicht
 * @param baan Naam van de baan
 * @param positie Positie op de baan in meters
 * @param cyclus Cyclustijd in seconden
 * @param heeftOranje Of dit verkeerslicht een oranje status heeft
 * @param isSlim Of dit een slim verkeerslicht is
 * @pre REQUIRE(!baan.empty(), "Baannaam mag niet leeg zijn.");
 * @post ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
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
 * @brief Getter voor de baannaam van het verkeerslicht
 * @return De naam van de baan
 * @post ENSURE(!baan.empty(), "Baanaam mag niet leeg zijn.");
 */
std::string Verkeerslicht::getBaan() const {
    //REQUIRE(properlyInitialized(), "getBaan moet eindigen in een geldige toestand.");
    ENSURE(!baan.empty(), "Baanaam mag niet leeg zijn.");

    return baan;
}


/**
 * @brief Getter voor de positie van het verkeerslicht
 * @return De positie in meters
 */
double Verkeerslicht::getPositie() const {
    REQUIRE(properlyInitialized(), "getPositie moet eindigen in een geldige toestand.");
    ENSURE(positie >= 0.0, "Positie moet positief zijn");
    return positie;
}

/**
 * @brief Getter voor de cyclustijd van het verkeerslicht
 * @return De cyclustijd in seconden
 * @pre REQUIRE(properlyInitialized(), "getCyclus moet eindigen in een geldige toestand.");
 */
int Verkeerslicht::getCyclus() const {
    REQUIRE(properlyInitialized(), "getCyclus moet eindigen in een geldige toestand.");
    ENSURE(cyclus >= 0, "Cyclus moet groter dan 0 zijn.");
    return cyclus;
}

/**
 * @brief Getter voor de huidige kleur van het verkeerslicht
 * @return De huidige kleur
 * @pre REQUIRE(properlyInitialized(), "getKleur moet eindigen in een geldige toestand.");
 */
Verkeerslicht::Kleur Verkeerslicht::getKleur() const {
    REQUIRE(properlyInitialized(), "getKleur moet eindigen in een geldige toestand.");

    return huidigeKleur;
}

/**
 * @brief Setter voor de huidige kleur van het verkeerslicht
 * @param kleur De nieuwe kleur
 * @pre REQUIRE(properlyInitialized(), "setKleur moet eindigen in een geldige toestand.");
 */
void Verkeerslicht::setKleur(Kleur kleur) {
    REQUIRE(properlyInitialized(), "setKleur moet eindigen in een geldige toestand.");

    huidigeKleur = kleur;
    tijdSindsLaatsteWissel = 0.0; // Reset timer bij kleurverandering
}

/**
 * @brief Haal kleur op als string
 * @return String-weergave van de huidige kleur
 * @pre REQUIRE(properlyInitialized(), "getKleurAsString moet eindigen in een geldige toestand.");
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
 * @brief Controleert of het verkeerslicht rood is
 * @return True als het licht rood is, false indien niet
 * @pre REQUIRE(properlyInitialized(), "isRood moet eindigen in een geldige toestand.");
 */
bool Verkeerslicht::isRood() const {
    REQUIRE(properlyInitialized(), "isRood moet eindigen in een geldige toestand.");
    return huidigeKleur == Kleur::ROOD;
}

/**
 * @brief Controleert of het verkeerslicht oranje is
 * @return True als het licht oranje is, false indien niet
 * @pre REQUIRE(properlyInitialized(), "isOranje moet eindigen in een geldige toestand.");
 */
bool Verkeerslicht::isOranje() const {
    REQUIRE(properlyInitialized(), "isOranje moet eindigen in een geldige toestand.");

    return huidigeKleur == Kleur::ORANJE;
}

/**
 * @brief Controleert of het verkeerslicht groen is
 * @return True als het licht groen is, false indien niet
 * @pre REQUIRE(properlyInitialized(), "isGroen moet eindigen in een geldige toestand.");
 */
bool Verkeerslicht::isGroen() const {
    REQUIRE(properlyInitialized(), "isGroen moet eindigen in een geldige toestand.");

    return huidigeKleur == Kleur::GROEN;
}

/**
 * @brief Controleert of dit verkeerslicht een oranje status heeft
 * @return True als het licht een oranje status heeft, false indien niet
 */
bool Verkeerslicht::getHeeftOranje() const {
    //REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand.");
    return heeftOranje;
}

/**
 * @brief Controleert of dit een slim verkeerslicht is
 * @return True als het licht slim is, false indien niet
 * @pre REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand.");
 */
bool Verkeerslicht::getIsSlim() const {
    REQUIRE(properlyInitialized(), "getHeeftOranje moet eindigen in een geldige toestand.");
    return isSlim;
}

/**
 * @brief Update het verkeerslicht op basis van verstreken tijd
 * @param tijdstap De huidige simulatie-tijdstap
 * @post ENSURE(isGroen(), "Moet groen zijn na schakelen");
 */
void Verkeerslicht::update(double tijdstap) {
    //REQUIRE(properlyInitialized(), "update moet eindigen in een geldige toestand.");

    tijdSindsLaatsteWissel += tijdstap;

    // Slim verkeerslicht logica
    if (isSlim) {
        // Minimale tijd in elke status is 10 seconden, maximale is 60 seconden
        if (tijdSindsLaatsteWissel >= 10.0) {
            // Als er voertuigen wachten en het licht is rood, schakel naar groen
            if (isRood() && voertuigenVoorLicht > 0) {
                setKleur(Kleur::GROEN);
                ENSURE(isGroen(), "Moet groen zijn na schakelen");
                resetVoertuigenVoorLicht();
            }
            // Als er een tijdje geen voertuigen zijn en het licht is groen, schakel naar oranje/rood
            else if (isGroen() && voertuigenVoorLicht == 0 && tijdSindsLaatsteWissel >= 20.0) {
                if (heeftOranje) {
                    setKleur(Kleur::ORANJE);
                } else {
                    setKleur(Kleur::ROOD);
                }
            }
            // Maximale tijd in elke status is 60 seconden
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
            // Verwerk oranje status (blijft alleen oranje voor 10% van de cyclus)
            else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                setKleur(Kleur::ROOD);
            }
        }
    }
    // Gewone verkeerslicht logica
    else {
        // De verkeerslicht cyclus
        if (heeftOranje) {
            // Met oranje licht:
            // Groen -> Oranje -> Rood -> Groen -> ...
            // Oranje duur is 10% van de cyclus
            if (isGroen() && tijdSindsLaatsteWissel >= cyclus) {
                setKleur(Kleur::ORANJE);
            } else if (isOranje() && tijdSindsLaatsteWissel >= cyclus * 0.1) {
                setKleur(Kleur::ROOD);
            } else if (isRood() && tijdSindsLaatsteWissel >= cyclus) {
                setKleur(Kleur::GROEN);
            }
        } else {
            // Zonder oranje licht: wissel gewoon tussen rood en groen
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
 * @brief Registreer een voertuig wachtend voor dit licht (voor slimme verkeerslichten)
 * @pre REQUIRE(properlyInitialized(), "registerVoertuigVoorLicht moet eindigen in een geldige toestand.");
 */
void Verkeerslicht::registerVoertuigVoorLicht() {
    REQUIRE(properlyInitialized(), "registerVoertuigVoorLicht moet eindigen in een geldige toestand.");

    if (isSlim) {
        voertuigenVoorLicht++;
    }
}

/**
 * @brief Deregistreer een voertuig dat aan dit licht wachtte (voor slimme verkeerslichten)
 * @pre REQUIRE(properlyInitialized(), "unregisterVoertuigVoorLicht moet eindigen in een geldige toestand.");
 */
void Verkeerslicht::unregisterVoertuigVoorLicht() {
    REQUIRE(properlyInitialized(), "unregisterVoertuigVoorLicht moet eindigen in een geldige toestand.");

    if (isSlim && voertuigenVoorLicht > 0) {
        voertuigenVoorLicht--;
    }
}

/**
 * @brief Haal het aantal voertuigen op dat voor dit licht wacht
 * @return Het aantal voertuigen
 * @pre REQUIRE(properlyInitialized(), "getVoertuigenVoorLicht moet eindigen in een geldige toestand");
 * @post ENSURE(voertuigenVoorLicht >= 0, "Aantal voertuigen mag niet negatief zijn.");
 */
int Verkeerslicht::getVoertuigenVoorLicht() const {
    REQUIRE(properlyInitialized(), "getVoertuigenVoorLicht moet eindigen in een geldige toestand");
    ENSURE(voertuigenVoorLicht >= 0, "Aantal voertuigen mag niet negatief zijn.");
    return voertuigenVoorLicht;
}

/**
 * @brief Reset het aantal voertuigen dat voor dit licht wacht
 * @pre REQUIRE(properlyInitialized(), "resetVoertuigenVoorLicht moet eindigen in een geldige toestand");
 * @post ENSURE(voertuigenVoorLicht == 0, "Aantal voertuigen moet op 0 staan.");
 */
void Verkeerslicht::resetVoertuigenVoorLicht() {
    REQUIRE(properlyInitialized(), "resetVoertuigenVoorLicht moet eindigen in een geldige toestand");
    voertuigenVoorLicht = 0;
    ENSURE(voertuigenVoorLicht == 0, "Aantal voertuigen moet op 0 staan.");
}