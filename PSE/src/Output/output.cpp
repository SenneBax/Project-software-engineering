/**
 * @file output.cpp
 * @author senne
 * @date 24/04/2025
 * @brief Implementation of the output class (Revised with graphical impressions)
 */

#include "output.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../Parser/tinyxml.h"
#include "DesignByContract.h"

/**
 * @brief Constructor
 */
output::output() : lastFoutmelding("")
{
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor moet eindingen in een geldige toestand.");

}

bool output::properlyInitialized() const
{
    return _initCheck == this;
}

/**
 * @brief Generates a textual representation of the traffic situation
 * @param situatie The traffic situation to be displayed
 * @return A string with the textual representation
 */
std::string output::genereerTekstRapport(const VerkeersSituatie& situatie) {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
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
        ss << " - " << voertuig.getType() << " op baan '" << voertuig.getBaanNaam()
           << "' (positie: " << voertuig.getPositie()
           << "m, snelheid: " << voertuig.getSnelheid() << "m/s)" << std::endl;
    }

    const auto& verkeerslichten = situatie.getVerkeerslichten();
    ss << "Verkeerslichten (" << verkeerslichten.size() << "):" << std::endl;
    for (const auto& licht : verkeerslichten) {
        ss << " - Verkeerslicht op baan '" << licht.getBaan()
           << "' (positie: " << licht.getPositie()
           << "m, cyclus: " << licht.getCyclus()
           << "s, kleur: " << licht.getKleurAsString() << ")" << std::endl;
    }

    const auto& bushaltes = situatie.getBushaltes();
    ss << "Bushaltes (" << bushaltes.size() << "):" << std::endl;
    for (const auto& halte : bushaltes) {
        ss << " - Bushalte op baan '" << halte.getBaan()
           << "' (positie: " << halte.getPositie()
           << "m, wachttijd: " << halte.getWachttijd() << "s)" << std::endl;
    }

    const auto& kruispunten = situatie.getKruispunten();
    ss << "Kruispunten (" << kruispunten.size() << "):" << std::endl;
    for (size_t i = 0; i < kruispunten.size(); i++) {
        const auto& kruispunt = kruispunten[i];
        ss << " - Kruispunt " << i+1 << " verbindt:" << std::endl;

        for (const auto& baanPaar : kruispunt.getBanen()) {
            ss << "   * Baan '" << baanPaar.first
               << "' op positie " << baanPaar.second << "m" << std::endl;
        }
    }

    return ss.str();
}

/**
 * @brief Generates a graphical impression of the traffic situation (ASCII art)
 * @param situatie The traffic situation to be displayed
 * @return A string with the graphical impression
 */
