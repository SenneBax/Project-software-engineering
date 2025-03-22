/**
* @file output.h
 * @author senne
 * @date 19/03/2025
 * @brief Header voor de Uitvoer klasse
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include "situatie.h"

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
     * @param situatie De verkeerssituatie die moet worden weergegeven
     * @return Een string met de tekstuele weergave
     */
    static std::string genereerTekstRapport(const VerkeersSituatie& situatie) ;

    /**
     * @brief Schrijft een verkeerssituatie naar een XML-bestand
     * @param situatie De verkeerssituatie die moet worden geschreven
     * @param bestandsnaam Pad naar het XML-bestand
     * @return true als het schrijven succesvol was, anders false
     */
    bool schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Geeft de laatste foutmelding terug
     * @return De laatste foutmelding
     */
    std::string getLastFoutmelding() const;

private:
    mutable std::string lastFoutmelding; ///< De laatste foutmelding
};

#endif // OUTPUT_H