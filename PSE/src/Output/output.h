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
    std::string genereerTekstRapport(const VerkeersSituatie& situatie) const;

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
     * - Verkeerslichten als kleurletters (G=groen, O=oranje, R=rood)
     * - Bushaltes als 'H' en kruispunten als '+'
     * - Automatische schaling voor lange banen
     */
    std::string genereerGrafischeImpressie(const VerkeersSituatie& situatie) const;

    /**
     * @brief Toon een verbeterde grafische impressie op stdout met voertuigdetails
     * @param situatie De weer te geven verkeerssituatie
     * @pre situatie.properlyInitialized() == true
     * @post Grafische impressie en voertuigdetails zijn getoond op stdout
     * @post Verkeerslicht informatie is getoond bij relevante voertuigen
     *
     * Toont de grafische impressie gevolgd door:
     * - Gedetailleerde voertuiginformatie met posities en snelheden
     * - Verkeerslicht status voor voertuigen binnen 200m van lichten
     * - Extra informatie voor slimme verkeerslichten
     * - Prioriteitsvoertuig detectie
     */
    void maakGrafischeImpressie(const VerkeersSituatie& situatie) const;

    /**
     * @brief Toon een tekstrapport op stdout
     * @param situatie De weer te geven verkeerssituatie
     * @pre situatie.properlyInitialized() == true
     * @post Tekstrapport is getoond op stdout
     *
     * Toont een gestructureerd overzicht van alle elementen
     * in de verkeerssituatie op de console.
     */
    void maakTekstRapport(const VerkeersSituatie& situatie) const;

    /**
     * @brief Schrijf de verkeerssituatie naar een XML bestand
     * @param situatie De te schrijven verkeerssituatie
     * @param bestandsnaam Naam van het uit te schrijven bestand
     * @return true als schrijven succesvol was, false anders
     * @pre situatie.properlyInitialized() == true
     * @pre !bestandsnaam.empty()
     * @post Bij success: XML bestand is aangemaakt met volledige verkeerssituatie
     * @post Bij success: return value == true
     * @post Bij faal: getLastFoutmelding() bevat beschrijving van probleem
     * @post Bij faal: return value == false
     *
     * Exporteert de volledige verkeerssituatie naar een XML bestand dat
     * kan worden gebruikt om de situatie later opnieuw in te laden.
     * Bevat alle banen, voertuigen, verkeerslichten, bushaltes en kruispunten.
     */
    bool schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Schrijf de verkeerssituatie naar een HTML bestand
     * @param situatie De te schrijven verkeerssituatie
     * @param bestandsnaam Naam van het uit te schrijven bestand
     * @return true als schrijven succesvol was, false anders
     * @pre situatie.properlyInitialized() == true
     * @pre !bestandsnaam.empty()
     * @post Bij success: HTML bestand is aangemaakt met interactieve visualisatie
     * @post Bij success: return value == true
     * @post Bij faal: getLastFoutmelding() bevat beschrijving van probleem
     * @post Bij faal: return value == false
     *
     * Creëert een HTML bestand met een interactieve visualisatie van
     * de verkeerssituatie. Bevat CSS styling, tooltips en gedetailleerde
     * informatie over alle elementen.
     */
    bool schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Haal de laatste foutmelding op
     * @return String met beschrijving van de laatste fout, lege string als geen fout
     * @pre properlyInitialized() == true
     * @post return waarde is beschrijving van meest recente fout
     *
     * Geeft toegang tot de meest recente foutmelding van write operaties.
     * Lege string betekent geen recente fouten.
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