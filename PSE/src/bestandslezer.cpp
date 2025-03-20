/**
 * @file bestandslezer.cpp
 * @author Claude
 * @date 19/03/2025
 * @brief Implementatie van de BestandsLezer klasse
 */

#include "bestandslezer.h"
#include <fstream>
#include <algorithm>
#include "tinyxml.h"

/**
 * @brief Constructor
 */
BestandsLezer::BestandsLezer() : lastFoutmelding("") {}

/**
 * @brief Lees een verkeerssituatie uit een XML-bestand
 * @param bestandsnaam Pad naar het XML-bestand
 * @param situatie Referentie naar de verkeerssituatie waarin de gegevens worden geladen
 * @return true als het inlezen succesvol was, anders false
 */
bool BestandsLezer::leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie) {
    TiXmlDocument doc;
    if (!doc.LoadFile(bestandsnaam.c_str())) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        lastFoutmelding = "XML-bestand heeft geen root element";
        return false;
    }

    std::string rootName = root->Value();
    if (rootName != "VerkeersSituatie" && rootName != "Verkeerssituatie") {
        lastFoutmelding = "Root element is geen VerkeersSituatie maar: " + rootName;
        return false;
    }

    bool success = true;

    for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            TiXmlElement* naamElem = elem->FirstChildElement("naam");
            TiXmlElement* lengteElem = elem->FirstChildElement("lengte");

            if (!naamElem || !lengteElem || !naamElem->GetText() || !lengteElem->GetText()) {
                lastFoutmelding = "Baan mist verplichte elementen (naam of lengte)";
                success = false;
                continue;
            }

            std::string naam = naamElem->GetText();
            double lengte;
            try {
                lengte = std::stod(lengteElem->GetText());
            } catch (const std::exception&) {
                lastFoutmelding = "Ongeldige lengte voor baan '" + naam + "'";
                success = false;
                continue;
            }

            if (lengte <= 0) {
                lastFoutmelding = "Lengte van baan '" + naam + "' moet positief zijn";
                success = false;
                continue;
            }

            Baan baan(naam, lengte);
            situatie.voegBaanToe(baan);
        }
        else if (elementType == "VOERTUIG") {
            TiXmlElement* baanElem = elem->FirstChildElement("baan");
            TiXmlElement* positieElem = elem->FirstChildElement("positie");

            if (!baanElem || !positieElem || !baanElem->GetText() || !positieElem->GetText()) {
                lastFoutmelding = "Voertuig mist verplichte elementen (baan of positie)";
                success = false;
                continue;
            }

            std::string baan = baanElem->GetText();
            double positie;
            try {
                positie = std::stod(positieElem->GetText());
            } catch (const std::exception&) {
                lastFoutmelding = "Ongeldige positie voor voertuig op baan '" + baan + "'";
                success = false;
                continue;
            }

            Voertuig voertuig(baan, positie);
            if (!situatie.voegVoertuigToe(voertuig)) {
                lastFoutmelding = "Kan voertuig niet toevoegen aan baan '" + baan + "'";
                success = false;
            }
        }
        else if (elementType == "VERKEERSLICHT") {
            TiXmlElement* baanElem = elem->FirstChildElement("baan");
            TiXmlElement* positieElem = elem->FirstChildElement("positie");
            TiXmlElement* cyclusElem = elem->FirstChildElement("cyclus");

            if (!baanElem || !positieElem || !cyclusElem ||
                !baanElem->GetText() || !positieElem->GetText() || !cyclusElem->GetText()) {
                lastFoutmelding = "Verkeerslicht mist verplichte elementen (baan, positie of cyclus)";
                success = false;
                continue;
            }

            std::string baan = baanElem->GetText();
            double positie;
            int cyclus;
            try {
                positie = std::stod(positieElem->GetText());
                cyclus = std::stoi(cyclusElem->GetText());
            } catch (const std::exception&) {
                lastFoutmelding = "Ongeldige positie of cyclus voor verkeerslicht op baan '" + baan + "'";
                success = false;
                continue;
            }

            if (cyclus <= 0) {
                lastFoutmelding = "Cyclus van verkeerslicht op baan '" + baan + "' moet positief zijn";
                success = false;
                continue;
            }

            Verkeerslicht licht(baan, positie, cyclus);
            if (!situatie.voegVerkeerslichtToe(licht)) {
                lastFoutmelding = "Kan verkeerslicht niet toevoegen aan baan '" + baan + "'";
                success = false;
            }
        }
    }

    // Verifieer de consistentie alleen als er ten minste één baan is toegevoegd
    if (!situatie.verificeerConsistentie()) {
        lastFoutmelding = "Verkeerssituatie is niet consistent";
        return false;
    }

    return success;
}

/**
 * @brief Geeft de laatste foutmelding terug
 * @return De laatste foutmelding
 */
std::string BestandsLezer::getLastFoutmelding() const {
    return lastFoutmelding;
}