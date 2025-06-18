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
 * ENSURE(properlyInitialized(), "constructor moet eindingen in een geldige toestand.");
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
 * @pre
 * @pre
 * @post
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");
 * ENSURE(!success || situatie.verificeerConsistentie(), "Bij succesvolle parsing moet de situatie consistent zijn." );
 *
 */
bool BestandsLezer::leesXmlBestand(const std::string& bestandsnaam, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");
    // Eerst controleren of het bestand kan worden geopend
    std::ifstream fileCheck(bestandsnaam);
    if (!fileCheck.is_open()) {
        lastFoutmelding = "Kan XML-bestand '" + bestandsnaam + "' niet openen";
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

    // Controleer of het bestand leeg is
    if (fileContent.empty() || fileContent.find_first_not_of(" \t\n\r") == std::string::npos) {
        lastFoutmelding = "XML-bestand is niet goed gevormd of kan niet worden geparst";
        return false;
    }

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
            lastFoutmelding = "XML-bestand is niet goed gevormd of kan niet worden geparst";
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

        return success;
    } else {
        // Verwerk normaal met TinyXML als het hoofdelement bestaat
        TiXmlDocument doc;
        if (!doc.LoadFile(bestandsnaam.c_str())) {
            lastFoutmelding = "XML-bestand is niet goed gevormd of kan niet worden geparst";
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

        return success;
    }
}

/**
 * @brief Verwerk XML-elementen en voeg ze toe aan de verkeerssituatie
 * @param root Hoofdelement van het XML-document
 * @param situatie De verkeerssituatie waaraan de elementen worden toegevoegd
 * @return true als de verwerking succesvol was, false indien niet
 * @pre moet correct geïnitialiseerd zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 */
bool BestandsLezer::processXmlElements(TiXmlElement* root, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");

    for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            if (!verwerkBaan(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else if (elementType == "VOERTUIG") {
            if (!verwerkVoertuig(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else if (elementType == "VERKEERSLICHT") {
            if (!verwerkVerkeerslicht(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else if (elementType == "VOERTUIGGENERATOR") {
            if (!verwerkVoertuigGenerator(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else if (elementType == "BUSHALTE") {
            if (!verwerkBushalte(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else if (elementType == "KRUISPUNT") {
            if (!verwerkKruispunt(elem, situatie)) {
                return false; // Stop bij eerste fout
            }
        }
        else {
            lastFoutmelding = "Onbekend element type: " + elementType;
            return false; // Stop bij onbekend element
        }
    }

    return true; // Alles succesvol verwerkt
}

/**
 * @brief Verwerk een wegelement
 * @param elem XML-element met weggegevens
 * @param situatie De verkeerssituatie waaraan de weg moet worden toegevoegd
 * @return true indien succesvol, false indien niet
 * @pre correcte initialisatie
 * @pre het wegelement mag geen nullptr zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor baan mag niet null zijn.");
 */
bool BestandsLezer::verwerkBaan(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor baan mag niet null zijn.");
    TiXmlElement* naamElem = elem->FirstChildElement("naam");
    TiXmlElement* lengteElem = elem->FirstChildElement("lengte");

    // Aangepaste foutmeldingen volgens tests
    if (!naamElem || !naamElem->GetText()) {
        lastFoutmelding = "BAAN element mist verplichte 'naam' eigenschap";
        return false;
    }

    if (!lengteElem || !lengteElem->GetText()) {
        lastFoutmelding = "BAAN element mist verplichte 'lengte' eigenschap";
        return false;
    }

    std::string naam = naamElem->GetText();
    double lengte;
    try {
        lengte = std::stod(lengteElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige lengte waarde voor BAAN '" + naam + "'";
        return false;
    }

    if (lengte <= 0) {
        lastFoutmelding = "BAAN lengte moet positief zijn voor '" + naam + "'";
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
 * @pre moet correct geinitialiseerd zijn
 * @pre voertuigelement mag niet de nullptr zijn.
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor Voertuig mag niet null zijn.");
 */
bool BestandsLezer::verwerkVoertuig(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Voertuig mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* positieElem = elem->FirstChildElement("positie");
    TiXmlElement* typeElem = elem->FirstChildElement("type");

    // Aangepaste foutmeldingen volgens tests
    if (!baanElem || !baanElem->GetText()) {
        lastFoutmelding = "VOERTUIG element mist verplichte 'baan' eigenschap";
        return false;
    }

    if (!positieElem || !positieElem->GetText()) {
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
        lastFoutmelding = "Ongeldige positie waarde voor VOERTUIG op baan '" + baan + "'";
        return false;
    }

    // Controleer of de baan bestaat voordat we het voertuig maken
    auto& banen = situatie.getBanen();
    if (banen.find(baan) == banen.end()) {
        lastFoutmelding = "VOERTUIG verwijst naar onbekende baan '" + baan + "'";
        return false;
    }

    // Controleer of positie binnen baangrenzen valt
    double baanLengte = banen.at(baan).getLengte();
    if (positie > baanLengte) {
        lastFoutmelding = "VOERTUIG positie (" + std::to_string((int)positie) + ") valt buiten baan grenzen voor '" + baan + "' (lengte: " + std::to_string((int)baanLengte) + ")";
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
 * @pre moet correct geïnitialiseerd zijn
 * @pre verkeerslicht mag geen nullptr zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor Verkeerslicht mag niet null zijn.");
 */
bool BestandsLezer::verwerkVerkeerslicht(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Verkeerslicht mag niet null zijn.");
    TiXmlElement* baanElem = elem->FirstChildElement("baan");
    TiXmlElement* positieElem = elem->FirstChildElement("positie");
    TiXmlElement* cyclusElem = elem->FirstChildElement("cyclus");
    TiXmlElement* oranjeElem = elem->FirstChildElement("oranje");
    TiXmlElement* slimElem = elem->FirstChildElement("slim");

    // Aangepaste foutmeldingen volgens tests
    if (!baanElem || !baanElem->GetText()) {
        lastFoutmelding = "VERKEERSLICHT element mist verplichte 'baan' eigenschap";
        return false;
    }

    if (!positieElem || !positieElem->GetText()) {
        lastFoutmelding = "Verkeerslicht mist verplichte elementen (baan, positie of cyclus)";
        return false;
    }

    if (!cyclusElem || !cyclusElem->GetText()) {
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
 * @pre correct geïnitialiseerde voertuiggenerator
 * @pre voertuiggenerator mag niet de nullptr zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor VoertuigGenerator mag niet null zijn.");
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

    // Type is optioneel
    if (typeElem && typeElem->GetText()) {
        type = typeElem->GetText();
    }

    double frequentie;
    try {
        frequentie = std::stod(frequentieElem->GetText());
    } catch (const std::exception&) {
        lastFoutmelding = "Ongeldige frequentie voor voertuiggenerator op baan '" + baan + "'";
        return false;
    }

    if (frequentie <= 0) {
        lastFoutmelding = "Frequentie van voertuiggenerator op baan '" + baan + "' moet positief zijn";
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
 * @pre correcte geïnitialiseerde bushalte
 * @pre bushalte element mag niet de nullptr zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor Bushalte mag niet null zijn.");
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
        // Aangepaste foutmelding volgens test
        lastFoutmelding = "Ongeldige wachttijd waarde voor BUSHALTE op baan '" + baan + "'";
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
 * @pre correcte geïnitialiseerd kruispunt
 * @pre XML element mag niet de nullptr zijn
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 * REQUIRE(elem != nullptr, "XML-element voor Kruispunt mag niet null zijn.");
 */
bool BestandsLezer::verwerkKruispunt(TiXmlElement* elem, VerkeersSituatie& situatie) {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    REQUIRE(elem != nullptr, "XML-element voor Kruispunt mag niet null zijn.");

    Kruispunt kruispunt;
    int baanCount = 0;

    // Verwerk alle baan-elementen
    for (TiXmlElement* baanElem = elem->FirstChildElement("baan");
         baanElem;
         baanElem = baanElem->NextSiblingElement("baan")) {

        baanCount++;  // Tel het aantal baan-elementen

        if (!baanElem->GetText()) {
            lastFoutmelding = "Kruispunt bevat een ongeldige baan (geen naam)";
            return false;
        }

        std::string baanNaam = baanElem->GetText();

        // Haal de positie op als attribuut, of gebruik 0.0 als standaard
        double positie = 0.0;
        const char* positieStr = baanElem->Attribute("positie");
        if (positieStr) {
            try {
                positie = std::stod(positieStr);
            } catch (const std::exception&) {
                lastFoutmelding = "Ongeldige positie voor kruispunt op baan '" + baanNaam + "'";
                return false;
            }
        }

        // Voeg de baan toe aan het kruispunt
        if (!kruispunt.voegBaanToe(baanNaam, positie)) {
            lastFoutmelding = "Kan baan '" + baanNaam + "' niet toevoegen aan kruispunt";
            return false;
        }
    }

    // Controleer of er überhaupt baan-elementen waren
    if (baanCount == 0) {
        lastFoutmelding = "Kruispunt heeft geen geldige banen";
        return false;
    }

    // Controleer of er tenminste 2 banen zijn toegevoegd
    if (kruispunt.getBanen().size() < 2) {
        lastFoutmelding = "Kruispunt heeft geen geldige banen";
        return false;
    }

    // Voeg het kruispunt toe aan de verkeerssituatie
    if (!situatie.voegKruispuntToe(kruispunt)) {
        lastFoutmelding = "Kan kruispunt niet toevoegen";
        return false;
    }

    return true;
}

/**
 * @brief Haal de laatste foutmelding op
 * @return De laatste foutmelding
 * REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
 */
std::string BestandsLezer::getLastFoutmelding() const {
    REQUIRE(properlyInitialized(),"BestandLezer werd niet correct ingesteld");
    return lastFoutmelding;
}