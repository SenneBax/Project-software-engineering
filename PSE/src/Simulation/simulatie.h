/**
 * @file simulatie.h
 * @brief Header voor de simulatie klasse
 */

#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "../Situation/situatie.h"
#include <map>
#include <string>

/**
 * @class simulatie
 * @brief Klasse voor het simuleren van een verkeerssituatie met voertuigen, verkeerslichten en kruispunten
 *
 * Deze klasse implementeert een discrete-time simulatie van verkeerssituaties. De simulatie
 * verwerkt voertuigbewegingen, verkeerslichtcycli, bushalte-interacties en kruispuntlogica.
 * Standaardwaarden voor fysieke parameters zijn gebaseerd op sectie B.6 van de specificatie.
 */
class simulatie {
private:
    VerkeersSituatie& verkeerssituatie; ///< Referentie naar de te simuleren verkeerssituatie
    double tijdstap;                     ///< Tijdstap voor de simulatie in seconden
    double huidigeSimulatieTijd;         ///< Huidige simulatietijd in seconden
    bool autoGenereerVoertuigen;         ///< Of voertuigen automatisch gegenereerd moeten worden

    std::map<std::string, double> lastGenerationTimes; ///< Laatste generatietijd per generator

    // Standaardwaarden uit sectie B.6
    double vertraagAfstand;      /**< Δxs - vertraagafstand in meters */
    double stopAfstand;          /**< Δxs0 - stopafstand in meters */
    double vertraagFactor;       /**< s - vertraagfactor (dimensieloos) */

    // Statistieken
    int aantalVoertuigen;           ///< Huidig aantal voertuigen in de simulatie
    double gemiddeldeSnelheid;      ///< Gemiddelde snelheid van alle voertuigen
    int verwijderdeVoertuigenTeller; ///< Aantal voertuigen verwijderd in laatste stap
    int totaalVerwijderdeVoertuigen; ///< Totaal aantal verwijderde voertuigen
    double totaleTijd;              ///< Totale gesimuleerde tijd

    simulatie* _initCheck; ///< Pointer voor Design by Contract verificatie

    /**
     * @brief Verwerk alle verkeerslichten in de simulatie
     * @pre properlyInitialized() == true
     * @post Alle verkeerslichten zijn bijgewerkt volgens hun cyclustijd
     */
    void verwerkVerkeerslichten();

    /**
     * @brief Controleer of een voertuig een verkeerslicht nadert en bepaal actie
     * @param voertuig Het te controleren voertuig
     * @param verkeerslicht Het te controleren verkeerslicht
     * @return Responscode: 0 = geen effect, 1 = vertragen, 2 = stoppen, 3 = doorrijden
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @pre verkeerslicht.properlyInitialized() == true
     * @post Prioriteitsvoertuigen worden geregistreerd bij slimme verkeerslichten
     */
    int controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht);

    /**
     * @brief Verwerk alle voertuigen in de simulatie
     * @pre properlyInitialized() == true
     * @post Alle voertuigen hebben nieuwe posities en snelheden
     * @post Voertuigen die de baan verlaten zijn verwijderd
     * @post verwijderdeVoertuigenTeller is bijgewerkt
     */
    void verwerkVoertuigen();

    /**
     * @brief Verwerk alle bushaltes in de simulatie
     * @pre properlyInitialized() == true
     * @post Bussen stoppen bij bushaltes en vertrekken na wachttijd
     * @post Bushalte bezettingsstatus is correct bijgewerkt
     */
    void verwerkBushaltes();

    /**
     * @brief Verwerk alle kruispunten in de simulatie
     * @pre properlyInitialized() == true
     * @post Voertuigen bij kruispunten worden verplaatst naar nieuwe banen
     */
    void verwerkKruispunten();

    /**
     * @brief Genereer nieuwe voertuigen vanaf voertuiggeneratoren - VEILIGE VERSIE
     * @pre properlyInitialized() == true
     * @pre autoGenereerVoertuigen == true
     * @post Nieuwe voertuigen zijn toegevoegd volgens generator frequenties
     */
    void genereerNieuweVoertuigen();

    /**
     * @brief Genereer een nieuw voertuig op een specifieke baan
     * @param baanNaam Naam van de baan waarop het voertuig wordt geplaatst
     * @param positie Startpositie op de baan in meters
     * @param type Type voertuig ("auto", "bus", "brandweerwagen", etc.)
     * @return true als het voertuig succesvol werd gegenereerd, false indien niet
     * @pre properlyInitialized() == true
     * @pre !baanNaam.empty()
     * @pre positie >= 0.0
     * @post Bij succes: nieuw voertuig is toegevoegd aan verkeerssituatie
     */
    bool genereertVoertuig(const std::string& baanNaam, double positie, const std::string& type = "auto");

    /**
     * @brief Verhoog de teller voor verwijderde voertuigen
     * @pre properlyInitialized() == true
     * @post verwijderdeVoertuigenTeller is verhoogd
     * @deprecated Deze methode wordt niet meer gebruikt
     */
    void verhoogVerwijderdeVoertuigenTeller();

    /**
     * @brief Verzamel simulatiestatistieken
     * @pre properlyInitialized() == true
     * @post aantalVoertuigen en gemiddeldeSnelheid zijn bijgewerkt
     */
    void verzamelStatistieken();

    /**
     * @brief Controleer of een voertuig naar een kruispunt moet worden verplaatst
     * @param voertuig Te controleren voertuig
     * @param kruispunt Te controleren kruispunt
     * @return true als het voertuig naar een kruispunt moet worden verplaatst, false indien niet
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @pre kruispunt.properlyInitialized() == true
     */
    bool moetNaarKruispunt(const Voertuig& voertuig, const Kruispunt& kruispunt) const;

    /**
     * @brief Verplaats een voertuig naar een nieuwe baan na een kruispunt
     * @param voertuig Het te verplaatsen voertuig
     * @param kruispunt Het kruispunt waar het voertuig zich bevindt
     * @return true als het voertuig werd verplaatst, false indien niet
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @pre kruispunt.properlyInitialized() == true
     * @post Bij succes: voertuig bevindt zich op nieuwe baan
     */
    bool verplaatsVoertuigNaKruispunt(Voertuig& voertuig, const Kruispunt& kruispunt);

    /**
     * @brief Controleer of een voertuig bij een bushalte moet stoppen
     * @param voertuig Te controleren voertuig
     * @param halte Te controleren bushalte
     * @return true als de bus bij de bushalte moet stoppen, false indien niet
     * @pre properlyInitialized() == true
     * @pre voertuig.properlyInitialized() == true
     * @pre halte.properlyInitialized() == true
     * @deprecated Deze methode wordt niet gebruikt in de huidige implementatie
     */
    bool moetStoppenBijHalte(const Voertuig& voertuig, const Bushalte& halte) const;

