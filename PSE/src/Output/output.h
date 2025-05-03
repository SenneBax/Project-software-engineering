/**
* @file output.h
 * @brief Header for the output class (Revised with graphical impressions)
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include "../Situation/situatie.h"

/**
 * @brief Class for generating output of traffic situations
 */
class output {
public:
    /**
     * @brief Constructor
     */
    output();

    /**
     * @brief Generates a textual representation of the traffic situation
     * @param situatie The traffic situation to be displayed
     * @return A string with the textual representation
     */
    static std::string genereerTekstRapport(const VerkeersSituatie& situatie);

    /**
     * @brief Generates a graphical impression of the traffic situation (ASCII art)
     * @param situatie The traffic situation to be displayed
     * @return A string with the graphical impression
     */
    static std::string genereerGrafischeImpressie(const VerkeersSituatie& situatie);

    /**
     * @brief Writes a traffic situation to an XML file
     * @param situatie The traffic situation to be written
     * @param bestandsnaam Path to the XML file
     * @return true if writing was successful, false otherwise
     */
    bool schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Generates an HTML representation of the traffic situation
     * @param situatie The traffic situation to be displayed
     * @param bestandsnaam Path to the HTML file
     * @return true if writing was successful, false otherwise
     */
    bool schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const;

    /**
     * @brief Gets the last error message
     * @return The last error message
     */
    std::string getLastFoutmelding() const;

    bool properlyInitialized() const;


private:
    mutable std::string lastFoutmelding; ///< The last error message
    output* _initCheck;
};

#endif // OUTPUT_H