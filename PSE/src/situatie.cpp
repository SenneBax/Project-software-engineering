//
// Created by senne on 27/02/2025.
//

#include "situatie.h"
#include "tinyxml.h" // Update this path to where TinyXML is installed on your system

using namespace std;

bool VerkeersSituatie::voegBaanToe(const Baan& baan) {
    if (banen.find(baan.getNaam()) != banen.end()) {
        cerr << "Fout: Baan '" << baan.getNaam() << "' bestaat al." << endl;
        return false;
    }
    banen[baan.getNaam()] = baan;
    return true;
}

bool VerkeersSituatie::voegVoertuigToe(const Voertuig& voertuig) {
    // Controleer of de baan bestaat
    if (banen.find(voertuig.getBaan()) == banen.end()) {
        cerr << "Fout: Baan '" << voertuig.getBaan() << "' bestaat niet voor voertuig." << endl;
        return false;
    }

    // Controleer of de positie geldig is
    if (voertuig.getPositie() < 0 || voertuig.getPositie() > banen[voertuig.getBaan()].getLengte()) {
        cerr << "Fout: Ongeldige positie voor voertuig op baan '" << voertuig.getBaan() << "'." << endl;
        return false;
    }

    voertuigen.push_back(voertuig);
    return true;
}

bool VerkeersSituatie::voegVerkeerslichtToe(const Verkeerslicht& verkeerslicht) {
    // Controleer of de baan bestaat
    if (banen.find(verkeerslicht.getBaan()) == banen.end()) {
        cerr << "Fout: Baan '" << verkeerslicht.getBaan() << "' bestaat niet voor verkeerslicht." << endl;
        return false;
    }

    // Controleer of de positie geldig is
    if (verkeerslicht.getPositie() < 0 || verkeerslicht.getPositie() > banen[verkeerslicht.getBaan()].getLengte()) {
        cerr << "Fout: Ongeldige positie voor verkeerslicht op baan '" << verkeerslicht.getBaan() << "'." << endl;
        return false;
    }

    // Controleer of de cyclus geldig is
    if (verkeerslicht.getCyclus() <= 0) {
        cerr << "Fout: Ongeldige cyclus voor verkeerslicht op baan '" << verkeerslicht.getBaan() << "'." << endl;
        return false;
    }

    verkeerslichten.push_back(verkeerslicht);
    return true;
}

bool VerkeersSituatie::verificeerConsistentie() const {
    // Controleer of er minstens één baan is
    if (banen.empty()) {
        cerr << "Fout: Geen banen gedefinieerd in de verkeerssituatie." << endl;
        return false;
    }

    // Andere consistentie checks kunnen hier worden toegevoegd
    return true;
}

void VerkeersSituatie::printInfo() const {
    cout << "Verkeerssituatie informatie:" << endl;

    cout << "\nBanen (" << banen.size() << "):" << endl;
    for (const auto& baan : banen) {
        cout << "  - " << baan.second.getNaam() << " (lengte: " << baan.second.getLengte() << ")" << endl;
    }

    cout << "\nVoertuigen (" << voertuigen.size() << "):" << endl;
    for (const auto& voertuig : voertuigen) {
        cout << "  - Voertuig op " << voertuig.getBaan() << " (positie: " << voertuig.getPositie() << ")" << endl;
    }

    cout << "\nVerkeerslichten (" << verkeerslichten.size() << "):" << endl;
    for (const auto& verkeerslicht : verkeerslichten) {
        cout << "  - Verkeerslicht op " << verkeerslicht.getBaan()
                  << " (positie: " << verkeerslicht.getPositie()
                  << ", cyclus: " << verkeerslicht.getCyclus() << ")" << endl;
    }
}


/*/
 * Case 1.1
 *
 *het inlezen van verkeerssituaties via de gegeven xml documenten.
 *dit volgens de stappen die gegeven waren in specificatie 1.0
 *
 *(de cerr kan in principe verwijderd worden voor de testen dit geeft de rode tekst om aan te geven wat er misloopt
 */