public:
    /**
     * @brief Constructor voor de simulatieklasse
     * @param situatie Referentie naar de te simuleren verkeerssituatie
     * @param tijdstap De tijdstap voor de simulatie in seconden (standaard 0.0166s voor 60fps)
     * @pre situatie.properlyInitialized() == true
     * @pre tijdstap > 0.0 of tijdstap wordt automatisch ingesteld op 0.0166
     * @post properlyInitialized() == true
     * @post getHuidigeSimulatieTijd() == 0.0
     * @post getTijdstap() == tijdstap (of 0.0166 als tijdstap <= 0)
     */
    simulatie(VerkeersSituatie& situatie, double tijdstap = 0.0166);

    /**
     * @brief Copy constructor - niet toegestaan omdat simulatie een referentie bevat
     */
    simulatie(const simulatie&) = delete;

    /**
     * @brief Assignment operator - niet toegestaan omdat simulatie een referentie bevat
     */
    simulatie& operator=(const simulatie&) = delete;

    /**
     * @brief Move constructor - niet toegestaan omdat simulatie een referentie bevat
     */
    simulatie(simulatie&&) = delete;

    /**
     * @brief Move assignment operator - niet toegestaan omdat simulatie een referentie bevat
     */
    simulatie& operator=(simulatie&&) = delete;

    /**
     * @brief Voer één simulatiestap uit
     * @pre properlyInitialized() == true
     * @post huidigeSimulatieTijd is verhoogd met tijdstap
     * @post alle verkeerselementen zijn bijgewerkt volgens hun regels
     * @post statistieken zijn bijgewerkt
     */
    void stap();

    /**
     * @brief Schakel automatische voertuiggeneratie in of uit
     * @param genereer Of voertuigen automatisch moeten worden gegenereerd
     * @pre properlyInitialized() == true
     * @post autoGenereerVoertuigen == genereer
     */
    void setAutoGenereerVoertuigen(bool genereer);

    /**
     * @brief Haal de huidige simulatietijd op
     * @return De huidige simulatietijd in seconden
     * @pre properlyInitialized() == true
     */
    [[nodiscard]] double getHuidigeSimulatieTijd() const;

    /**
     * @brief Haal de tijdstap op
     * @return De tijdstap in seconden
     * @pre properlyInitialized() == true
     */
    [[nodiscard]] double getTijdstap() const;

    /**
     * @brief Haal het huidige aantal voertuigen op
     * @return Het aantal voertuigen in de simulatie
     * @pre properlyInitialized() == true
     */
    [[nodiscard]] int getAantalVoertuigen() const;

    /**
     * @brief Haal de gemiddelde snelheid van alle voertuigen op
     * @return De gemiddelde snelheid in m/s
     * @pre properlyInitialized() == true
     * @post result >= 0.0
     */
    [[nodiscard]] double getGemiddeldeSnelheid() const;

    /**
     * @brief Haal het totale aantal verwijderde voertuigen op
     * @return Het totale aantal verwijderde voertuigen sinds start simulatie
     * @pre properlyInitialized() == true
     * @post result >= 0
     */
    [[nodiscard]] int getTotaalVerwijderdeVoertuigen() const;

    /**
     * @brief Haal de totale gesimuleerde tijd op
     * @return De totale tijd sinds start van de simulatie in seconden
     * @pre properlyInitialized() == true
     * @post result >= 0.0
     */
    [[nodiscard]] double getTotaleTijd() const;

    /**
     * @brief Controleer of het simulatie object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     */
    bool properlyInitialized() const;
};

#endif //SIMULATIE_H