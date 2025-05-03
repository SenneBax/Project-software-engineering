/**
 * @file verkeerslicht.h
 * @brief Definition of the traffic light class (Revised with orange light support)
 */

#ifndef VERKEERSLICHT_H
#define VERKEERSLICHT_H

#include <string>

/**
 * @class Verkeerslicht
 * @brief Class that represents a traffic light in the traffic simulation
 */
class Verkeerslicht {
public:
    /**
     * @brief Enum for the traffic light colors
     */
    enum class Kleur {
        GROEN,
        ORANJE,
        ROOD
    };

private:
    std::string baan;         /**< Name of the road where the traffic light is located */
    double positie;           /**< Position of the traffic light on the road in meters */
    int cyclus;               /**< Cycle time of the traffic light in seconds */
    double tijdSindsLaatsteWissel; /**< Time since the last color change */
    Kleur huidigeKleur;       /**< Current color of the traffic light */
    bool heeftOranje;         /**< Whether this traffic light has orange state */
    bool isSlim;              /**< Whether this is a smart traffic light */
    int voertuigenVoorLicht;  /**< Number of vehicles waiting at the light */
    Verkeerslicht* _initCheck;



public:
    /**
     * @brief Constructor for the traffic light
     * @param baan Name of the road
     * @param positie Position on the road in meters
     * @param cyclus Cycle time in seconds
     * @param heeftOranje Whether this traffic light has orange state
     * @param isSlim Whether this is a smart traffic light
     */
    Verkeerslicht(const std::string& baan, double positie, int cyclus, bool heeftOranje = false, bool isSlim = false);

    /**
     * @brief Getter for the road name of the traffic light
     * @return The name of the road
     */
    bool properlyInitialized() const;

    std::string getBaan() const;

    /**
     * @brief Getter for the position of the traffic light
     * @return The position in meters
     */
    double getPositie() const;

    /**
     * @brief Getter for the cycle time of the traffic light
     * @return The cycle time in seconds
     */
    int getCyclus() const;

    /**
     * @brief Getter for the current color of the traffic light
     * @return The current color
     */
    Kleur getKleur() const;

    /**
     * @brief Setter for the current color of the traffic light
     * @param kleur The new color
     */
    void setKleur(Kleur kleur);

    /**
     * @brief Get color as string
     * @return String representation of the current color
     */
    std::string getKleurAsString() const;

    /**
     * @brief Checks if the traffic light is red
     * @return True if the light is red, false otherwise
     */
    bool isRood() const;

    /**
     * @brief Checks if the traffic light is orange
     * @return True if the light is orange, false otherwise
     */
    bool isOranje() const;

    /**
     * @brief Checks if the traffic light is green
     * @return True if the light is green, false otherwise
     */
    bool isGroen() const;

    /**
     * @brief Checks if this traffic light has orange state
     * @return True if the light has orange state, false otherwise
     */
    bool getHeeftOranje() const;

    /**
     * @brief Checks if this is a smart traffic light
     * @return True if the light is smart, false otherwise
     */
    bool getIsSlim() const;

    /**
     * @brief Update the traffic light based on elapsed time
     * @param tijdstap The current simulation time step
     */
    void update(double tijdstap);

    /**
     * @brief Register a vehicle waiting at this light (for smart traffic lights)
     */
    void registerVoertuigVoorLicht();

    /**
     * @brief Unregister a vehicle that was waiting at this light (for smart traffic lights)
     */
    void unregisterVoertuigVoorLicht();

    /**
     * @brief Get the number of vehicles waiting at this light
     * @return The number of vehicles
     */
    int getVoertuigenVoorLicht() const;

    /**
     * @brief Reset the number of vehicles waiting at this light
     */
    void resetVoertuigenVoorLicht();


};

#endif // VERKEERSLICHT_H