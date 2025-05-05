/**
* @file output.h
 * @brief Header voor de output klasse (Herzien met grafische impressies)
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include "../Situation/situatie.h"

/**
 * @brief Klasse voor het genereren van uitvoer van verkeerssituaties
 */
class output {
public:
    /**
     * @brief Constructor
     */
    output();

    /**
     * @brief Genereert een tekstuele weergave van de verkeerssituatie
     * @param situatie De weer te geven verkeerssituatie
     * @return Een string met de tekstuele weergave
     */
    static std::string genereerTekstRapport(const VerkeersSituatie& situatie);

    /**
     * @brief Genereert een grafische impressie van de verkeerssituatie (ASCII-art)
     * @param situatie De weer te geven verkeerssituatie
     * @return Een string met de grafische impressie
     */
    static std::string genereerGrafischeImpressie(const VerkeersSituatie& situatie);

    /**
     * @brief Schrijft een verkeerssituatie naar een XML-bestand
     * @param situatie De te schrijven verkeerssituatie
     * @param bestandsnaam Pad naar het XML-bestand
     * @return true als het schrijven succesvol was, false indien niet
     */
    bool schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Genereert een HTML-weergave van de verkeerssituatie
     * @param situatie De weer te geven verkeerssituatie
     * @param bestandsnaam Pad naar het HTML-bestand
     * @return true als het schrijven succesvol was, false indien niet
     */
    bool schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Haalt de laatste foutmelding op
     * @return De laatste foutmelding
     */
    std::string getLastFoutmelding() const;

    bool properlyInitialized() const;


private:
    mutable std::string lastFoutmelding; ///< De laatste foutmelding
    output* _initCheck;
};

#endif // OUTPUT_H