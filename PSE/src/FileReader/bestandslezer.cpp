/**
 * @file bestandslezer.cpp
 * @brief Implementatie van de BestandsLezer klasse (Herzien met bushaltes en kruispunten)
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
 * @brief Lees een verkeerssituatie uit een XML-bestand
 * @param bestandsnaam Pad naar het XML-bestand
 * @param situatie Referentie naar de verkeerssituatie waar de gegevens worden geladen
 * @return true als het lezen succesvol was, false indien niet
 */
bool BestandsLezer::leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");
    // Eerst controleren of het bestand kan worden geopend
    std::ifstream fileCheck(bestandsnaam);
    if (!fileCheck.is_open()) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }
    fileCheck.close();

    // Lees de bestandsinhoud
    std::string fileContent;
    {
        std::ifstream file(bestandsnaam);
        std::stringstream buffer;
        buffer << file.rdbuf();
        fileContent = buffer.str();
    }

    // Controleer of het een geldige XML-structuur heeft
    bool hasRootElement = (fileContent.find("<VerkeersSituatie>") != std::string::npos) ||
                          (fileContent.find("<Verkeerssituatie>") != std::string::npos);

    // Als er geen hoofdelement is, voeg er dan een toe
    if (!hasRootElement) {
        // Maak een tijdelijk bestand met de juiste structuur
        std::string tempFileName = bestandsnaam + ".temp.xml";
        std::ofstream tempFile(tempFileName);

        tempFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        tempFile << "<VerkeersSituatie>" << std::endl;
        tempFile << fileContent;
        tempFile << "</VerkeersSituatie>" << std::endl;

        tempFile.close();

        // Gebruik nu TinyXML om het tijdelijke bestand te parsen
        TiXmlDocument doc;
        if (!doc.LoadFile(tempFileName.c_str())) {
            lastFoutmelding = "Kan XML-bestand niet parsen, controleer de syntax.";
            std::remove(tempFileName.c_str());  // Tijdelijk bestand opruimen
            return false;
        }

        TiXmlElement* root = doc.RootElement();
        if (!root) {
            lastFoutmelding = "XML-bestand heeft geen root element";
            std::remove(tempFileName.c_str());  // Tijdelijk bestand opruimen
            return false;
        }

        bool success = processXmlElements(root, situatie);

        // Tijdelijk bestand opruimen
        std::remove(tempFileName.c_str());

        // Controleer de consistentie alleen als er ten minste één weg is toegevoegd
        if (!situatie.verificeerConsistentie()) {
            lastFoutmelding = "Verkeerssituatie is niet consistent";
            return false;
        }
        ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );
        return success;
    } else {
        // Verwerk normaal met TinyXML als het hoofdelement bestaat
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
            // We proberen het toch te verwerken in plaats van te mislukken
        }

        bool success = processXmlElements(root, situatie);

        // Controleer de consistentie alleen als er ten minste één weg is toegevoegd
        if (!situatie.verificeerConsistentie()) {
            lastFoutmelding = "Verkeerssituatie is niet consistent";
            return false;
        }
        ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );

        return success;
    }
}

/**
 * @brief Verwerk XML-elementen en voeg ze toe aan de verkeerssituatie
 * @param root Hoofdelement van het XML-document
 * @param situatie De verkeerssituatie waaraan de elementen worden toegevoegd
 * @return true als de verwerking succesvol was, false indien niet
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
            // Ga door met verwerken, zet success niet op false voor onbekende elementen
        }
    }
    //ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );

    return success;
}

/**
 * @brief Verwerk een wegelement
 * @param elem XML-element met weggegevens
 * @param situatie De verkeerssituatie waaraan de weg moet worden toegevoegd
 * @return true indien succesvol, false indien niet
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
 * @brief Verwerk een voertuigelement
 * @param elem XML-element met voertuiggegevens
 * @param situatie De verkeerssituatie waaraan het voertuig moet worden toegevoegd
 * @return true indien succesvol, false indien niet
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
    std::string type = "auto"; // Standaard type

    // Type is optioneel, dus controleer of het bestaat
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

    // Use the factory method to create a vehicle of the appropriate type
    auto voertuig = Voertuig::maakVoertuig(baan, positie, type);
    if (!situatie.voegVoertuigToe(std::move(voertuig))) {
        lastFoutmelding = "Kan voertuig niet toevoegen aan baan '" + baan + "'";
        return false;
    }

    return true;
}

/**
 * @brief Verwerk een verkeerslicht element
 * @param elem XML-element met verkeerslichtgegevens
 * @param situatie De verkeerssituatie waaraan het verkeerslicht moet worden toegevoegd
 * @return true indien succesvol, false indien niet
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

        // Controleer optionele attributen
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
 * @brief Verwerk een voertuiggenerator element
 * @param elem XML-element met voertuiggeneratorgegevens
 * @param situatie De verkeerssituatie waaraan de generator moet worden toegevoegd
 * @return true indien succesvol, false indien niet
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
    std::string type = "auto"; // Standaard type

    // Type is optioneel, dus controleer of het bestaat
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
 * @brief Verwerk een bushalte element
 * @param elem XML-element met bushaltegegevens
 * @param situatie De verkeerssituatie waaraan de bushalte moet worden toegevoegd
 * @return true indien succesvol, false indien niet
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
 * @brief Verwerk een kruispunt element
 * @param elem XML-element met kruispuntgegevens
 * @param situatie De verkeerssituatie waaraan het kruispunt moet worden toegevoegd
 * @return true indien succesvol, false indien niet
 */
bool BestandsLezer::verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Kruispunt mag niet null zijn.");
    Kruispunt kruispunt;
    bool success = true;

    // Verwerk alle wegen die deel uitmaken van dit kruispunt
    for (TiXmlElement* baanElem = elem->FirstChildElement("baan");
         baanElem;
         baanElem = baanElem->NextSiblingElement("baan")) {

        // Haal de wegpositie op als een attribuut
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

        // Voeg de weg toe aan het kruispunt
        if (!kruispunt.voegBaanToe(baanNaam, positie)) {
            lastFoutmelding = "Kan baan '" + baanNaam + "' niet toevoegen aan kruispunt";
            success = false;
        }
    }

    // Voeg het kruispunt alleen toe als er ten minste één weg met succes is toegevoegd
    if (kruispunt.getBanen().empty()) {
        lastFoutmelding = "Kruispunt heeft geen geldige banen";
        return false;
    }

    // Voeg het kruispunt toe aan de verkeerssituatie
    if (!situatie.voegKruispuntToe(kruispunt)) {
        lastFoutmelding = "Kan kruispunt niet toevoegen aan de verkeerssituatie";
        return false;
    }

    return success;
}

/**
 * @brief Krijg de laatste foutmelding
 * @return De laatste foutmelding
 */
std::string BestandsLezer::getLastFoutmelding() const {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    return lastFoutmelding;
}