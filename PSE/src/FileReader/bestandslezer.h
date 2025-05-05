/**
 * @file bestandslezer.h
 * @brief Header voor de BestandsLezer klasse (Herzien met bushaltes en kruispunten)
 */

#ifndef BESTANDSLEZER_H
#define BESTANDSLEZER_H

#include <string>
#include "../Situation/situatie.h"
#include "../Parser/tinyxml.h"

/**
 * @brief Klasse voor het lezen van bestanden met verkeerssituaties
 */
class BestandsLezer {
public:
    /**
     * @brief Constructor
     */
    BestandsLezer();

    /**
     * @brief Lees een verkeerssituatie uit een XML-bestand
     * @param bestandsnaam Pad naar het XML-bestand
     * @param situatie Referentie naar de verkeerssituatie waarin de gegevens worden geladen
     * @return true als het lezen succesvol was, false indien niet
     */
    bool leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie);

    /**
     * @brief Haal de laatste foutmelding op
     * @return De laatste foutmelding
     */
    std::string getLastFoutmelding() const;

    bool properlyInitialized() const;


private:
    std::string lastFoutmelding; ///< De laatste foutmelding
    BestandsLezer* _initCheck;

    /**
     * @brief Verwerk XML-elementen en voeg ze toe aan de verkeerssituatie
     * @param root Hoofdelement van het XML-document
     * @param situatie De verkeerssituatie waaraan de elementen worden toegevoegd
     * @return true als de verwerking succesvol was, false indien niet
     */
    bool processXmlElements(TiXmlElement* root, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een wegelement
     * @param elem XML-element met weggegevens
     * @param situatie De verkeerssituatie waaraan de weg wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkBaan(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een voertuigelement
     * @param elem XML-element met voertuiggegevens
     * @param situatie De verkeerssituatie waaraan het voertuig wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkVoertuig(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een verkeerslicht-element
     * @param elem XML-element met verkeerslichtgegevens
     * @param situatie De verkeerssituatie waaraan het verkeerslicht wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkVerkeerslicht(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een voertuiggenerator-element
     * @param elem XML-element met voertuiggeneratorgegevens
     * @param situatie De verkeerssituatie waaraan de generator wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkVoertuigGenerator(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een bushalte-element
     * @param elem XML-element met bushaltegegevens
     * @param situatie De verkeerssituatie waaraan de bushalte wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkBushalte(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een kruispunt-element
     * @param elem XML-element met kruispuntgegevens
     * @param situatie De verkeerssituatie waaraan het kruispunt wordt toegevoegd
     * @return true als succesvol, false indien niet
     */
    bool verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie);
};

#endif // BESTANDSLEZER_H