/**
 * @file bestandslezer.cpp
 * @brief Implementation of the BestandsLezer class (Revised with bus stops and intersections)
 */

#include "bestandslezer.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "DesignByContract.h"
#include "../Parser/tinyxml.h"
#include "../TraficObjects/voertuiggenerator.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"

using namespace std;

/**
 * @brief Constructor
 */
BestandsLezer::BestandsLezer() : lastFoutmelding("")
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor moet eindingen in een geldige toestand.");

}

bool BestandsLezer::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Read a traffic situation from an XML file
 * @param bestandsnaam Path to the XML file
 * @param situatie Reference to the traffic situation where the data will be loaded
 * @return true if reading was successful, false otherwise
 */
bool BestandsLezer::leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");
    // First, check if the file can be opened
    std::ifstream fileCheck(bestandsnaam);
    if (!fileCheck.is_open()) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }
    fileCheck.close();

    // Read the file content
    std::string fileContent;
    {
        std::ifstream file(bestandsnaam);
        std::stringstream buffer;
        buffer << file.rdbuf();
        fileContent = buffer.str();
    }

    // Check if it has a valid XML structure
    bool hasRootElement = (fileContent.find("<VerkeersSituatie>") != std::string::npos) ||
                          (fileContent.find("<Verkeerssituatie>") != std::string::npos);

    // If no root element, add one
    if (!hasRootElement) {
        // Create a temporary file with proper structure
        std::string tempFileName = bestandsnaam + ".temp.xml";
        std::ofstream tempFile(tempFileName);

        tempFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        tempFile << "<VerkeersSituatie>" << std::endl;
        tempFile << fileContent;
        tempFile << "</VerkeersSituatie>" << std::endl;

        tempFile.close();

        // Now use TinyXML to parse the temporary file
        TiXmlDocument doc;
        if (!doc.LoadFile(tempFileName.c_str())) {
            lastFoutmelding = "Kan XML-bestand niet parsen, controleer de syntax";
            std::remove(tempFileName.c_str());  // Clean up temp file
            return false;
        }

        TiXmlElement* root = doc.RootElement();
        if (!root) {
            lastFoutmelding = "XML-bestand heeft geen root element";
            std::remove(tempFileName.c_str());  // Clean up temp file
            return false;
        }

        bool success = processXmlElements(root, situatie);

        // Clean up temporary file
        std::remove(tempFileName.c_str());

        // Verify the consistency only if there is at least one road added
        if (!situatie.verificeerConsistentie()) {
            lastFoutmelding = "Verkeerssituatie is niet consistent";
            return false;
        }
        ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );
        return success;
    } else {
        // Process normally with TinyXML if root element exists
        TiXmlDocument doc;
        if (!doc.LoadFile(bestandsnaam.c_str())) {
            lastFoutmelding = "Kan XML-bestand niet parsen, controleer de syntax";
            return false;
        }

        TiXmlElement* root = doc.RootElement();
        if (!root) {
            lastFoutmelding = "XML-bestand heeft geen root element";
            return false;
        }

        std::string rootName = root->Value();
        if (rootName != "VerkeersSituatie" && rootName != "Verkeerssituatie") {
            cerr << "Root element is geen VerkeersSituatie maar: " << rootName << std::endl;
            // We'll try to process it anyway instead of failing
        }

        bool success = processXmlElements(root, situatie);

        // Verify the consistency only if there is at least one road added
        if (!situatie.verificeerConsistentie()) {
            lastFoutmelding = "Verkeerssituatie is niet consistent";
            return false;
        }
        ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );

        return success;
    }
}

/**
 * @brief Process XML elements and add them to the traffic situation
 * @param root Root element of the XML document
 * @param situatie The traffic situation to add the elements to
 * @return true if processing was successful, false otherwise
 */
bool BestandsLezer::processXmlElements(TiXmlElement* root, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");

    bool success = true;

    for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            success &= verwerkBaan(elem, situatie);
        }
        else if (elementType == "VOERTUIG") {
            success &= verwerkVoertuig(elem, situatie);
        }
        else if (elementType == "VERKEERSLICHT") {
            success &= verwerkVerkeerslicht(elem, situatie);
        }
        else if (elementType == "VOERTUIGGENERATOR") {
            success &= verwerkVoertuigGenerator(elem, situatie);
        }
        else if (elementType == "BUSHALTE") {
            success &= verwerkBushalte(elem, situatie);
        }
        else if (elementType == "KRUISPUNT") {
            success &= verwerkKruispunt(elem, situatie);
        }
        else {
            lastFoutmelding = "Onbekend element type: " + elementType;
            // Continue processing, don't set success to false for unknown elements
        }
    }
    //ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );

    return success;
}

