//
// Gemaakt door Senne op 11/03/2025.
//

#include "Situatie.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "tinyxml.h"

// Methode om een baan toe te voegen
bool VerkeersSituatie::voegBaanToe(const Baan& baan) {
    // Controleer of de baan al bestaat
    if (banen.find(baan.getNaam()) != banen.end()) {

        return false;
    }

    // Voeg de baan toe
    banen[baan.getNaam()] = baan;
    return true;
}

// Methode om een voertuig toe te voegen
bool VerkeersSituatie::voegVoertuigToe(const Voertuig& voertuig) {
    // Controleer of de baan bestaat
    if (banen.find(voertuig.getBaan()) == banen.end()) {

        return false;
    }

    // Controleer of de positie geldig is
    const Baan& baan = banen[voertuig.getBaan()];
    if (voertuig.getPositie() < 0 || voertuig.getPositie() > baan.getLengte()) {

        return false;
    }

    // Voeg het voertuig toe
    voertuigen.push_back(voertuig);
    return true;
}

// Methode om een verkeerslicht toe te voegen
bool VerkeersSituatie::voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht) {
    // Controleer of de baan bestaat
    if (banen.find(verkeerslicht.getBaan()) == banen.end()) {

        return false;
    }

    // Controleer of de positie geldig is
    const Baan& baan = banen[verkeerslicht.getBaan()];
    if (verkeerslicht.getPositie() < 0 || verkeerslicht.getPositie() > baan.getLengte()) {

        return false;
    }

    // Controleer of de cyclus geldig is
    if (verkeerslicht.getCyclus() <= 0) {

        return false;
    }

    // Voeg het verkeerslicht toe
    verkeerslichten.push_back(verkeerslicht);
    return true;
}

// Methode om consistentie te verifiëren
bool VerkeersSituatie::verificeerConsistentie() const {
    bool isConsistent = true;

    // Controleer of er banen zijn
    if (banen.empty()) {

        isConsistent = false;
    }

    // Controleer of alle voertuigen op geldige banen staan
    for (const Voertuig& voertuig : voertuigen) {
        if (banen.find(voertuig.getBaan()) == banen.end()) {

            isConsistent = false;
        }
    }

    // Controleer of alle verkeerslichten op geldige banen staan
    for (const Verkeerslicht& verkeerslicht : verkeerslichten) {
        auto it = banen.find(verkeerslicht.getBaan());
        if (it == banen.end()) {

            isConsistent = false;
        } else {
            const Baan& baan = it->second;
            if (verkeerslicht.getPositie() < 0 || verkeerslicht.getPositie() > baan.getLengte()) {

                isConsistent = false;
            }
        }
    }

    return isConsistent;
}

// Methode om een voertuig te verwijderen
bool VerkeersSituatie::verwijderVoertuig(int index) {
    if (index < 0 || index >= static_cast<int>(voertuigen.size())) {

        return false;
    }

    voertuigen.erase(voertuigen.begin() + index);
    return true;
}

// Methode om informatie te printen
void VerkeersSituatie::printInfo() const {
    std::cout << "=== Verkeerssituatie Info ===" << std::endl;

    std::cout << "Banen (" << banen.size() << "):" << std::endl;
    for (const auto& paar : banen) {
        const Baan& baan = paar.second;
        std::cout << " - " << baan.getNaam() << " (lengte: " << baan.getLengte() << "m)" << std::endl;
    }

    std::cout << "Voertuigen (" << voertuigen.size() << "):" << std::endl;
    for (const auto& voertuig : voertuigen) {
        std::cout << " - Voertuig op baan '" << voertuig.getBaan()
                  << "' (positie: " << voertuig.getPositie()
                  << "m, snelheid: " << voertuig.getSnelheid() << "m/s)" << std::endl;
    }

    std::cout << "Verkeerslichten (" << verkeerslichten.size() << "):" << std::endl;
    for (const auto& licht : verkeerslichten) {
        std::cout << " - Verkeerslicht op baan '" << licht.getBaan()
                  << "' (positie: " << licht.getPositie()
                  << "m, cyclus: " << licht.getCyclus() << "s)" << std::endl;
    }
}

// Functie om een verkeerssituatie te lezen vanuit een XML-bestand
bool leesVerkeersSituatie(const std::string& bestandsnaam, VerkeersSituatie& situatie) {
    TiXmlDocument doc;
    if (!doc.LoadFile(bestandsnaam.c_str())) {

        return false;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {

        return false;
    }

    std::string rootName = root->Value();
    if (rootName != "VerkeersSituatie" && rootName != "Verkeerssituatie") {

        return false;
    }

    bool success = true;

    for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string elementType = elem->Value();

        if (elementType == "BAAN") {
            TiXmlElement* naamElem = elem->FirstChildElement("naam");
            TiXmlElement* lengteElem = elem->FirstChildElement("lengte");

            if (!naamElem || !lengteElem || !naamElem->GetText() || !lengteElem->GetText()) {

                success = false;
                continue;
            }

            std::string naam = naamElem->GetText();
            double lengte;
            try {
                lengte = std::stod(lengteElem->GetText());
            } catch (const std::exception&) {

                success = false;
                continue;
            }

            if (lengte <= 0) {

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

                success = false;
                continue;
            }

            std::string baan = baanElem->GetText();
            double positie;
            try {
                positie = std::stod(positieElem->GetText());
            } catch (const std::exception&) {

                success = false;
                continue;
            }

            Voertuig voertuig(baan, positie);
            if (!situatie.voegVoertuigToe(voertuig)) {
                success = false;
            }
        }
        else if (elementType == "VERKEERSLICHT") {
            TiXmlElement* baanElem = elem->FirstChildElement("baan");
            TiXmlElement* positieElem = elem->FirstChildElement("positie");
            TiXmlElement* cyclusElem = elem->FirstChildElement("cyclus");

            if (!baanElem || !positieElem || !cyclusElem ||
                !baanElem->GetText() || !positieElem->GetText() || !cyclusElem->GetText()) {

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

                success = false;
                continue;
            }

            if (cyclus <= 0) {

                success = false;
                continue;
            }

            Verkeerslicht licht(baan, positie, cyclus);
            if (!situatie.voegVerkeerslichtToe(licht)) {
                success = false;
            }
        }
    }

    // Verifieer de consistentie alleen als er ten minste één baan is toegevoegd
    return situatie.verificeerConsistentie() && success;
}