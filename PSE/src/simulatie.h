/**
 * @file simulatie.h
 * @author senne
 * @date 11/03/2025
 * @brief Definitie van de simulatie klasse
 */

#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "situatie.h"

/**
 * @class simulatie
 * @brief Klasse voor het simuleren van een verkeerssituatie
 */
class simulatie {
private:
    VerkeersSituatie& verkeerssituatie;
    double tijdstap;
    double huidigeSimulatieTijd;
    bool autoGenereerVoertuigen = false;

    // Standaardwaarden volgens sectie B.6
    double voertuigLengte;       /**< l - lengte van een voertuig */
    double maxSnelheid;          /**< Vmax - absolute maximale snelheid */
    double maxVersnelling;       /**< amax - maximale versnelling */
    double maxRemFactor;         /**< bmax - maximale remfactor */
    double minVolgafstand;       /**< fmin - minimale volgafstand */
    double vertraagAfstand;      /**< Δxs - vertraagafstand */
    double stopAfstand;          /**< Δxs0 - stopafstand */
    double vertraagFactor;       /**< s - vertraagfactor */

    // Statistieken
    int aantalVoertuigen = 0;
    double gemiddeldeSnelheid = 0.0;
    int verwijderdeVoertuigenTeller = 0;
    int totaalVerwijderdeVoertuigen = 0;
    double totaleTijd = 0.0;

    /**
     * @brief Ondersteunende methode om positie en snelheid van een voertuig te updaten
     * @param voertuig Het te updaten voertuig
     */
    void updatePositieEnSnelheid(Voertuig& voertuig) const;

    /**
     * @brief Ondersteunende methode om versnelling van een voertuig te updaten
     * @param voertuig Het te updaten voertuig
     * @param voorgaandVoertuig Pointer naar het voorgaande voertuig (nullptr als er geen is)
     * @param isEersteVoertuig Boolean die aangeeft of dit het eerste voertuig op de baan is
     */
    void updateVersnelling(Voertuig& voertuig, const Voertuig* voorgaandVoertuig, bool isEersteVoertuig);

    /**
     * @brief Zoekt het eerstvolgende verkeerslicht voor een voertuig
     * @param voertuig Het voertuig waarvoor gezocht wordt
     * @return Pointer naar het eerstvolgende verkeerslicht, of nullptr als er geen is
     */
    const Verkeerslicht* zoekEerstvolgendVerkeerslicht(const Voertuig& voertuig) const;

    /**
     * @brief Controleert of een verkeerslicht rood is
     * @param verkeerslicht Het te controleren verkeerslicht
     * @return true als het verkeerslicht rood is, anders false
     */
    bool isVerkeerslichtRood(const Verkeerslicht& verkeerslicht) const;

    /**
     * @brief Genereert nieuwe voertuigen in de simulatie
     */
    void genereerNieuweVoertuigen() const;

    /**
     * @brief Verhoogt de teller voor verwijderde voertuigen
     */
    void verhoogVerwijderdeVoertuigenTeller();

    /**
     * @brief Verzamelt statistieken over de simulatie
     */
    void verzamelStatistieken();

public:
    /**
     * @brief Constructor voor de simulatie klasse
     * @param situatie Referentie naar de te simuleren verkeerssituatie
     * @param tijdstap De tijdstap voor de simulatie in seconden
     */
    simulatie(VerkeersSituatie& situatie, double tijdstap);

    void stap();
    void setAutoGenereerVoertuigen(bool genereer);

    [[nodiscard]] double getHuidigeSimulatieTijd() const;
    [[nodiscard]] double getTijdstap() const;
    [[nodiscard]] int getAantalVoertuigen() const;
    [[nodiscard]] double getGemiddeldeSnelheid() const;
    [[nodiscard]] int getTotaalVerwijderdeVoertuigen() const;
    [[nodiscard]] double getTotaleTijd() const;
};

#endif //SIMULATIE_H