#ifndef SIMULATIE_H
#define SIMULATIE_H

#include "../Situation/situatie.h"

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
    bool autoGenereerVoertuigen = false; ///< Of voertuigen automatisch gegenereerd moeten worden

    // Standaardwaarden uit sectie B.6
    double vertraagAfstand;      /**< Δxs - vertraagafstand in meters */
    double stopAfstand;          /**< Δxs0 - stopafstand in meters */
    double vertraagFactor;       /**< s - vertraagfactor (dimensieloos) */

    // Statistieken
    int aantalVoertuigen = 0;           ///< Huidig aantal voertuigen in de simulatie
    double gemiddeldeSnelheid = 0.0;    ///< Gemiddelde snelheid van alle voertuigen
    int verwijderdeVoertuigenTeller = 0; ///< Aantal voertuigen verwijderd in laatste stap
    int totaalVerwijderdeVoertuigen = 0; ///< Totaal aantal verwijderde voertuigen
    double totaleTijd = 0.0;            ///< Totale gesimuleerde tijd

    simulatie* _initCheck; ///< Pointer voor Design by Contract verificatie

    /**
     * @brief Verwerk alle verkeerslichten in de simulatie
     * @pre properlyInitialized() == true
     * @post Alle verkeerslichten zijn bijgewerkt volgens hun cyclustijd
     *
     * Deze methode update de status van alle verkeerslichten op basis van hun
     * cyclustijd en huidige toestand (groen/oranje/rood).
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
     *
     * Controleert de afstand tussen voertuig en verkeerslicht en bepaalt de vereiste actie
     * op basis van de verkeerslichtkleur en voertuigtype (prioriteit).
     */
    int controleerVerkeerslichtNadering(Voertuig& voertuig, const Verkeerslicht& verkeerslicht);

    /**
     * @brief Verwerk alle voertuigen in de simulatie
     * @pre properlyInitialized() == true
     * @post Alle voertuigen hebben nieuwe posities en snelheden
     * @post Voertuigen die de baan verlaten zijn verwijderd
     * @post verwijderdeVoertuigenTeller is bijgewerkt
     *
     * Deze methode:
     * - Sorteert voertuigen per baan van hoog naar laag positie
     * - Berekent versnelling gebaseerd op voorligger en verkeerslichten
     * - Update posities en snelheden
     * - Verwijdert voertuigen die het einde van de baan bereiken
     */
    void verwerkVoertuigen();

    /**
     * @brief Verwerk alle bushaltes in de simulatie
     * @pre properlyInitialized() == true
     * @post Bussen stoppen bij bushaltes en vertrekken na wachttijd
     * @post Bushalte bezettingsstatus is correct bijgewerkt
     *
     * Beheert bus-bushalte interacties:
     * - Detecteert bussen die bij haltes aankomen
     * - Beheert wachttijden voor geparkeerde bussen
     * - Laat bussen vertrekken na voltooide wachttijd
     */
    void verwerkBushaltes();

    /**
     * @brief Verwerk alle kruispunten in de simulatie
     * @pre properlyInitialized() == true
     * @post Voertuigen bij kruispunten worden verplaatst naar nieuwe banen
     *
     * Detecteert voertuigen die kruispunten bereiken en verplaatst ze
     * willekeurig naar één van de aangesloten banen.
     */
    void verwerkKruispunten();

    /**
     * @brief Genereer nieuwe voertuigen vanaf voertuiggeneratoren
     * @pre properlyInitialized() == true
     * @pre autoGenereerVoertuigen == true
     * @post Nieuwe voertuigen zijn toegevoegd volgens generator frequenties
     *
     * Controleert alle voertuiggeneratoren en genereert nieuwe voertuigen
     * wanneer de frequentietijd is verstreken.
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
     *
     * Controleert of de baan bestaat en er geen voertuig te dichtbij staat
     * voordat een nieuw voertuig wordt geplaatst.
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
     *
     * Berekent het huidige aantal voertuigen en de gemiddelde snelheid
     * van alle voertuigen in de simulatie.
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
     *
     * Controleert of het voertuig zich op een baan van het kruispunt bevindt
     * en de kruispuntpositie heeft bereikt (binnen 0.5m marge).
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
     *
     * Kiest willekeurig een nieuwe baan vanuit het kruispunt (behalve huidige baan)
     * en plaatst het voertuig net na het kruispunt op die baan.
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
     *
     * Initialiseert de simulatie met standaardparameters:
     * - vertraagAfstand = 250.0m
     * - stopAfstand = 50.0m
     * - vertraagFactor = 0.4
     */
    simulatie(VerkeersSituatie& situatie, double tijdstap);

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
     *
     * Voert een volledige simulatiestap uit in deze volgorde:
     * 1. Update verkeerslichten
     * 2. Verwerk voertuigen (beweging, versnelling, verwijdering)
     * 3. Verwerk bushaltes (bus stops en vertrek)
     * 4. Verwerk kruispunten (voertuigverplaatsing)
     * 5. Genereer nieuwe voertuigen (indien ingeschakeld)
     * 6. Verzamel statistieken
     */
    void stap();

    /**
     * @brief Schakel automatische voertuiggeneratie in of uit
     * @param genereer Of voertuigen automatisch moeten worden gegenereerd
     * @pre properlyInitialized() == true
     * @post autoGenereerVoertuigen == genereer
     *
     * Wanneer ingeschakeld, worden nieuwe voertuigen automatisch gegenereerd
     * op basis van de voertuiggeneratoren in de verkeerssituatie.
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
     *
     * Berekent het gemiddelde van de snelheden van alle voertuigen.
     * Retourneert 0.0 als er geen voertuigen zijn.
     */
    [[nodiscard]] double getGemiddeldeSnelheid() const;

    /**
     * @brief Haal het totale aantal verwijderde voertuigen op
     * @return Het totale aantal verwijderde voertuigen sinds start simulatie
     * @pre properlyInitialized() == true
     * @post result >= 0
     *
     * Voertuigen worden verwijderd wanneer ze het einde van een baan bereiken.
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
     *
     * Design by Contract verificatie methode.
     */
    bool properlyInitialized() const;
};

#endif //SIMULATIE_H