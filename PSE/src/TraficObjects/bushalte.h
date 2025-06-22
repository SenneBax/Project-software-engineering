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
 *
 * Bushalte hoort bij een bepaalde positie en tijd
 * Er wordt gemonitord hoelang de bus is gestopt en hoe lang hij is gestopt
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
     * @pre !baan.empty()
     * @pre positie >= 0.0
     * @pre wachttijd >= 0
     * @post properlyInitialized() == true
     * @post getBaan() == baan
     * @post getPositie() == positie
     * @post getWachttijd() == wachttijd
     */
    Bushalte(const std::string& baan, double positie, int wachttijd);

    /**
     * @brief Getter voor de baannaam van de bushalte
     * @return De naam van de baan
     * @pre properlyInitialized() == true
     * @post return waarde is de huidige naam van de baan
     */
    std::string getBaan() const;

    /**
     * @brief Getter voor de positie van de bushalte
     * @return De positie in meters
     * @pre properlyInitialized() == true
     * @post return waarde is de huidige positie
     */
    double getPositie() const;

    /**
     * @brief Getter voor de wachttijd van de bushalte
     * @return De wachttijd in seconden
     * @pre properlyInitialized() == true
     * @post return waarde is de huidige wachttijd
     */
    int getWachttijd() const;

    /**
     * @brief Update de timer voor een gestopte bus
     * @param timestep De huidige simulatie-tijdstap
     * @return True als de wachttijd voorbij is, false indien niet
     * @pre properlyInitialized() == true
     * @pre timestep >= 0.0
     * @post tijdSindsLaatsteStop is verhoogd met timestep indien bus gestopt is
     */
    bool updateWachttijd(double timestep);

    /**
     * @brief Registreert dat een bus gestopt is bij deze halte
     * @pre properlyInitialized() == true
     * @post isBusGestopt() == true
     * @post tijdSindsLaatsteStop == 0.0
     */
    void setBusStopped();

    /**
     * @brief Registreert dat een bus deze halte heeft verlaten
     * @pre properlyInitialized() == true
     * @post isBusGestopt() == false
     * @post tijdSindsLaatsteStop == 0.0
     */
    void setBusLeft();

    /**
     * @brief Controleert of er momenteel een bus gestopt is bij deze halte
     * @return True als er een bus gestopt is, false indien niet
     * @pre properlyInitialized() == true
     * @post return waarde geeft de huidige toestand van isBusStopped
     */
    bool isBusGestopt() const;

    /**
     * @brief Reset de wachttijd
     * @pre properlyInitialized() == true
     * @post tijdSindsLaatsteStop == 0.0
     */
    void resetWachttijd();

    /**
     * @brief Controleer of het VerkeersSituatie object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand is
     */

    bool properlyInitialized() const;
};

#endif // BUSHALTE_H