bool leesVerkeersSituatie(const string& bestandsnaam, VerkeersSituatie& situatie) {
    // 1. Open invoerbestand
    TiXmlDocument doc(bestandsnaam.c_str());
    if (!doc.LoadFile()) {
        cerr << "Fout bij openen bestand: " << doc.ErrorDesc() << endl;
        return false;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        cerr << "Leeg XML bestand." << endl;
        return false;
    }

    // 2. WHILE Bestand niet ingelezen
    for (TiXmlElement* element = root->FirstChildElement(); element; element = element->NextSiblingElement()) {
        string elementType = element->Value();

        // 2.1. Herken het soort element
        if (elementType == "BAAN") {
            // 2.2. Lees verdere informatie voor het element
            TiXmlElement* naamElement = element->FirstChildElement("naam");
            TiXmlElement* lengteElement = element->FirstChildElement("lengte");

            // 2.3. Verifieer geldige informatie
            if (!naamElement || !lengteElement) {
                cerr << "Fout: Ontbrekende informatie voor BAAN element." << endl;
                continue; // Ga naar volgend element
            }

            string naam = naamElement->GetText() ? naamElement->GetText() : "";
            int lengte = 0;
            if (lengteElement->GetText()) {
                lengte = stoi(lengteElement->GetText());
            }

            if (naam.empty() || lengte <= 0) {
                cerr << "Fout: Ongeldige informatie voor BAAN element." << endl;
                continue; // Ga naar volgend element
            }

            // 2.3.1 Voeg element toe aan de simulatie
            Baan baan(naam, lengte);
            situatie.voegBaanToe(baan);
        }
        else if (elementType == "VOERTUIG") {
            // 2.2. Lees verdere informatie voor het element
            TiXmlElement* baanElement = element->FirstChildElement("baan");
            TiXmlElement* positieElement = element->FirstChildElement("positie");

            // 2.3. Verifieer geldige informatie
            if (!baanElement || !positieElement) {
                cerr << "Fout: Ontbrekende informatie voor VOERTUIG element." << endl;
                continue; // Ga naar volgend element
            }

            string baan = baanElement->GetText() ? baanElement->GetText() : "";
            int positie = 0;
            if (positieElement->GetText()) {
                positie = stoi(positieElement->GetText());
            }

            if (baan.empty() || positie < 0) {
                cerr << "Fout: Ongeldige informatie voor VOERTUIG element." << endl;
                continue; // Ga naar volgend element
            }

            // 2.3.1 Voeg element toe aan de simulatie
            Voertuig voertuig(baan, positie);
            situatie.voegVoertuigToe(voertuig);
        }
        else if (elementType == "VERKEERSLICHT") {
            // 2.2. Lees verdere informatie voor het element
            TiXmlElement* baanElement = element->FirstChildElement("baan");
            TiXmlElement* positieElement = element->FirstChildElement("positie");
            TiXmlElement* cyclusElement = element->FirstChildElement("cyclus");

            // 2.3. Verifieer geldige informatie
            if (!baanElement || !positieElement || !cyclusElement) {
                cerr << "Fout: Ontbrekende informatie voor VERKEERSLICHT element." << endl;
                continue; // Ga naar volgend element
            }

            string baan = baanElement->GetText() ? baanElement->GetText() : "";
            int positie = 0;
            int cyclus = 0;

            if (positieElement->GetText()) {
                positie = stoi(positieElement->GetText());
            }

            if (cyclusElement->GetText()) {
                cyclus = stoi(cyclusElement->GetText());
            }

            if (baan.empty() || positie < 0 || cyclus <= 0) {
                cerr << "Fout: Ongeldige informatie voor VERKEERSLICHT element." << endl;
                continue; // Ga naar volgend element
            }

            // 2.3.1 Voeg element toe aan de simulatie
            Verkeerslicht verkeerslicht(baan, positie, cyclus);
            situatie.voegVerkeerslichtToe(verkeerslicht);
        }
        else {
            // Onherkenbaar element
            cerr << "Fout: Onherkenbaar element '" << elementType << "'." << endl;
            // Ga door naar volgend element
        }
    }

    // 3. Verifieer consistentie van de verkeerssituatie
    if (!situatie.verificeerConsistentie()) {
        cerr << "Fout: Inconsistente verkeerssituatie." << endl;
        return false;
    }

    // 4. Sluit invoerbestand (gebeurt automatisch bij het verlaten van de functie)
    return true;
}