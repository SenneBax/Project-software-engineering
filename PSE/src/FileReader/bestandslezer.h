#ifndef BESTANDSLEZER_H
#define BESTANDSLEZER_H

#include <string>
#include "../Situation/situatie.h"
#include "../Parser/tinyxml.h"

/**
 * @brief Klasse voor het lezen van bestanden met verkeerssituaties
 *
 * De BestandsLezer klasse gebruikt TinyXML om XML-bestanden te parsen
 * en verkeerssituaties op te bouwen. De klasse is tolerant voor verschillende
 * XML-formaten en kan omgaan met zowel gewone elementen als elementen
 * gewikkeld in een VerkeersSituatie root element.
 */
class BestandsLezer {
public:
    /**
     * @brief Constructor voor BestandsLezer klasse
     * @pre Geen specifieke precondities
     * @post properlyInitialized() == true
     * @post getLastFoutmelding() == ""
     *
     * Initialiseert een BestandsLezer object in een geldige toestand
     * klaar voor het lezen van XML-bestanden.
     */
    BestandsLezer();

    /**
     * @brief Lees een verkeerssituatie uit een XML-bestand
     * @param bestandsnaam Pad naar het XML-bestand
     * @param situatie Referentie naar de verkeerssituatie waarin de gegevens worden geladen
     * @return true als het lezen succesvol was, false indien niet
     * @pre properlyInitialized() == true
     * @pre !bestandsnaam.empty()
     * @pre situatie.properlyInitialized() == true
     * @pre XML-bestand moet bestaan en leesbaar zijn
     * @post Bij success: situatie bevat alle data uit XML-bestand
     * @post Bij success: situatie.verificeerConsistentie() == true
     * @post Bij faal: getLastFoutmelding() bevat foutbeschrijving
     * @post situatie blijft in geldige toestand (mogelijk leeg bij faal)
     *
     * Deze methode:
     * - Controleert bestandstoegang
     * - Detecteert en corrigeert ontbrekende root elementen
     * - Parseert alle ondersteunde XML-elementen
     * - Valideert de consistentie van de geladen data
     * - Ondersteunt zowel gewikkelde als ongewikkelde XML-formaten
     */
    bool leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie);

    /**
     * @brief Haal de laatste foutmelding op
     * @return De laatste foutmelding als string
     * @pre properlyInitialized() == true
     * @post return waarde bevat beschrijving van laatste fout (kan leeg zijn)
     *
     * Geeft toegang tot de meest recente foutmelding die optrad tijdens
     * het lezen en parsen van XML-bestanden. Lege string betekent geen
     * recente fouten.
     */
    std::string getLastFoutmelding() const;

    /**
     * @brief Controleer of het BestandsLezer object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;

private:
    std::string lastFoutmelding; ///< De laatste foutmelding van parse operaties
    BestandsLezer* _initCheck; ///< Pointer voor Design by Contract verificatie

    /**
     * @brief Verwerk XML-elementen en voeg ze toe aan de verkeerssituatie
     * @param root Hoofdelement van het XML-document
     * @param situatie De verkeerssituatie waaraan de elementen worden toegevoegd
     * @return true als de verwerking succesvol was, false indien niet
     * @pre properlyInitialized() == true
     * @pre root != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Alle geldige XML-elementen zijn toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     *
     * Doorloopt alle child-elementen van root en delegeert parsing
     * naar specifieke verwerkingsmethoden per elementtype.
     */
    bool processXmlElements(TiXmlElement* root, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een wegelement (BAAN)
     * @param elem XML-element met weggegevens
     * @param situatie De verkeerssituatie waaraan de weg wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuwe baan is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Baan heeft geldige naam (niet leeg) en positieve lengte
     *
     * Verwacht XML-structuur:
     * <BAAN>
     *   <naam>baannaam</naam>
     *   <lengte>positief_getal</lengte>
     * </BAAN>
     */
    bool verwerkBaan(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een voertuigelement (VOERTUIG)
     * @param elem XML-element met voertuiggegevens
     * @param situatie De verkeerssituatie waaraan het voertuig wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuw voertuig is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Voertuig bevindt zich op bestaande baan binnen baanlengte
     *
     * Verwacht XML-structuur:
     * <VOERTUIG>
     *   <baan>bestaande_baannaam</baan>
     *   <positie>geldige_positie</positie>
     *   <type>voertuigtype</type> <!-- optioneel, default "auto" -->
     * </VOERTUIG>
     */
    bool verwerkVoertuig(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een verkeerslicht-element (VERKEERSLICHT)
     * @param elem XML-element met verkeerslichtgegevens
     * @param situatie De verkeerssituatie waaraan het verkeerslicht wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuw verkeerslicht is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Verkeerslicht heeft positieve cyclustijd en geldige positie
     * @post Verkeerslicht staat niet binnen 50m van ander verkeerslicht
     *
     * Verwacht XML-structuur:
     * <VERKEERSLICHT>
     *   <baan>bestaande_baannaam</baan>
     *   <positie>geldige_positie</positie>
     *   <cyclus>positief_getal</cyclus>
     *   <oranje>true/false</oranje> <!-- optioneel -->
     *   <slim>true/false</slim> <!-- optioneel -->
     * </VERKEERSLICHT>
     */
    bool verwerkVerkeerslicht(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een voertuiggenerator-element (VOERTUIGGENERATOR)
     * @param elem XML-element met voertuiggeneratorgegevens
     * @param situatie De verkeerssituatie waaraan de generator wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuwe generator is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Generator heeft positieve frequentie en bestaande baan
     *
     * Verwacht XML-structuur:
     * <VOERTUIGGENERATOR>
     *   <baan>bestaande_baannaam</baan>
     *   <frequentie>positief_getal</frequentie>
     *   <type>voertuigtype</type> <!-- optioneel, default "auto" -->
     * </VOERTUIGGENERATOR>
     */
    bool verwerkVoertuigGenerator(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een bushalte-element (BUSHALTE)
     * @param elem XML-element met bushaltegegevens
     * @param situatie De verkeerssituatie waaraan de bushalte wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuwe bushalte is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Bushalte heeft positieve wachttijd en geldige positie
     *
     * Verwacht XML-structuur:
     * <BUSHALTE>
     *   <baan>bestaande_baannaam</baan>
     *   <positie>geldige_positie</positie>
     *   <wachttijd>positief_getal</wachttijd>
     * </BUSHALTE>
     */
    bool verwerkBushalte(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Verwerk een kruispunt-element (KRUISPUNT)
     * @param elem XML-element met kruispuntgegevens
     * @param situatie De verkeerssituatie waaraan het kruispunt wordt toegevoegd
     * @return true als succesvol, false indien niet
     * @pre properlyInitialized() == true
     * @pre elem != nullptr
     * @pre situatie.properlyInitialized() == true
     * @post Bij success: nieuw kruispunt is toegevoegd aan situatie
     * @post Bij faal: lastFoutmelding bevat beschrijving van probleem
     * @post Kruispunt bevat minimaal één geldige baan
     * @post Alle banen in kruispunt bestaan en hebben geldige posities
     *
     * Verwacht XML-structuur:
     * <KRUISPUNT>
     *   <baan positie="getal">bestaande_baannaam1</baan>
     *   <baan positie="getal">bestaande_baannaam2</baan>
     *   <!-- meer banen mogelijk -->
     * </KRUISPUNT>
     */
    bool verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie);
};

#endif // BESTANDSLEZER_H