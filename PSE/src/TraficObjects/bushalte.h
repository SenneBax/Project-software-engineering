/**
 * @file bushalte.h
 * @brief Definitie van de bushalte-klasse
 */

#ifndef BUSHALTE_H
#define BUSHALTE_H

#include <string>

/**
 * @class Bushalte
 * @brief Klasse die een bushalte in de verkeerssimulatie voorstelt
 */
class Bushalte {
private:
    std::string baan;   /**< Naam van de baan waar de bushalte zich bevindt */
    double positie;     /**< Positie van de bushalte op de baan in meters */
    int wachttijd;      /**< Wachttijd bij de bushalte in seconden */
    double tijdSindsLaatsteStop; /**< Tijd sinds de laatste bus hier stopte */
    bool isBusStopped;  /**< Vlag die aangeeft of er momenteel een bus gestopt is bij deze halte */
    Bushalte* _initCheck;

public:
    /**
     * @brief Constructor voor de bushalte
     * @param baan Naam van de baan
     * @param positie Positie op de baan in meters
     * @param wachttijd Wachttijd in seconden
     */
    Bushalte(const std::string& baan, double positie, int wachttijd);

    /**
     * @brief Getter voor de baannaam van de bushalte
     * @return De naam van de baan
     */
    std::string getBaan() const;

    /**
     * @brief Getter voor de positie van de bushalte
     * @return De positie in meters
     */
    double getPositie() const;

    /**
     * @brief Getter voor de wachttijd van de bushalte
     * @return De wachttijd in seconden
     */
    int getWachttijd() const;

    /**
     * @brief Update de timer voor een gestopte bus
     * @param timestep De huidige simulatie-tijdstap
     * @return True als de wachttijd voorbij is, false indien niet
     */
    bool updateWachttijd(double timestep);

    /**
     * @brief Registreert dat een bus gestopt is bij deze halte
     */
    void setBusStopped();

    /**
     * @brief Registreert dat een bus deze halte heeft verlaten
     */
    void setBusLeft();

    /**
     * @brief Controleert of er momenteel een bus gestopt is bij deze halte
     * @return True als er een bus gestopt is, false indien niet
     */
    bool isBusGestopt() const;

    /**
     * @brief Reset de wachttijd
     */
    void resetWachttijd();

    bool properlyInitialized() const;
};

#endif // BUSHALTE_H