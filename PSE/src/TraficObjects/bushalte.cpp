/**
 * @file bushalte.cpp
 * @brief Implementatie van de bushalte-klasse
 */

#include "bushalte.h"
#include "DesignByContract.h"
#include <algorithm>

/**
 * @brief Constructor voor de bushalte
 * @param baan Naam van de baan
 * @param positie Positie op de baan in meters
 * @param wachttijd Wachttijd in seconden
 * REQUIRE(!baan.empty(), "naam van de baan mag niet leeg zijn.");
 * REQUIRE(positie >= 0.0, "Positie mag niet negatief zijn.");
 * REQUIRE(wachttijd >= 0, "Wachttijd mag niet negatief zijn." );
 * ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
 */
Bushalte::Bushalte(const std::string& baan, double positie, int wachttijd)
    : baan(baan), positie(positie), wachttijd(wachttijd), tijdSindsLaatsteStop(0.0), isBusStopped(false)
{
    _initCheck = this;

    REQUIRE(!baan.empty(), "naam van de baan mag niet leeg zijn.");
    REQUIRE(positie >= 0.0, "Positie mag niet negatief zijn.");
    REQUIRE(wachttijd >= 0, "Wachttijd mag niet negatief zijn." );

    ENSURE(properlyInitialized(), "Constructor moet eindigen in een geldige toestand.");
}


bool Bushalte::properlyInitialized() const
{
    return _initCheck == this;
}


/**
 * @brief Getter voor de baannaam van de bushalte
 * @return De naam van de baan
 */
std::string Bushalte::getBaan() const {
    //REQUIRE(properlyInitialized(), "Bushalte is niet correct geïnitialiseerd bij getBaan.");
    return baan;
}

/**
 * @brief Getter voor de positie van de bushalte
 * @return De positie in meters
 */
double Bushalte::getPositie() const {
    //REQUIRE(properlyInitialized(), "Positie is niet correct geïnitialiseerd bij getPositie");
    return positie;
}

/**
 * @brief Getter voor de wachttijd van de bushalte
 * @return De wachttijd in seconden
 * REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij getWachttijd.");
 */
int Bushalte::getWachttijd() const {
    REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij getWachttijd.");
    return wachttijd;
}

/**
 * @brief Update de timer voor een gestopte bus
 * @param timestep De huidige simulatie-tijdstap
 * @return True als de wachttijd voorbij is, false indien niet
 * REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij updateWachttijd.");
 * REQUIRE(timestep >= 0.0, "timestep moet positief zijn.");
 */
bool Bushalte::updateWachttijd(double timestep) {
    REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij updateWachttijd.");
    REQUIRE(timestep >= 0.0, "timestep moet positief zijn.");
    if (isBusStopped) {
        tijdSindsLaatsteStop += timestep;

        // Controleer of de wachttijd voorbij is
        if (tijdSindsLaatsteStop >= wachttijd) {
            return true;
        }
    }
    return false;
    ENSURE(tijdSindsLaatsteStop >= timestep, "Tijd mag niet teruglopen");

}
/**
 * @brief Registreert dat een bus gestopt is bij deze halte
 * REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusStopped");
 */
void Bushalte::setBusStopped() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusStopped");
    isBusStopped = true;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Registreert dat een bus deze halte heeft verlaten
 * REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusLeft");
 */
void Bushalte::setBusLeft() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusLeft");
    isBusStopped = false;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Controleert of er momenteel een bus gestopt is bij deze halte
 * @return True als er een bus gestopt is, false indien niet
 */
bool Bushalte::isBusGestopt() const {
    //REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij isBusGestopt");
    return isBusStopped;
}

/**
 * @brief Reset de wachttijd
 * REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij resetWachttijd");
 */
void Bushalte::resetWachttijd() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij resetWachttijd");
    tijdSindsLaatsteStop = 0.0;
}