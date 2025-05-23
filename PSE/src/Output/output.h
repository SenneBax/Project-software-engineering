#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include "../Situation/situatie.h"

/**
 * @brief Klasse voor het genereren van uitvoer van verkeerssituaties
 *
 * De output klasse biedt verschillende methoden om verkeerssituaties
 * te visualiseren en te exporteren. Ondersteunde formaten zijn:
 * - Tekstuele rapporten met statistieken
 * - ASCII-art grafische impressies
 * - XML export voor persistentie
 * - HTML visualisaties voor webbrowsers
 */
class output {
public:
    /**
     * @brief Constructor voor output klasse
     * @pre Geen specifieke precondities
     * @post properlyInitialized() == true
     * @post getLastFoutmelding() == ""
     *
     * Initialiseert een output object in een geldige toestand.
     */
    output();

    /**
     * @brief Genereert een tekstuele weergave van de verkeerssituatie
     * @param situatie De weer te geven verkeerssituatie
     * @return Een string met de tekstuele weergave inclusief statistieken
     * @pre situatie.properlyInitialized() == true
     * @post return waarde bevat volledige situatiebeschrijving
     * @post return waarde is niet leeg voor geldige situaties
     *
     * Genereert een gestructureerd tekstrapport met:
     * - Overzicht van alle banen met lengtes
     * - Lijst van voertuigen met posities en types
     * - Verkeerslichten met status en cyclustijden
     * - Bushaltes met wachttijden
     * - Kruispunten met verbonden banen
     */
    std::string genereerTekstRapport(const VerkeersSituatie& situatie);

    /**
     * @brief Genereert een grafische impressie van de verkeerssituatie (ASCII-art)
     * @param situatie De weer te geven verkeerssituatie
     * @return Een string met de grafische impressie inclusief legende
     * @pre situatie.properlyInitialized() == true
     * @post return waarde bevat ASCII-art visualisatie
     * @post return waarde bevat legende voor symbolen
     * @post banen worden geschaald naar maximaal 80 karakters breedte
     *
     * Creëert een visuele representatie waarbij:
     * - Banen worden weergegeven als horizontale lijnen (=)
     * - Voertuigen als letters (A=auto, B=bus, F=brandweer, Z=ziekenwagen, P=politie)
     * - Verkeerslichten als G/O/R (groen/oranje/rood)
     * - Bushaltes als B symbolen
     * - Kruispunten als + symbolen
     * - Lange banen worden automatisch geschaald
     */
    std::string genereerGrafischeImpressie(const VerkeersSituatie& situatie);

    /**
     * @brief Schrijft een verkeerssituatie naar een XML-bestand
     * @param situatie De te schrijven verkeerssituatie
     * @param bestandsnaam Pad naar het XML-bestand
     * @return true als het schrijven succesvol was, false indien niet
     * @pre situatie.properlyInitialized() == true
     * @pre !bestandsnaam.empty()
     * @pre bestandsnaam heeft geldige bestandsextensie (.xml aanbevolen)
     * @post Bij success: XML-bestand is aangemaakt met volledige situatiedata
     * @post Bij faal: getLastFoutmelding() bevat foutbeschrijving
     * @post XML-bestand is geldig en kan weer ingelezen worden
     *
     * Exporteert alle elementen van de verkeerssituatie naar XML:
     * - BAAN elementen met naam en lengte
     * - VOERTUIG elementen met baan, positie en type
     * - VERKEERSLICHT elementen met cyclus, oranje en slim attributen
     * - BUSHALTE elementen met wachttijden
     * - VOERTUIGGENERATOR elementen met frequenties
     * - KRUISPUNT elementen met baan-positie combinaties
     */
    bool schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Genereert een HTML-weergave van de verkeerssituatie
     * @param situatie De weer te geven verkeerssituatie
     * @param bestandsnaam Pad naar het HTML-bestand
     * @return true als het schrijven succesvol was, false indien niet
     * @pre situatie.properlyInitialized() == true
     * @pre !bestandsnaam.empty()
     * @pre bestandsnaam heeft geldige bestandsextensie (.html aanbevolen)
     * @post Bij success: HTML-bestand is aangemaakt met interactieve visualisatie
     * @post Bij faal: getLastFoutmelding() bevat foutbeschrijving
     * @post HTML-bestand bevat CSS styling en tooltips
     * @post HTML-bestand kan geopend worden in webbrowsers
     *
     * Creëert een HTML-pagina met:
     * - CSS-gestylede weergave van banen als gekleurde balken
     * - Voertuigen als gekleurde cirkels met letters
     * - Verkeerslichten als gekleurde rechthoeken
     * - Tooltips met detailinformatie bij hover
     * - Legende met uitleg van alle symbolen
     * - Statistieken sectie met tellingen
     * - Responsieve layout die schaalt naar 1000px breedte
     */
    bool schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Haalt de laatste foutmelding op
     * @return De laatste foutmelding als string
     * @pre properlyInitialized() == true
     * @post return waarde bevat beschrijving van laatste fout (kan leeg zijn)
     *
     * Geeft toegang tot de meest recente foutmelding die optrad
     * tijdens XML of HTML schrijfoperaties. Lege string betekent
     * geen recente fouten.
     */
    std::string getLastFoutmelding() const;

    /**
     * @brief Controleer of het output object correct is geïnitialiseerd
     * @return true als het object correct is geïnitialiseerd, false anders
     * @post return waarde geeft aan of object in geldige toestand verkeert
     *
     * Design by Contract verificatie methode om object integriteit
     * te controleren.
     */
    bool properlyInitialized() const;

private:
    mutable std::string lastFoutmelding; ///< De laatste foutmelding van write operaties
    output* _initCheck; ///< Pointer voor Design by Contract verificatie
};

#endif // OUTPUT_H