/**
 * @file simulatie.h
 * @brief Definitie van de simulatieklasse (Herzien voor betere modulariteit)
 */

#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "../Situation/situatie.h"


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

    // Standaardwaarden uit sectie B.6
    double vertraagAfstand;      /**< Δxs - vertraagafstand */
    double stopAfstand;          /**< Δxs0 - stopafstand */
    double vertraagFactor;       /**< s - vertraagfactor */

    // Statistieken
    int aantalVoertuigen = 0;
    double gemiddeldeSnelheid = 0.0;
    int verwijderdeVoertuigenTeller = 0;
    int totaalVerwijderdeVoertuigen = 0;
    double totaleTijd = 0.0;

    simulatie* _initCheck;
    /**
     * @brief Verwerk alle verkeerslichten in de simulatie
     */
    void verwerkVerkeerslichten();

    /**
     * @brief Controleer of een voertuig een verkeerslicht nadert
     * @param voertuig Het te controleren voertuig
     * @param verkeerslicht Het te controleren verkeerslicht
     * @return Responscode: 0 = geen effect, 1 = vertragen, 2 = stoppen, 3 = doorrijden
     */
    int controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht);

    /**
     * @brief Verwerk alle voertuigen in de simulatie
     */
    void verwerkVoertuigen();

    /**
     * @brief Verwerk alle bushaltes in de simulatie
     */
    void verwerkBushaltes();

    /**
     * @brief Verwerk alle kruispunten in de simulatie
     */
    void verwerkKruispunten();

    /**
     * @brief Genereer nieuwe voertuigen in de simulatie vanaf generatoren
     */
    void genereerNieuweVoertuigen();

    /**
     * @brief Genereer een nieuw voertuig op een weg
     * @param baanNaam Wegnaam
     * @param positie Positie
     * @param type Voertuigtype
     * @return true als het voertuig succesvol werd gegenereerd
     */
    bool genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type = "auto");

    /**
     * @brief Verhoog de teller voor verwijderde voertuigen
     */
    void verhoogVerwijderdeVoertuigenTeller();

    /**
     * @brief Verzamel simulatiestatistieken
     */
    void verzamelStatistieken();

    /**
     * @brief Controleer of een voertuig naar een kruispunt kan gaan
     * @param voertuig Te controleren voertuig
     * @param kruispunt Te controleren kruispunt
     * @return true als het voertuig naar een kruispunt moet worden verplaatst, false indien niet
     */
    bool moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const;

    /**
     * @brief Verplaats een voertuig naar een nieuwe weg na een kruispunt
     * @param voertuig Het te verplaatsen voertuig
     * @param kruispunt Het kruispunt waar het voertuig zich bevindt
     * @return true als het voertuig werd verplaatst, false indien niet
     */
    bool verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt);

    /**
     * @brief Controleer of een voertuig bij een bushalte moet stoppen
     * @param voertuig Te controleren voertuig
     * @param halte Te controleren bushalte
     * @return true als de bus bij de bushalte moet stoppen, false indien niet
     */
    bool moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const;

public:
    /**
     * @brief Constructor
     * @param situatie Referentie naar de te simuleren verkeerssituatie
     * @param tijdstap De tijdstap voor de simulatie in seconden
     */
    simulatie(VerkeersSituatie& situatie, double tijdstap);

    /**
     * @brief Voer één simulatiestap uit
     */
    void stap();

    /**
     * @brief Schakel automatische voertuiggeneratie in of uit
     * @param genereer Of voertuigen automatisch moeten worden gegenereerd
     */
    void setAutoGenereerVoertuigen(bool genereer);

    /**
     * @brief Haal de huidige simulatietijd op
     * @return De huidige simulatietijd
     */
    [[nodiscard]] double getHuidigeSimulatieTijd() const;

    /**
     * @brief Haal de tijdstap op
     * @return De tijdstap
     */
    [[nodiscard]] double getTijdstap() const;

    /**
     * @brief Haal het aantal voertuigen op
     * @return Het aantal voertuigen
     */
    [[nodiscard]] int getAantalVoertuigen() const;

    /**
     * @brief Haal de gemiddelde snelheid op
     * @return De gemiddelde snelheid
     */
    [[nodiscard]] double getGemiddeldeSnelheid() const;

    /**
     * @brief Haal het totale aantal verwijderde voertuigen op
     * @return Het totale aantal verwijderde voertuigen
     */
    [[nodiscard]] int getTotaalVerwijderdeVoertuigen() const;

    /**
     * @brief Haal de totale tijd op
     * @return De totale tijd
     */
    [[nodiscard]] double getTotaleTijd() const;


     bool properlyInitialized() const;
};

#endif //SIMULATIE_H