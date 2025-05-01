/**
 * @file bushalte.h
 * @brief Definition of the bus stop class
 */

#ifndef BUSHALTE_H
#define BUSHALTE_H

#include <string>

/**
 * @class Bushalte
 * @brief Class representing a bus stop in the traffic simulation
 */
class Bushalte {
private:
    std::string baan;   /**< Name of the road where the bus stop is located */
    double positie;     /**< Position of the bus stop on the road in meters */
    int wachttijd;      /**< Waiting time at the bus stop in seconds */
    double tijdSindsLaatsteStop; /**< Time since the last bus stopped here */
    bool isBusStopped;  /**< Flag indicating if a bus is currently stopped at this stop */
    Bushalte* _initCheck;

public:
    /**
     * @brief Constructor for the bus stop
     * @param baan Name of the road
     * @param positie Position on the road in meters
     * @param wachttijd Waiting time in seconds
     */
    Bushalte(const std::string& baan, double positie, int wachttijd);

    /**
     * @brief Getter for the road name of the bus stop
     * @return The name of the road
     */
    std::string getBaan() const;

    /**
     * @brief Getter for the position of the bus stop
     * @return The position in meters
     */
    double getPositie() const;

    /**
     * @brief Getter for the waiting time of the bus stop
     * @return The waiting time in seconds
     */
    int getWachttijd() const;

    /**
     * @brief Updates the timer for a stopped bus
     * @param timestep The current simulation time step
     * @return True if waiting time is over, false otherwise
     */
    bool updateWachttijd(double timestep);

    /**
     * @brief Records that a bus has stopped at this stop
     */
    void setBusStopped();

    /**
     * @brief Records that a bus has left this stop
     */
    void setBusLeft();

    /**
     * @brief Checks if a bus is currently stopped at this stop
     * @return True if a bus is stopped, false otherwise
     */
    bool isBusGestopt() const;

    /**
     * @brief Resets the waiting time
     */
    void resetWachttijd();

    bool properlyInitialized() const;
};

#endif // BUSHALTE_H