/**
 * @brief Process a road element
 * @param elem XML element containing road data
 * @param situatie The traffic situation to add the road to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkBaan(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor baan mag niet null zijn.");
    TiXmlElement* naamElem = elem->FirstChildElement("naam");
    TiXmlElement* lengteElem = elem->FirstChildElement("lengte");

    if (!naamElem || !lengteElem || !naamElem->GetText() || !lengteElem->GetText()) {
        lastFoutmelding = "Baan mist verplichte elementen (naam of lengte)";
        return false;
    }

    std::string naam = naamElem->GetText();
    double lengte;
    try {
        lengte = std::stod(lengteElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige lengte voor baan '" + naam + "'";
        return false;
    }

    if (lengte <= 0) {
        lastFoutmelding = "Lengte van baan '" + naam + "' moet positief zijn";
        return false;
    }


    Baan baan(naam, lengte);
    return situatie.voegBaanToe(baan);
}

/**
 * @brief Process a vehicle element
 * @param elem XML element containing vehicle data
 * @param situatie The traffic situation to add the vehicle to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkVoertuig(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Voertuig mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* positieElem = elem->FirstChildElement("positie");
    TiXmlElement* typeElem = elem->FirstChildElement("type");

    if (!baanElem || !positieElem || !baanElem->GetText() || !positieElem->GetText()) {
        lastFoutmelding = "Voertuig mist verplichte elementen (baan of positie)";
        return false;
    }

    std::string baan = baanElem->GetText();
    std::string type = "auto"; // Default type

    // Type is optional, so check if it exists
    if (typeElem && typeElem->GetText()) {
        type = typeElem->GetText();
    }

    double positie;
    try {
        positie = std::stod(positieElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige positie voor voertuig op baan '" + baan + "'";
        return false;
    }

    Voertuig voertuig(baan, positie, type);
    if (!situatie.voegVoertuigToe(voertuig)) {
        lastFoutmelding = "Kan voertuig niet toevoegen aan baan '" + baan + "'";
        return false;
    }

    return true;
}

/**
 * @brief Process a traffic light element
 * @param elem XML element containing traffic light data
 * @param situatie The traffic situation to add the traffic light to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkVerkeerslicht(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Verkeerslicht mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* positieElem = elem->FirstChildElement("positie");
    TiXmlElement* cyclusElem = elem->FirstChildElement("cyclus");
    TiXmlElement* oranjeElem = elem->FirstChildElement("oranje");
    TiXmlElement* slimElem = elem->FirstChildElement("slim");

    if (!baanElem || !positieElem || !cyclusElem ||
        !baanElem->GetText() || !positieElem->GetText() || !cyclusElem->GetText()) {
        lastFoutmelding = "Verkeerslicht mist verplichte elementen (baan, positie of cyclus)";
        return false;
    }

    std::string baan = baanElem->GetText();
    double positie;
    int cyclus;
    bool heeftOranje = false;
    bool isSlim = false;

    try {
        positie = std::stod(positieElem->GetText());
        cyclus = std::stoi(cyclusElem->GetText());

        // Check optional attributes
        if (oranjeElem && oranjeElem->GetText()) {
            heeftOranje = std::string(oranjeElem->GetText()) == "true" ||
                          std::string(oranjeElem->GetText()) == "1" ||
                          std::string(oranjeElem->GetText()) == "ja";
        }

        if (slimElem && slimElem->GetText()) {
            isSlim = std::string(slimElem->GetText()) == "true" ||
                     std::string(slimElem->GetText()) == "1" ||
                     std::string(slimElem->GetText()) == "ja";
        }
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige positie of cyclus voor verkeerslicht op baan '" + baan + "'";
        return false;
    }

    if (cyclus <= 0) {
        lastFoutmelding = "Cyclus van verkeerslicht op baan '" + baan + "' moet positief zijn";
        return false;
    }

    Verkeerslicht licht(baan, positie, cyclus, heeftOranje, isSlim);
    if (!situatie.voegVerkeerslichtToe(licht)) {
        lastFoutmelding = "Kan verkeerslicht niet toevoegen aan baan '" + baan + "'";
        return false;
    }

    return true;
}

/**
 * @brief Process a vehicle generator element
 * @param elem XML element containing vehicle generator data
 * @param situatie The traffic situation to add the generator to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkVoertuigGenerator(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor VoertuigGenerator mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* frequentieElem = elem->FirstChildElement("frequentie");
    TiXmlElement* typeElem = elem->FirstChildElement("type");

    if (!baanElem || !frequentieElem || !baanElem->GetText() || !frequentieElem->GetText()) {
        lastFoutmelding = "VoertuigGenerator mist verplichte elementen (baan of frequentie)";
        return false;
    }

    std::string baan = baanElem->GetText();
    std::string type = "auto"; // Default type

    // Type is optional, so check if it exists
    if (typeElem && typeElem->GetText()) {
        type = typeElem->GetText();
    }

    int frequentie;
    try {
        frequentie = std::stoi(frequentieElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige frequentie voor voertuiggenerator op baan '" + baan + "'";
        return false;
    }

    if (frequentie <= 0) {
        lastFoutmelding = "Frequentie van voertuiggenerator moet positief zijn";
        return false;
    }

    VoertuigGenerator generator(baan, frequentie, type);
    if (!situatie.voegVoertuigGeneratorToe(generator)) {
        lastFoutmelding = "Kan voertuiggenerator niet toevoegen aan baan '" + baan + "'";
        return false;
    }

    return true;
}

/**
 * @brief Process a bus stop element
 * @param elem XML element containing bus stop data
 * @param situatie The traffic situation to add the bus stop to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkBushalte(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Bushalte mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* positieElem = elem->FirstChildElement("positie");
    TiXmlElement* wachttijdElem = elem->FirstChildElement("wachttijd");

    if (!baanElem || !positieElem || !wachttijdElem ||
        !baanElem->GetText() || !positieElem->GetText() || !wachttijdElem->GetText()) {
        lastFoutmelding = "Bushalte mist verplichte elementen (baan, positie of wachttijd)";
        return false;
    }

    std::string baan = baanElem->GetText();
    double positie;
    int wachttijd;

    try {
        positie = std::stod(positieElem->GetText());
        wachttijd = std::stoi(wachttijdElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige positie of wachttijd voor bushalte op baan '" + baan + "'";
        return false;
    }

    if (wachttijd <= 0) {
        lastFoutmelding = "Wachttijd van bushalte op baan '" + baan + "' moet positief zijn";
        return false;
    }

    Bushalte halte(baan, positie, wachttijd);
    if (!situatie.voegBushalteToe(halte)) {
        lastFoutmelding = "Kan bushalte niet toevoegen aan baan '" + baan + "'";
        return false;
    }

    return true;
}

/**
 * @brief Process an intersection element
 * @param elem XML element containing intersection data
 * @param situatie The traffic situation to add the intersection to
 * @return true if successful, false otherwise
 */
