/**
 * @file output.cpp
 * @author Claude
 * @date 19/03/2025
 * @brief Implementatie van de output klasse
 */

#include "output.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "tinyxml.h"

/**
 * @brief Constructor
 */
output::output() : lastFoutmelding("") {}

/**
 * @brief Genereert een tekstuele weergave van de verkeerssituatie
 * @param situatie De verkeerssituatie die moet worden weergegeven
 * @return Een string met de tekstuele weergave
 */
std::string output::genereerTekstRapport(const VerkeersSituatie& situatie) const {
    std::stringstream ss;
    ss << "=== Verkeerssituatie Info ===" << std::endl;

    const auto& banen = situatie.getBanen();
    ss << "Banen (" << banen.size() << "):" << std::endl;
    for (const auto& paar : banen) {
        const Baan& baan = paar.second;
        ss << " - " << baan.getNaam() << " (lengte: " << baan.getLengte() << "m)" << std::endl;
    }

    const auto& voertuigen = situatie.getVoertuigen();
    ss << "Voertuigen (" << voertuigen.size() << "):" << std::endl;
    for (const auto& voertuig : voertuigen) {
        ss << " - Voertuig op baan '" << voertuig.getBaan()
           << "' (positie: " << voertuig.getPositie()
           << "m, snelheid: " << voertuig.getSnelheid() << "m/s)" << std::endl;
    }

    const auto& verkeerslichten = situatie.getVerkeerslichten();
    ss << "Verkeerslichten (" << verkeerslichten.size() << "):" << std::endl;
    for (const auto& licht : verkeerslichten) {
        ss << " - Verkeerslicht op baan '" << licht.getBaan()
           << "' (positie: " << licht.getPositie()
           << "m, cyclus: " << licht.getCyclus() << "s)" << std::endl;
    }

    return ss.str();
}

/**
 * @brief Schrijft een verkeerssituatie naar een XML-bestand
 * @param situatie De verkeerssituatie die moet worden geschreven
 * @param bestandsnaam Pad naar het XML-bestand
 * @return true als het schrijven succesvol was, anders false
 */
bool output::schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const {
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    doc.LinkEndChild(decl);

    TiXmlElement* root = new TiXmlElement("VerkeersSituatie");
    doc.LinkEndChild(root);

    // Schrijf banen
    const auto& banen = situatie.getBanen();
    for (const auto& paar : banen) {
        const Baan& baan = paar.second;
        
        TiXmlElement* baanElement = new TiXmlElement("BAAN");
        
        TiXmlElement* naamElement = new TiXmlElement("naam");
        TiXmlText* naamText = new TiXmlText(baan.getNaam().c_str());
        naamElement->LinkEndChild(naamText);
        baanElement->LinkEndChild(naamElement);
        
        TiXmlElement* lengteElement = new TiXmlElement("lengte");
        std::stringstream ss;
        ss << baan.getLengte();
        TiXmlText* lengteText = new TiXmlText(ss.str().c_str());
        lengteElement->LinkEndChild(lengteText);
        baanElement->LinkEndChild(lengteElement);
        
        root->LinkEndChild(baanElement);
    }

    // Schrijf voertuigen
    const auto& voertuigen = situatie.getVoertuigen();
    for (const auto& voertuig : voertuigen) {
        TiXmlElement* voertuigElement = new TiXmlElement("VOERTUIG");
        
        TiXmlElement* baanElement = new TiXmlElement("baan");
        TiXmlText* baanText = new TiXmlText(voertuig.getBaan().c_str());
        baanElement->LinkEndChild(baanText);
        voertuigElement->LinkEndChild(baanElement);
        
        TiXmlElement* positieElement = new TiXmlElement("positie");
        std::stringstream ss;
        ss << voertuig.getPositie();
        TiXmlText* positieText = new TiXmlText(ss.str().c_str());
        positieElement->LinkEndChild(positieText);
        voertuigElement->LinkEndChild(positieElement);
        
        root->LinkEndChild(voertuigElement);
    }

    // Schrijf verkeerslichten
    const auto& verkeerslichten = situatie.getVerkeerslichten();
    for (const auto& licht : verkeerslichten) {
        TiXmlElement* lichtElement = new TiXmlElement("VERKEERSLICHT");
        
        TiXmlElement* baanElement = new TiXmlElement("baan");
        TiXmlText* baanText = new TiXmlText(licht.getBaan().c_str());
        baanElement->LinkEndChild(baanText);
        lichtElement->LinkEndChild(baanElement);
        
        TiXmlElement* positieElement = new TiXmlElement("positie");
        std::stringstream ssPos;
        ssPos << licht.getPositie();
        TiXmlText* positieText = new TiXmlText(ssPos.str().c_str());
        positieElement->LinkEndChild(positieText);
        lichtElement->LinkEndChild(positieElement);
        
        TiXmlElement* cyclusElement = new TiXmlElement("cyclus");
        std::stringstream ssCyclus;
        ssCyclus << licht.getCyclus();
        TiXmlText* cyclusText = new TiXmlText(ssCyclus.str().c_str());
        cyclusElement->LinkEndChild(cyclusText);
        lichtElement->LinkEndChild(cyclusElement);
        
        root->LinkEndChild(lichtElement);
    }

    // Schrijf het bestand
    bool saveSuccess = doc.SaveFile(bestandsnaam.c_str());
    if (!saveSuccess) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet schrijven";
        return false;
    }

    return true;
}

/**
 * @brief Geeft de laatste foutmelding terug
 * @return De laatste foutmelding
 */
std::string output::getLastFoutmelding() const {
    return lastFoutmelding;
}