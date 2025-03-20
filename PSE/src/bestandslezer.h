/**
* @file bestandslezer.h
 * @author senne
 * @date 19/03/2025
 * @brief Header voor de BestandsLezer klasse
 */

#ifndef BESTANDSLEZER_H
#define BESTANDSLEZER_H

#include <string>
#include "situatie.h"

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
     * @return true als het inlezen succesvol was, anders false
     */
    bool leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie);

    /**
     * @brief Geeft de laatste foutmelding terug
     * @return De laatste foutmelding
     */
    std::string getLastFoutmelding() const;

private:
    std::string lastFoutmelding; ///< De laatste foutmelding
};

#endif // BESTANDSLEZER_H
