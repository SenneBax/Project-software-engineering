/**
 * @file bestandslezer.h
 * @brief Header for the BestandsLezer class (Revised with bus stops and intersections)
 */

#ifndef BESTANDSLEZER_H
#define BESTANDSLEZER_H

#include <string>
#include "situatie.h"
#include "tinyxml.h"

/**
 * @brief Class for reading files with traffic situations
 */
class BestandsLezer {
public:
    /**
     * @brief Constructor
     */
    BestandsLezer();

    /**
     * @brief Read a traffic situation from an XML file
     * @param bestandsnaam Path to the XML file
     * @param situatie Reference to the traffic situation where the data will be loaded
     * @return true if reading was successful, false otherwise
     */
    bool leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie);

    /**
     * @brief Get the last error message
     * @return The last error message
     */
    std::string getLastFoutmelding() const;

private:
    std::string lastFoutmelding; ///< The last error message

    /**
     * @brief Process a road element
     * @param elem XML element containing road data
     * @param situatie The traffic situation to add the road to
     * @return true if successful, false otherwise
     */
    bool verwerkBaan(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Process a vehicle element
     * @param elem XML element containing vehicle data
     * @param situatie The traffic situation to add the vehicle to
     * @return true if successful, false otherwise
     */
    bool verwerkVoertuig(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Process a traffic light element
     * @param elem XML element containing traffic light data
     * @param situatie The traffic situation to add the traffic light to
     * @return true if successful, false otherwise
     */
    bool verwerkVerkeerslicht(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Process a vehicle generator element
     * @param elem XML element containing vehicle generator data
     * @param situatie The traffic situation to add the generator to
     * @return true if successful, false otherwise
     */
    bool verwerkVoertuigGenerator(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Process a bus stop element
     * @param elem XML element containing bus stop data
     * @param situatie The traffic situation to add the bus stop to
     * @return true if successful, false otherwise
     */
    bool verwerkBushalte(TiXmlElement* elem, VerkeersSituatie& situatie);

    /**
     * @brief Process an intersection element
     * @param elem XML element containing intersection data
     * @param situatie The traffic situation to add the intersection to
     * @return true if successful, false otherwise
     */
    bool verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie);
};

#endif // BESTANDSLEZER_H