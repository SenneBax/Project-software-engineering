/**
 * @file situatie.h
 * @brief Header voor de VerkeersSituatie klasse (Herzien met bushaltes, kruispunten en polymorfische voertuigen)
 */

#ifndef SITUATIE_H
#define SITUATIE_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../TraficObjects/voertuiggenerator.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"

/**
 * @brief Klasse die een verkeerssituatie voorstelt
 */
class VerkeersSituatie {
public:
    /**
     * @brief Constructor
     */
    VerkeersSituatie();

    /**
     * @brief Copy constructor - deleted omdat we unieke pointers gebruiken
     */
    VerkeersSituatie(const VerkeersSituatie&) = delete;

    /**
     * @brief Assignment operator - deleted omdat we unieke pointers gebruiken
     */
    VerkeersSituatie& operator=(const VerkeersSituatie&) = delete;

    /**
     * @brief Move constructor
     */
    VerkeersSituatie(VerkeersSituatie&&) = default;

    /**
     * @brief Move assignment operator
     */
    VerkeersSituatie& operator=(VerkeersSituatie&&) = default;

    /**
     * @brief Destructor
     */
    ~VerkeersSituatie();

    /**
     * @brief Voeg een weg toe aan de verkeerssituatie
     * @param baan De toe te voegen weg
     * @return true als de weg succesvol is toegevoegd, false indien niet
     */
    bool voegBaanToe(const Baan& baan);

    /**
     * @brief Voeg een voertuig toe aan de verkeerssituatie
     * @param voertuig Het toe te voegen voertuig
     * @return true als het voertuig succesvol is toegevoegd, false indien niet
     */
    bool voegVoertuigToe(const Voertuig& voertuig);

    /**
     * @brief Voeg een voertuig toe aan de verkeerssituatie
     * @param voertuig Unieke pointer naar het toe te voegen voertuig
     * @return true als het voertuig succesvol is toegevoegd, false indien niet
     */
    bool voegVoertuigToe(std::unique_ptr<Voertuig> voertuig);

    /**
     * @brief Voeg een verkeerslicht toe aan de verkeerssituatie
     * @param licht Het toe te voegen verkeerslicht
     * @return true als het verkeerslicht succesvol is toegevoegd, false indien niet
     */
    bool voegVerkeerslichtToe(const Verkeerslicht& licht);

    /**
     * @brief Voeg een voertuiggenerator toe aan de verkeerssituatie
     * @param generator De toe te voegen generator
     * @return true als de generator succesvol is toegevoegd, false indien niet
     */
    bool voegVoertuigGeneratorToe(const VoertuigGenerator& generator);

    /**
     * @brief Voeg een bushalte toe aan de verkeerssituatie
     * @param bushalte De toe te voegen bushalte
     * @return true als de bushalte succesvol is toegevoegd, false indien niet
     */
    bool voegBushalteToe(const Bushalte& bushalte);

    /**
     * @brief Voeg een kruispunt toe aan de verkeerssituatie
     * @param kruispunt Het toe te voegen kruispunt
     * @return true als het kruispunt succesvol is toegevoegd, false indien niet
     */
    bool voegKruispuntToe(const Kruispunt& kruispunt);

    /**
     * @brief Verifieer de consistentie van de verkeerssituatie
     * @return true als de verkeerssituatie consistent is, false indien niet
     */
    bool verificeerConsistentie() const;

    /**
     * @brief Verwijder een voertuig uit de simulatie
     * @param index Index van het te verwijderen voertuig
     * @return true als het voertuig succesvol is verwijderd, false indien niet
     */
    bool verwijderVoertuig(int index);

    /**
     * @brief Haal een map van alle wegen op
     * @return Map met wegnamen als sleutels en wegobjecten als waarden
     */
    const std::map<std::string, Baan>& getBanen() const;

    /**
     * @brief Haal een vector met kopieën van alle voertuigen op
     * @return Vector met kopieën van alle voertuigen
     */
    std::vector<std::unique_ptr<Voertuig>> kopieVoertuigen() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle voertuigen op
     * @return Vector met alle voertuigen
     */
    std::vector<std::unique_ptr<Voertuig>>& getVoertuigen();

