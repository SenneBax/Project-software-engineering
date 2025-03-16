/**
 * @file Voertuig.h
 * @author senne
 * @date 27/02/2025
 * @brief definitie van de voertuigklasse
 */

// Updated Voertuig.h
#ifndef VOERTUIG_H
#define VOERTUIG_H

#include <string>
using namespace std;

/**
 * @class Voertuig
 * @brief Klasse die een voertuig representeert in de verkeerssimulatie
 */
class Voertuig {
private:
    string baan;       /**< Naam van de baan waarop het voertuig zich bevindt */
    int positie;       /**< Positie van het voertuig op de baan in meters */
    double snelheid;   /**< Snelheid van het voertuig in m/s */
    double versnelling; /**< Versnelling van het voertuig in m/s² */

public:
    /**
     * @brief Constructor voor een voertuig
     * @param baan Naam van de baan
     * @param positie Positie op de baan in meters
     * @param snelheid Snelheid van het voertuig in m/s, standaard 0.0
     * @param versnelling Versnelling van het voertuig in m/s², standaard 0.0
     */
    Voertuig(const string& baan, int positie, double snelheid = 0.0, double versnelling = 0.0);

    /**
     * @brief Getter voor de baan van het voertuig
     * @return De naam van de baan
     */
    string getBaan() const;

    /**
     * @brief Getter voor de positie van het voertuig
     * @return De positie in meters
     */
    [[nodiscard]] int getPositie() const;

    /**
     * @brief Getter voor de snelheid van het voertuig
     * @return De snelheid in m/s
     */
    [[nodiscard]] double getSnelheid() const;

    /**
     * @brief Getter voor de versnelling van het voertuig
     * @return De versnelling in m/s²
     */
    [[nodiscard]] double getVersnelling() const;

    /**
     * @brief Setter voor de positie van het voertuig
     * @param nieuwePositie De nieuwe positie in meters
     */
    void setPositie(int nieuwePositie);

    /**
     * @brief Setter voor de snelheid van het voertuig
     * @param nieuweSnelheid De nieuwe snelheid in m/s
     */
    void setSnelheid(double nieuweSnelheid);

    /**
     * @brief Setter voor de versnelling van het voertuig
     * @param nieuweVersnelling De nieuwe versnelling in m/s²
     */
    void setVersnelling(double nieuweVersnelling);

    /**
     * @brief Methode om de voertuigpositie te updaten en ook de snelheid op voorbije tijd
     * @param tijdstap De tijdstap in seconden
     */
    void rijd(double tijdstap);
};

#endif // VOERTUIG_H