std::string output::genereerGrafischeImpressie(const VerkeersSituatie& situatie) {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");

    std::stringstream ss;

    const auto& banen = situatie.getBanen();
    const auto& voertuigen = situatie.getVoertuigen();
    const auto& verkeerslichten = situatie.getVerkeerslichten();
    const auto& bushaltes = situatie.getBushaltes();

    // Bepaal langste baannaam voor uitlijning.
    size_t maxBaanNaamLenghte = 0;
    for (const auto& baanPaar : banen)
    {
        maxBaanNaamLenghte = std::max(maxBaanNaamLenghte, baanPaar.first.length());
    }

    // Process each road
    for (const auto& baanPaar : banen) {
        const std::string& baanNaam = baanPaar.first;
        const Baan& baan = baanPaar.second;
        const int baanLengte = baan.getLengte();

        // Creëer een schaalfactor om de weergave passend te maken
        const int maxDispLen = 80; // Maximale weergavelengte
        double schaalfactor = 1.0;
        if (baanLengte > maxDispLen) {
            schaalfactor = static_cast<double>(maxDispLen) / baanLengte;
        }

        const int displayLengte = std::max(1, static_cast<int>(baanLengte * schaalfactor));

        // Create a string representation of the road
        std::string baanVisualisatie(displayLengte, '=');

        // Mark vehicles on the road
        for (const auto& voertuig : voertuigen) {
            if (voertuig.getBaanNaam() == baanNaam) {
                int positie = static_cast<int>(voertuig.getPositie() * schaalfactor);
                if (positie >= 0 && positie < displayLengte) {
                    // Mark different vehicle types with different characters
                    if (voertuig.getType() == "auto") {
                        baanVisualisatie[positie] = 'A';
                    } else if (voertuig.getType() == "bus") {
                        baanVisualisatie[positie] = 'B';
                    } else if (voertuig.getType() == "brandweerwagen") {
                        baanVisualisatie[positie] = 'F'; // F for fire truck
                    } else if (voertuig.getType() == "ziekenwagen") {
                        baanVisualisatie[positie] = 'Z';
                    } else if (voertuig.getType() == "politiecombi") {
                        baanVisualisatie[positie] = 'P';
                    }
                }
            }
        }

        // Create a list of traffic lights on this road
        std::vector<std::pair<int, char>> verkeerslichtPosities;
        for (const auto& licht : verkeerslichten) {
            if (licht.getBaan() == baanNaam) {
                int positie = static_cast<int>(licht.getPositie() * schaalfactor);
                if (positie >= 0 && positie < displayLengte) {
                    char kleur = licht.isGroen() ? 'G' : (licht.isOranje() ? 'O' : 'R');
                    verkeerslichtPosities.push_back(std::make_pair(positie, kleur));
                }
            }
        }

        // Create a list of bus stops on this road
        std::vector<int> bushaltePosities;
        for (const auto& halte : bushaltes) {
            if (halte.getBaan() == baanNaam) {
                int positie = static_cast<int>(halte.getPositie() * schaalfactor);
                if (positie >= 0 && positie < displayLengte) {
                    bushaltePosities.push_back(positie);
                }
            }
        }

        // Create a list of intersections on this road
        std::vector<int> kruispuntPosities;
        for (const auto& kruispunt : situatie.getKruispunten()) {
            if (kruispunt.bevatBaan(baanNaam)) {
                int positie = static_cast<int>(kruispunt.getPositieOpBaan(baanNaam) * schaalfactor);
                if (positie >= 0 && positie < displayLengte) {
                    kruispuntPosities.push_back(positie);
                }
            }
        }

        // Markeer kruispunten in de weergave
        for (int pos : kruispuntPosities) {
            if (pos >= 0 && pos < displayLengte) {
                baanVisualisatie[pos] = '+';
            }
        }

        // Print the road name and visualization
        ss << std::left << std::setw(maxBaanNaamLenghte) << baanNaam << " | " << baanVisualisatie << std::endl;

        // Print traffic light information if there are any on this road
        if (!verkeerslichtPosities.empty()) {
            ss << std::left << std::setw(maxBaanNaamLenghte) << "> Verkeerslichten" << " | ";
            for (int i = 0; i < displayLengte; i++) {
                bool found = false;
                for (const auto& lichtPaar : verkeerslichtPosities) {
                    if (lichtPaar.first == i) {
                        ss << lichtPaar.second;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ss << " ";
                }
            }
            ss << std::endl;
        }

        // Print bus stop information if there are any on this road
        if (!bushaltePosities.empty()) {
            ss << std::left << std::setw(maxBaanNaamLenghte) << "> bushaltes" << " | ";
            for (int i = 0; i < displayLengte; i++) {
                bool found = false;
                for (int haltePositie : bushaltePosities) {
                    if (haltePositie == i) {
                        ss << "B";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ss << " ";
                }
            }
            ss << std::endl;
        }

        // Print intersection information if there are any on this road
        if (!kruispuntPosities.empty()) {
            ss << std::left << std::setw(maxBaanNaamLenghte) << "> kruispunten" << " | ";
            for (int i = 0; i < displayLengte; i++) {
                bool found = false;
                for (int kruisPunt : kruispuntPosities) {
                    if (kruisPunt == i) {
                        ss << "+";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ss << " ";
                }
            }
            ss << std::endl;
        }

        // Add a blank line between roads
        ss << std::endl;
    }

    // Legende
    ss << "Legende:" << std::endl;
    ss << " A: Auto          G: Groen verkeerslicht" << std::endl;
    ss << " B: Bus           O: Oranje verkeerslicht" << std::endl;
    ss << " F: Brandweerwagen R: Rood verkeerslicht" << std::endl;
    ss << " Z: Ziekenwagen   B: Bushalte" << std::endl;
    ss << " P: Politiecombi  +: Kruispunt" << std::endl;

    return ss.str();
}

/**
 * @brief Writes a traffic situation to an XML file
 * @param situatie The traffic situation to be written
 * @param bestandsnaam Path to the XML file
 * @return true if writing was successful, false otherwise
 */
bool output::schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandsNaam mag niet leeg zijn.");
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    doc.LinkEndChild(decl);

    TiXmlElement* root = new TiXmlElement("VerkeersSituatie");
    doc.LinkEndChild(root);

    // Write roads
    for (const auto& paar : situatie.getBanen()) {
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

    // Write vehicles
    for (const auto& voertuig : situatie.getVoertuigen()) {
        TiXmlElement* voertuigElement = new TiXmlElement("VOERTUIG");

        TiXmlElement* baanElement = new TiXmlElement("baan");
        TiXmlText* baanText = new TiXmlText(voertuig.getBaanNaam().c_str());
        baanElement->LinkEndChild(baanText);
        voertuigElement->LinkEndChild(baanElement);

        TiXmlElement* positieElement = new TiXmlElement("positie");
        std::stringstream ssPos;
        ssPos << voertuig.getPositie();
        TiXmlText* positieText = new TiXmlText(ssPos.str().c_str());
        positieElement->LinkEndChild(positieText);
        voertuigElement->LinkEndChild(positieElement);

        // Add type if it's not the default "auto"
        if (voertuig.getType() != "auto") {
            TiXmlElement* typeElement = new TiXmlElement("type");
            TiXmlText* typeText = new TiXmlText(voertuig.getType().c_str());
            typeElement->LinkEndChild(typeText);
            voertuigElement->LinkEndChild(typeElement);
        }

        root->LinkEndChild(voertuigElement);
    }

    // Write traffic lights
    for (const auto& licht : situatie.getVerkeerslichten()) {
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

        // Add additional attributes if needed
        if (licht.getHeeftOranje()) {
            TiXmlElement* oranjeElement = new TiXmlElement("oranje");
            TiXmlText* oranjeText = new TiXmlText("true");
            oranjeElement->LinkEndChild(oranjeText);
            lichtElement->LinkEndChild(oranjeElement);
        }

        if (licht.getIsSlim()) {
            TiXmlElement* slimElement = new TiXmlElement("slim");
            TiXmlText* slimText = new TiXmlText("true");
            slimElement->LinkEndChild(slimText);
            lichtElement->LinkEndChild(slimElement);
        }

        root->LinkEndChild(lichtElement);
    }

    // Write bus stops
    for (const auto& halte : situatie.getBushaltes()) {
        TiXmlElement* halteElement = new TiXmlElement("BUSHALTE");

        TiXmlElement* baanElement = new TiXmlElement("baan");
        TiXmlText* baanText = new TiXmlText(halte.getBaan().c_str());
        baanElement->LinkEndChild(baanText);
        halteElement->LinkEndChild(baanElement);

        TiXmlElement* positieElement = new TiXmlElement("positie");
        std::stringstream ssPos;
        ssPos << halte.getPositie();
        TiXmlText* positieText = new TiXmlText(ssPos.str().c_str());
        positieElement->LinkEndChild(positieText);
        halteElement->LinkEndChild(positieElement);

        TiXmlElement* wachttijdElement = new TiXmlElement("wachttijd");
        std::stringstream ssWacht;
        ssWacht << halte.getWachttijd();
        TiXmlText* wachttijdText = new TiXmlText(ssWacht.str().c_str());
        wachttijdElement->LinkEndChild(wachttijdText);
        halteElement->LinkEndChild(wachttijdElement);

        root->LinkEndChild(halteElement);
    }

    // Write vehicle generators
    for (const auto& generator : situatie.getVoertuigGenerators()) {
        TiXmlElement* generatorElement = new TiXmlElement("VOERTUIGGENERATOR");

        TiXmlElement* baanElement = new TiXmlElement("baan");
        TiXmlText* baanText = new TiXmlText(generator.getBaanNaam().c_str());
        baanElement->LinkEndChild(baanText);
        generatorElement->LinkEndChild(baanElement);

        TiXmlElement* frequentieElement = new TiXmlElement("frequentie");
        std::stringstream ssFreq;
        ssFreq << generator.getFrequentie();
        TiXmlText* frequentieText = new TiXmlText(ssFreq.str().c_str());
        frequentieElement->LinkEndChild(frequentieText);
        generatorElement->LinkEndChild(frequentieElement);

        // Add type if it's not the default "auto"
        if (generator.getType() != "auto") {
            TiXmlElement* typeElement = new TiXmlElement("type");
            TiXmlText* typeText = new TiXmlText(generator.getType().c_str());
            typeElement->LinkEndChild(typeText);
            generatorElement->LinkEndChild(typeElement);
        }

        root->LinkEndChild(generatorElement);
    }

    // Write intersections
    for (const auto& kruispunt : situatie.getKruispunten()) {
        TiXmlElement* kruispuntElement = new TiXmlElement("KRUISPUNT");

        for (const auto& baanPaar : kruispunt.getBanen()) {
            TiXmlElement* baanElement = new TiXmlElement("baan");

            // Add position as attribute
            std::stringstream ssPos;
            ssPos << baanPaar.second;
            baanElement->SetAttribute("positie", ssPos.str().c_str());

            // Add road name as text
            TiXmlText* baanText = new TiXmlText(baanPaar.first.c_str());
            baanElement->LinkEndChild(baanText);

            kruispuntElement->LinkEndChild(baanElement);
        }

        root->LinkEndChild(kruispuntElement);
    }

    // Write the file
    bool saveSuccess = doc.SaveFile(bestandsnaam.c_str());
    if (!saveSuccess) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet schrijven";
        return false;
    }

    return true;
}

/**
 * @brief Generates an HTML representation of the traffic situation
 * @param situatie The traffic situation to be displayed
 * @param bestandsnaam Path to the HTML file
 * @return true if writing was successful, false otherwise
 */
bool output::schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");
    std::ofstream file(bestandsnaam);
    if (!file.is_open()) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }

    // schrijft HTML header gedaan met AI maar persoonlijk nagekeken op juistheid
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Verkeerssimulatie</title>\n"
         << "    <meta charset=\"UTF-8\">\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
         << "        h1 { color: #333; }\n"
         << "        .road { position: relative; margin-bottom: 30px; }\n"
         << "        .road-name { font-weight: bold; margin-bottom: 5px; }\n"
         << "        .road-line { height: 10px; background-color: #ccc; position: relative; }\n"
         << "        .vehicle { position: absolute; width: 20px; height: 20px; top: -5px; text-align: center; line-height: 20px; font-weight: bold; border-radius: 10px; }\n"
         << "        .auto { background-color: blue; color: white; }\n"
         << "        .bus { background-color: green; color: white; }\n"
         << "        .brandweerwagen { background-color: red; color: white; }\n"
         << "        .ziekenwagen { background-color: white; color: red; border: 1px solid red; }\n"
         << "        .politiecombi { background-color: #000066; color: white; }\n"
         << "        .traffic-light { position: absolute; width: 10px; height: 20px; top: -5px; }\n"
         << "        .traffic-light-red { background-color: red; }\n"
         << "        .traffic-light-orange { background-color: orange; }\n"
         << "        .traffic-light-green { background-color: green; }\n"
         << "        .bus-stop { position: absolute; width: 10px; height: 10px; top: -5px; background-color: yellow; border: 1px solid black; }\n"
         << "        .intersection { position: absolute; width: 10px; height: 10px; top: 0px; background-color: black; border-radius: 5px; }\n"
         << "        .legend { margin-top: 30px; border: 1px solid #ccc; padding: 10px; }\n"
         << "        .legend-item { display: inline-block; margin-right: 20px; }\n"
         << "        .legend-color { display: inline-block; width: 15px; height: 15px; margin-right: 5px; vertical-align: middle; }\n"
         << "        .statistics { margin-top: 20px; border: 1px solid #ccc; padding: 10px; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Verkeerssimulatie</h1>\n";

    // Write roads
    for (const auto& baanPaar : situatie.getBanen()) {
        const std::string& baanNaam = baanPaar.first;
        const Baan& baan = baanPaar.second;
        const int baanLengte = baan.getLengte();

        // Start road div
        file << "    <div class=\"road\">\n"
             << "        <div class=\"road-name\">" << baanNaam << " (lengte: " << baanLengte << "m)</div>\n"
             << "        <div class=\"road-line\" style=\"width: " << std::min(1000, baanLengte) << "px;\">\n";

        // Schaalfactor voor grote wegen
        double schaalfactor = 1.0;
        if (baanLengte > 1000) {
            schaalfactor = 1000.0 / baanLengte;
        }

        // Add vehicles
        for (const auto& voertuig : situatie.getVoertuigen()) {
            if (voertuig.getBaanNaam() == baanNaam) {
                int positie = static_cast<int>(voertuig.getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    std::string type = voertuig.getType();
                    std::string label;

                    if (type == "auto") label = "A";
                    else if (type == "bus") label = "B";
                    else if (type == "brandweerwagen") label = "F";
                    else if (type == "ziekenwagen") label = "Z";
                    else if (type == "politiecombi") label = "P";

                    file << "            <div class=\"vehicle " << type << "\" style=\"left: " << positie << "px;\" title=\"" << type << " op " << voertuig.getPositie() << "m\">" << label << "</div>\n";
                }
            }
        }

        // Add traffic lights
        for (const auto& licht : situatie.getVerkeerslichten()) {
            if (licht.getBaan() == baanNaam) {
                int positie = static_cast<int>(licht.getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    std::string kleur;
                    if (licht.isGroen()) kleur = "green";
                    else if (licht.isOranje()) kleur = "orange";
                    else kleur = "red";

                    file << "            <div class=\"traffic-light traffic-light-" << kleur << "\" style=\"left: " << positie << "px;\" title=\"Verkeerslicht (" << kleur << ") op " << licht.getPositie() << "m\"></div>\n";
                }
            }
        }

        // Add bus stops
        for (const auto& halte : situatie.getBushaltes()) {
            if (halte.getBaan() == baanNaam) {
                int positie = static_cast<int>(halte.getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    file << "            <div class=\"bus-stop\" style=\"left: " << positie << "px;\" title=\"Bushalte op " << halte.getPositie() << "m, wachttijd: " << halte.getWachttijd() << "s\"></div>\n";
                }
            }
        }

        // Add intersections
        for (const auto& kruispunt : situatie.getKruispunten()) {
            if (kruispunt.bevatBaan(baanNaam)) {
                int positie = static_cast<int>(kruispunt.getPositieOpBaan(baanNaam) * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    file << "            <div class=\"intersection\" style=\"left: " << positie << "px;\" title=\"Kruispunt op " << kruispunt.getPositieOpBaan(baanNaam) << "m\"></div>\n";
                }
            }
        }

        // End road div
        file << "        </div>\n"
             << "    </div>\n";
    }

    // Write legend
    file << "    <div class=\"legend\">\n"
         << "        <h3>Legende</h3>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color auto\"></div> Auto</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color bus\"></div> Bus</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color brandweerwagen\"></div> Brandweerwagen</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color ziekenwagen\"></div> Ziekenwagen</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color politiecombi\"></div> Politiecombi</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-red\"></div> Rood licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-orange\"></div> Oranje licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-green\"></div> Groen licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color bus-stop\" style=\"position: static;\"></div> Bushalte</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color intersection\" style=\"position: static;\"></div> Kruispunt</div>\n"
         << "    </div>\n";

    // Write statistics
    file << "    <div class=\"statistics\">\n"
         << "        <h3>Statistieken</h3>\n"
         << "        <p>Aantal banen: " << situatie.getBanen().size() << "</p>\n"
         << "        <p>Aantal voertuigen: " << situatie.getVoertuigen().size() << "</p>\n"
         << "        <p>Aantal verkeerslichten: " << situatie.getVerkeerslichten().size() << "</p>\n"
         << "        <p>Aantal bushaltes: " << situatie.getBushaltes().size() << "</p>\n"
         << "        <p>Aantal kruispunten: " << situatie.getKruispunten().size() << "</p>\n"
         << "    </div>\n";

    // HTML footer
    file << "</body>\n"
         << "</html>\n";

    file.close();
    return true;
}

    /**
    * @brief Get the last error message
    * @return The last error message
    */
    std::string output::getLastFoutmelding() const {
    REQUIRE(properlyInitialized(), "Output niet correct ingesteld");

        return lastFoutmelding;
}