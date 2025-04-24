/**
 * @file bushalte.cpp
 * @brief Implementation of the bus stop class
 */

#include "bushalte.h"

/**
 * @brief Constructor for the bus stop
 * @param baan Name of the road
 * @param positie Position on the road in meters
 * @param wachttijd Waiting time in seconds
 */
Bushalte::Bushalte(const std::string& baan, double positie, int wachttijd)
    : baan(baan), positie(positie), wachttijd(wachttijd), tijdSindsLaatsteStop(0.0), isBusStopped(false) {}

/**
 * @brief Getter for the road name of the bus stop
 * @return The name of the road
 */
std::string Bushalte::getBaan() const {
    return baan;
}

/**
 * @brief Getter for the position of the bus stop
 * @return The position in meters
 */
double Bushalte::getPositie() const {
    return positie;
}

/**
 * @brief Getter for the waiting time of the bus stop
 * @return The waiting time in seconds
 */
int Bushalte::getWachttijd() const {
    return wachttijd;
}

/**
 * @brief Updates the timer for a stopped bus
 * @param timestep The current simulation time step
 * @return True if waiting time is over, false otherwise
 */
bool Bushalte::updateWachttijd(double timestep) {
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
    isBusStopped = true;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Records that a bus has left this stop
 */
void Bushalte::setBusLeft() {
    isBusStopped = false;
    tijdSindsLaatsteStop = 0.0;
}

/**
 * @brief Checks if a bus is currently stopped at this stop
 * @return True if a bus is stopped, false otherwise
 */
bool Bushalte::isBusGestopt() const {
    return isBusStopped;
}

/**
 * @brief Resets the waiting time
 */
void Bushalte::resetWachttijd() {
    tijdSindsLaatsteStop = 0.0;
}