bool BestandsLezer::verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Kruispunt mag niet null zijn.");
    Kruispunt kruispunt;
    bool success = true;

    // Process all roads that are part of this intersection
    for (TiXmlElement* baanElem = elem->FirstChildElement("baan");
         baanElem;
         baanElem = baanElem->NextSiblingElement("baan")) {

        // Get the road position as an attribute
        const char* positieStr = baanElem->Attribute("positie");
        if (!positieStr || !baanElem->GetText()) {
            lastFoutmelding = "Kruispunt bevat een ongeldige baan (geen positie attribuut of naam)";
            success = false;
            continue;
        }

        std::string baanNaam = baanElem->GetText();
        double positie;

        try {
            positie = std::stod(positieStr);
        } catch (const std::exception&) {
            lastFoutmelding = "Ongeldige positie voor kruispunt op baan '" + baanNaam + "'";
            success = false;
            continue;
        }

        // Add the road to the intersection
        if (!kruispunt.voegBaanToe(baanNaam, positie)) {
            lastFoutmelding = "Kan baan '" + baanNaam + "' niet toevoegen aan kruispunt";
            success = false;
        }
    }

    // Only add the intersection if at least one road was successfully added
    if (kruispunt.getBanen().empty()) {
        lastFoutmelding = "Kruispunt heeft geen geldige banen";
        return false;
    }

    // Add the intersection to the traffic situation
    if (!situatie.voegKruispuntToe(kruispunt)) {
        lastFoutmelding = "Kan kruispunt niet toevoegen aan de verkeerssituatie";
        return false;
    }

    return success;
}

/**
 * @brief Get the last error message
 * @return The last error message
 */
std::string BestandsLezer::getLastFoutmelding() const {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    return lastFoutmelding;
}