    /**
     * @brief Haal een constante referentie naar alle voertuigen op
     * @return Vector met alle voertuigen
     */
    const std::vector<std::unique_ptr<Voertuig>>& getVoertuigen() const;

    /**
     * @brief Geef het aantal voertuigen in de situatie
     * @return Aantal voertuigen
     */
    size_t getAantalVoertuigen() const;

    /**
     * @brief Haal alle verkeerslichten op
     * @return Vector met alle verkeerslichten
     */
    const std::vector<Verkeerslicht>& getVerkeerslichten() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle verkeerslichten op
     * @return Vector met alle verkeerslichten
     */
    std::vector<Verkeerslicht>& getVerkeerslichten();

    /**
     * @brief Haal alle voertuiggeneratoren op
     * @return Vector met alle voertuiggeneratoren
     */
    const std::vector<VoertuigGenerator>& getVoertuigGenerators() const;

    /**
     * @brief Haal alle bushaltes op
     * @return Vector met alle bushaltes
     */
    const std::vector<Bushalte>& getBushaltes() const;

    /**
     * @brief Haal een aanpasbare referentie naar alle bushaltes op
     * @return Vector met alle bushaltes
     */
    std::vector<Bushalte>& getBushaltes();

    /**
     * @brief Haal alle kruispunten op
     * @return Vector met alle kruispunten
     */
    const std::vector<Kruispunt>& getKruispunten() const;

    /**
     * @brief Zoek bushaltes op een specifieke weg
     * @param baanNaam Naam van de weg
     * @return Vector met pointers naar bushaltes op de weg
     */
    std::vector<Bushalte*> zoekBushaltesOpBaan(const std::string& baanNaam);

    /**
     * @brief Zoek de volgende bushalte voor een voertuig
     * @param voertuig Het voertuig waarvoor de volgende bushalte wordt gezocht
     * @return Pointer naar de volgende bushalte, nullptr indien geen
     */
    Bushalte* zoekEerstvolgendeHalte(const Voertuig& voertuig);

    /**
     * @brief Zoek het volgende verkeerslicht voor een voertuig
     * @param voertuig Het voertuig waarvoor het volgende verkeerslicht wordt gezocht
     * @return Pointer naar het volgende verkeerslicht, nullptr indien geen
     */
    Verkeerslicht* zoekEerstvolgendeVerkeerslicht(const Voertuig& voertuig);

    /**
     * @brief Zoek kruispunten die een specifieke weg bevatten
     * @param baanNaam Naam van de weg
     * @return Vector met pointers naar kruispunten die de weg bevatten
     */
    std::vector<Kruispunt*> zoekKruispuntenOpBaan(const std::string& baanNaam);

    /**
     * @brief Zoek het volgende kruispunt voor een voertuig
     * @param voertuig Het voertuig waarvoor het volgende kruispunt wordt gezocht
     * @return Pointer naar het volgende kruispunt, nullptr indien geen
     */
    Kruispunt* zoekEerstvolgendeKruispunt(const Voertuig& voertuig);

    bool properlyInitialized() const;

private:
    std::map<std::string, Baan> banen;        ///< De wegen in de verkeerssituatie, geïndexeerd op naam
    std::vector<std::unique_ptr<Voertuig>> voertuigen;  ///< De voertuigen in de verkeerssituatie
    std::vector<Verkeerslicht> verkeerslichten; ///< De verkeerslichten in de verkeerssituatie
    std::vector<VoertuigGenerator> generators;  ///< De voertuiggeneratoren in de verkeerssituatie
    std::vector<Bushalte> bushaltes;          ///< De bushaltes in de verkeerssituatie
    std::vector<Kruispunt> kruispunten;       ///< De kruispunten in de verkeerssituatie
    VerkeersSituatie* _initCheck;
};

#endif // SITUATIE_H