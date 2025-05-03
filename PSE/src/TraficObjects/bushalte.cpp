/**
 * @file bushalte.cpp
 * @brief Implementation of the bus stop class
 */

#include "bushalte.h"
#include "DesignByContract.h"

/**
 * @brief Constructor for the bus stop
 * @param baan Name of the road
 * @param positie Position on the road in meters
 * @param wachttijd Waiting time in seconds
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
 * @brief Getter for the road name of the bus stop
 * @return The name of the road
 */
std::string Bushalte::getBaan() const {
    //REQUIRE(properlyInitialized(), "Bushalte is niet correct geïnitialiseerd bij getBaan.");
    return baan;
}

/**
 * @brief Getter for the position of the bus stop
 * @return The position in meters
 */
double Bushalte::getPositie() const {
    //REQUIRE(properlyInitialized(), "Positie is niet correct geïnitialiseerd bij getPositie");
    return positie;
}

/**
 * @brief Getter for the waiting time of the bus stop
 * @return The waiting time in seconds
 */
int Bushalte::getWachttijd() const {
    REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij getWachttijd.");
    return wachttijd;
}

/**
 * @brief Updates the timer for a stopped bus
 * @param timestep The current simulation time step
 * @return True if waiting time is over, false otherwise
 */
bool Bushalte::updateWachttijd(double timestep) {
    REQUIRE(properlyInitialized(), "Wachttijd is niet correct geïnitialiseerd bij updateWachttijd.");
    REQUIRE(timestep >= 0.0, "timestep moet positief zijn.");
    if (isBusStopped) {
        tijdSindsLaatsteStop += timestep;

        // Check if waiting time is over
        if (tijdSindsLaatsteStop >= wachttijd) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Records that a bus has stopped at this stop
 */
void Bushalte::setBusStopped() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusStopped");
    isBusStopped = true;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Records that a bus has left this stop
 */
void Bushalte::setBusLeft() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij setBusLeft");
    isBusStopped = false;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Checks if a bus is currently stopped at this stop
 * @return True if a bus is stopped, false otherwise
 */
bool Bushalte::isBusGestopt() const {
    //REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij isBusGestopt");
    return isBusStopped;
}

/**
 * @brief Resets the waiting time
 */
void Bushalte::resetWachttijd() {
    REQUIRE(properlyInitialized(), "Bushalte niet correct geïnitialiseerd bij resetWachttijd");
    tijdSindsLaatsteStop = 0.0;
}