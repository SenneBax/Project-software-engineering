/**
 * @file output.cpp
 * @brief Implementatie van de output klasse
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
 * @post properlyInitialized() == true
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
 * @brief Genereert een genereerTekstRapport van de verkeerssituatie
 * @param situatie De weer te geven verkeerssituatie
 * @return Een string met de tekstuele weergave
 * @pre situatie.properlyInitialized() == true
 */
std::string output::genereerTekstRapport(const VerkeersSituatie& situatie) const {
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
        ss << " - " << voertuig->getType() << " op baan '" << voertuig->getBaanNaam()
           << "' (positie: " << voertuig->getPositie()
           << "m, snelheid: " << voertuig->getSnelheid() << "m/s)" << std::endl;
    }

    const auto& verkeerslichten = situatie.getVerkeerslichten();
    ss << "Verkeerslichten (" << verkeerslichten.size() << "):" << std::endl;
    for (const auto& licht : verkeerslichten) {
        ss << " - Verkeerslicht op baan '" << licht.getBaan()
           << "' (positie: " << licht.getPositie()
           << "m, cyclus: " << licht.getCyclus()
           << "s, kleur: " << licht.getKleurAsString();

        if (licht.getIsSlim()) {
            ss << ", slim";
            if (licht.getTotaalVoertuigenVoorLicht() > 0) {
                ss << ", " << licht.getTotaalVoertuigenVoorLicht() << " wachtend";
            }
        }
        ss << ")" << std::endl;
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
 * @brief Genereert een genereerGrafischeImpressie van de verkeerssituatie 
 * @param situatie De weer te geven verkeerssituatie
 * @return Een string met de grafische impressie
 * @pre situatie.properlyInitialized() == true
 */
std::string output::genereerGrafischeImpressie(const VerkeersSituatie& situatie) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");

    std::stringstream ss;

    const auto& banen = situatie.getBanen();
    const auto& voertuigen = situatie.getVoertuigen();
    const auto& verkeerslichten = situatie.getVerkeerslichten();
    const auto& bushaltes = situatie.getBushaltes();
    const auto& kruispunten = situatie.getKruispunten();

    // Bepaal langste baannaam voor uitlijning
    size_t maxNaamLengte = 0;
    for (const auto& paar : banen) {
        maxNaamLengte = std::max(maxNaamLengte, paar.first.length());
    }

    // Minimale breedte voor de weergave
    const int MIN_BAAN_BREEDTE = 80;

    for (const auto& paar : banen) {
        const std::string& baanNaam = paar.first;
        const Baan& baan = paar.second;
        const int baanLengte = baan.getLengte();

        // Bepaal schaalfactor om baan in 80 karakters te passen
        double schaalfactor = static_cast<double>(MIN_BAAN_BREEDTE) / baanLengte;
        if (schaalfactor > 1.0) schaalfactor = 1.0; // Niet groter maken dan werkelijke grootte

        // Maak een array voor de baan visualisatie
        std::vector<char> baanVisualisatie(MIN_BAAN_BREEDTE, '=');

        // Plaats voertuigen op de baan
        for (const auto& voertuig : voertuigen) {
            if (voertuig->getBaanNaam() == baanNaam) {
                int positie = static_cast<int>(voertuig->getPositie() * schaalfactor);
                if (positie >= 0 && positie < MIN_BAAN_BREEDTE) {
                    char symbool = '?';
                    std::string type = voertuig->getType();

                    if (type == "auto") symbool = 'A';
                    else if (type == "bus") symbool = 'B';
                    else if (type == "brandweerwagen") symbool = 'F';
                    else if (type == "ziekenwagen") symbool = 'Z';
                    else if (type == "politiecombi") symbool = 'P';

                    baanVisualisatie[positie] = symbool;
                }
            }
        }

        // Plaats verkeerslichten op de baan (in de lijn!)
        for (const auto& licht : verkeerslichten) {
            if (licht.getBaan() == baanNaam) {
                int positie = static_cast<int>(licht.getPositie() * schaalfactor);
                if (positie >= 0 && positie < MIN_BAAN_BREEDTE) {
                    char kleurSymbool = '?';
                    std::string kleur = licht.getKleurAsString();

                    if (kleur == "groen") kleurSymbool = 'G';
                    else if (kleur == "oranje") kleurSymbool = 'O';
                    else if (kleur == "rood") kleurSymbool = 'R';

                    baanVisualisatie[positie] = kleurSymbool;
                }
            }
        }

        // Plaats bushaltes op de baan
        for (const auto& halte : bushaltes) {
            if (halte.getBaan() == baanNaam) {
                int positie = static_cast<int>(halte.getPositie() * schaalfactor);
                if (positie >= 0 && positie < MIN_BAAN_BREEDTE) {
                    baanVisualisatie[positie] = 'H'; // H voor Halte
                }
            }
        }

        // Plaats kruispunten op de baan
        for (const auto& kruispunt : kruispunten) {
            auto kruispuntBanen = kruispunt.getBanen();
            for (const auto& kruispuntBaan : kruispuntBanen) {
                if (kruispuntBaan.first == baanNaam) {
                    int positie = static_cast<int>(kruispuntBaan.second * schaalfactor);
                    if (positie >= 0 && positie < MIN_BAAN_BREEDTE) {
                        baanVisualisatie[positie] = '+'; // + voor kruispunt
                    }
                }
            }
        }

        // Print de baan met uitlijning
        ss << std::left << std::setw(maxNaamLengte + 1) << baanNaam << "| ";
        for (char c : baanVisualisatie) {
            ss << c;
        }
        ss << std::endl;
    }

    // Legende
    ss << std::endl << "Legende:" << std::endl;
    ss << "  A=Auto, B=Bus, F=Brandweer, Z=Ziekenwagen, P=Politie" << std::endl;
    ss << "  G=Groen licht, O=Oranje licht, R=Rood licht" << std::endl;
    ss << "  H=Bushalte, +=Kruispunt, ==Weg" << std::endl;

    return ss.str();
}

/**
 * @brief Maakt een grafische impressie met details
 * @param situatie De weer te geven verkeerssituatie
 * @pre situatie.properlyInitialized() == true
 */
void output::maakGrafischeImpressie(const VerkeersSituatie& situatie) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");

    // Toon grafische impressie
    std::cout << genereerGrafischeImpressie(situatie) << std::endl;

    // Toon scheidingslijn
    std::cout << "-------------------" << std::endl;

    // Toon voertuigdetails met verkeerslicht informatie
    const auto& voertuigen = situatie.getVoertuigen();
    const auto& verkeerslichten = situatie.getVerkeerslichten();

    if (!voertuigen.empty()) {
        for (const auto& voertuig : voertuigen) {
            std::cout << "  " << voertuig->getType()
                      << " op " << voertuig->getBaanNaam()
                      << " @ " << std::fixed << std::setprecision(1) << voertuig->getPositie() << "m"
                      << " (snelheid: " << std::setprecision(1) << voertuig->getSnelheid() << " m/s)";

            // Zoek verkeerslichten op dezelfde baan
            std::vector<std::string> lichtenInfo;
            for (const auto& licht : verkeerslichten) {
                if (licht.getBaan() == voertuig->getBaanNaam()) {
                    double afstand = licht.getPositie() - voertuig->getPositie();

                    // Alleen tonen als verkeerslicht voor het voertuig ligt (binnen 200m)
                    if (afstand > 0 && afstand <= 200.0) {
                        std::stringstream lichtInfo;
                        lichtInfo << licht.getKleurAsString() << " licht @ "
                                  << std::fixed << std::setprecision(1) << licht.getPositie() << "m";

                        // Extra info voor slimme lichten (gebruik bestaande functies)
                        if (licht.getIsSlim()) {
                            lichtInfo << " (slim";

                            // Gebruik alleen bestaande functies die beschikbaar zijn
                            try {
                                int wachtenden = licht.getVoertuigenVoorLicht();
                                if (wachtenden > 0) {
                                    lichtInfo << ", " << wachtenden << " wachtend";
                                }
                            } catch (...) {
                                // Functie mogelijk niet beschikbaar
                            }

                            lichtInfo << ")";
                        }

                        lichtenInfo.push_back(lichtInfo.str());
                    }
                }
            }

            // Voeg verkeerslicht info toe als die er is
            if (!lichtenInfo.empty()) {
                std::cout << " -> ";
                for (size_t i = 0; i < lichtenInfo.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << lichtenInfo[i];
                }
            }

            std::cout << std::endl;
        }
    } else {
        std::cout << "  Geen voertuigen in de simulatie." << std::endl;
    }

    // Toon verkeerslichten status als er voertuigen zijn die wachten
    bool heeftWachtendeVoertuigen = false;
    for (const auto& licht : verkeerslichten) {
        if (licht.getIsSlim()) {
            try {
                int wachtenden = licht.getVoertuigenVoorLicht();
                if (wachtenden > 0) {
                    if (!heeftWachtendeVoertuigen) {
                        std::cout << std::endl << "Slimme verkeerslichten met wachtende voertuigen:" << std::endl;
                        heeftWachtendeVoertuigen = true;
                    }

                    std::cout << "  " << licht.getBaan() << " @ " << licht.getPositie() << "m: "
                              << licht.getKleurAsString() << " licht, "
                              << wachtenden << " voertuigen wachtend"
                              << std::endl;
                }
            } catch (...) {

            }
        }
    }

    // Toon altijd alle verkeerslichten met hun status
    if (!verkeerslichten.empty()) {
        std::cout << std::endl << "Alle verkeerslichten:" << std::endl;
        for (const auto& licht : verkeerslichten) {
            try {
                std::cout << "  " << licht.getBaan() << " @ "
                          << std::fixed << std::setprecision(1) << licht.getPositie() << "m: "
                          << licht.getKleurAsString() << " licht";

                if (licht.getIsSlim()) {
                    std::cout << " (slim, cyclus: " << licht.getCyclus() << "s)";
                } else {
                    std::cout << " (normaal, cyclus: " << licht.getCyclus() << "s)";
                }

                std::cout << std::endl;
            } catch (...) {
                // Bij errors, toon minimale info
                std::cout << "  Verkeerslicht op " << licht.getBaan() << std::endl;
            }
        }
    }

    std::cout << "-------------------" << std::endl;
}

/**
 * @brief Toon een tekstrapport op stdout
 * @param situatie De weer te geven verkeerssituatie
 * @pre situatie.properlyInitialized() == true
 */
void output::maakTekstRapport(const VerkeersSituatie& situatie) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
    std::cout << genereerTekstRapport(situatie) << std::endl;
}

/**
 * @brief Schrijf de verkeerssituatie naar een XML bestand
 * @param situatie De te schrijven verkeerssituatie
 * @param bestandsnaam Naam van het uit te schrijven bestand
 * @return true als schrijven succesvol was, false anders
 * @pre situatie.properlyInitialized() == true
 * @pre !bestandsnaam.empty()
 */
bool output::schrijfNaarXml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");

    std::ofstream file(bestandsnaam);
    if (!file.is_open()) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    file << "<VERKEERSSITUATIE>" << std::endl;

    // Schrijf banen
    const auto& banen = situatie.getBanen();
    for (const auto& paar : banen) {
        const Baan& baan = paar.second;
        file << "    <BAAN>" << std::endl;
        file << "        <naam>" << baan.getNaam() << "</naam>" << std::endl;
        file << "        <lengte>" << baan.getLengte() << "</lengte>" << std::endl;
        file << "    </BAAN>" << std::endl;
    }

    // Schrijf voertuigen
    const auto& voertuigen = situatie.getVoertuigen();
    for (const auto& voertuig : voertuigen) {
        file << "    <VOERTUIG>" << std::endl;
        file << "        <type>" << voertuig->getType() << "</type>" << std::endl;
        file << "        <baan>" << voertuig->getBaanNaam() << "</baan>" << std::endl;
        file << "        <positie>" << voertuig->getPositie() << "</positie>" << std::endl;
        file << "        <snelheid>" << voertuig->getSnelheid() << "</snelheid>" << std::endl;
        file << "    </VOERTUIG>" << std::endl;
    }

    // Schrijf verkeerslichten
    const auto& verkeerslichten = situatie.getVerkeerslichten();
    for (const auto& licht : verkeerslichten) {
        file << "    <VERKEERSLICHT>" << std::endl;
        file << "        <baan>" << licht.getBaan() << "</baan>" << std::endl;
        file << "        <positie>" << licht.getPositie() << "</positie>" << std::endl;
        file << "        <cyclus>" << licht.getCyclus() << "</cyclus>" << std::endl;
        file << "        <kleur>" << licht.getKleurAsString() << "</kleur>" << std::endl;
        file << "        <oranje>" << (licht.getHeeftOranje() ? "true" : "false") << "</oranje>" << std::endl;
        file << "        <slim>" << (licht.getIsSlim() ? "true" : "false") << "</slim>" << std::endl;
        if (licht.getIsSlim()) {
            file << "        <voertuigen_wachtend>" << licht.getVoertuigenVoorLicht() << "</voertuigen_wachtend>" << std::endl;
            file << "        <prioriteit_wachtend>" << licht.getPrioriteitsVoertuigenVoorLicht() << "</prioriteit_wachtend>" << std::endl;
        }
        file << "    </VERKEERSLICHT>" << std::endl;
    }

    // Schrijf bushaltes
    const auto& bushaltes = situatie.getBushaltes();
    for (const auto& halte : bushaltes) {
        file << "    <BUSHALTE>" << std::endl;
        file << "        <baan>" << halte.getBaan() << "</baan>" << std::endl;
        file << "        <positie>" << halte.getPositie() << "</positie>" << std::endl;
        file << "        <wachttijd>" << halte.getWachttijd() << "</wachttijd>" << std::endl;
        file << "    </BUSHALTE>" << std::endl;
    }

    // Schrijf kruispunten
    const auto& kruispunten = situatie.getKruispunten();
    for (const auto& kruispunt : kruispunten) {
        file << "    <KRUISPUNT>" << std::endl;
        for (const auto& baanPaar : kruispunt.getBanen()) {
            file << "        <baan positie=\"" << baanPaar.second << "\">" << baanPaar.first << "</baan>" << std::endl;
        }
        file << "    </KRUISPUNT>" << std::endl;
    }

    file << "</VERKEERSSITUATIE>" << std::endl;
    file.close();
    return true;
}

/**
 * @brief Schrijf de verkeerssituatie naar een HTML bestand
 * @param situatie De te schrijven verkeerssituatie
 * @param bestandsnaam Naam van het uit te schrijven bestand
 * @return true als schrijven succesvol was, false anders
 * @pre situatie.properlyInitialized() == true
 * @pre !bestandsnaam.empty()
 */
bool output::schrijfNaarHtml(const VerkeersSituatie& situatie, const std::string& bestandsnaam) const {
    REQUIRE(situatie.properlyInitialized(), "situatie niet correct ingesteld");
    REQUIRE(!bestandsnaam.empty(), "BestandNaam mag niet leeg zijn.");

    std::ofstream file(bestandsnaam);
    if (!file.is_open()) {
        lastFoutmelding = "Kan bestand '" + bestandsnaam + "' niet openen";
        return false;
    }

    // HTML header met verbeterde styling
    file << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Verkeerssimulatie</title>\n"
         << "    <meta charset=\"UTF-8\">\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n"
         << "        h1 { color: #333; text-align: center; }\n"
         << "        .road { position: relative; margin-bottom: 30px; background: white; padding: 10px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n"
         << "        .road-name { font-weight: bold; margin-bottom: 5px; font-size: 14px; }\n"
         << "        .road-line { height: 10px; background-color: #ccc; position: relative; border-radius: 5px; }\n"
         << "        .vehicle { position: absolute; width: 20px; height: 20px; top: -5px; text-align: center; line-height: 20px; font-weight: bold; border-radius: 10px; font-size: 12px; }\n"
         << "        .auto { background-color: blue; color: white; }\n"
         << "        .bus { background-color: green; color: white; }\n"
         << "        .brandweerwagen { background-color: red; color: white; }\n"
         << "        .ziekenwagen { background-color: white; color: red; border: 2px solid red; }\n"
         << "        .politiecombi { background-color: #000066; color: white; }\n"
         << "        .traffic-light { position: absolute; width: 12px; height: 24px; top: -7px; border-radius: 6px; border: 1px solid #333; }\n"
         << "        .traffic-light-red { background-color: #ff0000; }\n"
         << "        .traffic-light-orange { background-color: #ffa500; }\n"
         << "        .traffic-light-green { background-color: #00ff00; }\n"
         << "        .bus-stop { position: absolute; width: 10px; height: 10px; top: -5px; background-color: yellow; border: 1px solid black; }\n"
         << "        .intersection { position: absolute; width: 10px; height: 10px; top: 0px; background-color: black; border-radius: 5px; }\n"
         << "        .legend { margin-top: 30px; border: 1px solid #ccc; padding: 15px; background: white; border-radius: 8px; }\n"
         << "        .legend-item { display: inline-block; margin-right: 20px; margin-bottom: 5px; }\n"
         << "        .legend-color { display: inline-block; width: 15px; height: 15px; margin-right: 5px; vertical-align: middle; border-radius: 3px; }\n"
         << "        .statistics { margin-top: 20px; border: 1px solid #ccc; padding: 15px; background: white; border-radius: 8px; }\n"
         << "        .details { margin-top: 20px; border: 1px solid #ccc; padding: 15px; background: white; border-radius: 8px; }\n"
         << "        .vehicle-detail { margin: 5px 0; padding: 5px; background: #f9f9f9; border-radius: 4px; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Verkeerssimulatie Visualisatie</h1>\n";

    // Schrijf banen met alle elementen
    for (const auto& baanPaar : situatie.getBanen()) {
        const std::string& baanNaam = baanPaar.first;
        const Baan& baan = baanPaar.second;
        const int baanLengte = baan.getLengte();

        // Start baan div
        file << "    <div class=\"road\">\n"
             << "        <div class=\"road-name\">" << baanNaam << " (lengte: " << baanLengte << "m)</div>\n"
             << "        <div class=\"road-line\" style=\"width: " << std::min(1000, baanLengte) << "px;\">\n";

        // Schaalfactor voor grote wegen
        double schaalfactor = 1.0;
        if (baanLengte > 1000) {
            schaalfactor = 1000.0 / baanLengte;
        }

        // Voeg voertuigen toe
        for (const auto& voertuig : situatie.getVoertuigen()) {
            if (voertuig->getBaanNaam() == baanNaam) {
                int positie = static_cast<int>(voertuig->getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    std::string type = voertuig->getType();
                    std::string label;

                    if (type == "auto") label = "A";
                    else if (type == "bus") label = "B";
                    else if (type == "brandweerwagen") label = "F";
                    else if (type == "ziekenwagen") label = "Z";
                    else if (type == "politiecombi") label = "P";

                    file << "            <div class=\"vehicle " << type << "\" style=\"left: " << positie << "px;\" title=\"" << type << " op " << voertuig->getPositie() << "m, snelheid: " << voertuig->getSnelheid() << "m/s\">" << label << "</div>\n";
                }
            }
        }

        // Voeg verkeerslichten toe
        for (const auto& licht : situatie.getVerkeerslichten()) {
            if (licht.getBaan() == baanNaam) {
                int positie = static_cast<int>(licht.getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    std::string kleur = licht.getKleurAsString();
                    std::string kleurClass = "traffic-light-red";
                    if (kleur == "groen") kleurClass = "traffic-light-green";
                    else if (kleur == "oranje") kleurClass = "traffic-light-orange";

                    std::string title = "Verkeerslicht: " + kleur + ", cyclus: " + std::to_string(licht.getCyclus()) + "s";
                    if (licht.getIsSlim()) {
                        title += " (slim)";
                    }

                    file << "            <div class=\"traffic-light " << kleurClass << "\" style=\"left: " << positie << "px;\" title=\"" << title << "\"></div>\n";
                }
            }
        }

        // Voeg bushaltes toe
        for (const auto& halte : situatie.getBushaltes()) {
            if (halte.getBaan() == baanNaam) {
                int positie = static_cast<int>(halte.getPositie() * schaalfactor);
                if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                    file << "            <div class=\"bus-stop\" style=\"left: " << positie << "px;\" title=\"Bushalte, wachttijd: " << halte.getWachttijd() << "s\"></div>\n";
                }
            }
        }

        // Voeg kruispunten toe
        for (const auto& kruispunt : situatie.getKruispunten()) {
            auto kruispuntBanen = kruispunt.getBanen();
            for (const auto& kruispuntBaan : kruispuntBanen) {
                if (kruispuntBaan.first == baanNaam) {
                    int positie = static_cast<int>(kruispuntBaan.second * schaalfactor);
                    if (positie >= 0 && positie < std::min(1000, baanLengte)) {
                        file << "            <div class=\"intersection\" style=\"left: " << positie << "px;\" title=\"Kruispunt\"></div>\n";
                    }
                }
            }
        }

        file << "        </div>\n"
             << "    </div>\n";
    }

    // Legende
    file << "    <div class=\"legend\">\n"
         << "        <h3>Legende</h3>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color auto\"></div> A = Auto</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color bus\"></div> B = Bus</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color brandweerwagen\"></div> F = Brandweerwagen</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color ziekenwagen\"></div> Z = Ziekenwagen</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color politiecombi\"></div> P = Politiecombi</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-red\"></div> Rood licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-orange\"></div> Oranje licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color traffic-light-green\"></div> Groen licht</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color bus-stop\" style=\"position: static;\"></div> Bushalte</div>\n"
         << "        <div class=\"legend-item\"><div class=\"legend-color intersection\" style=\"position: static;\"></div> Kruispunt</div>\n"
         << "    </div>\n";

    // Voertuig details
    file << "    <div class=\"details\">\n"
         << "        <h3>Voertuig Details</h3>\n";

    const auto& voertuigen = situatie.getVoertuigen();
    const auto& verkeerslichten = situatie.getVerkeerslichten();

    if (!voertuigen.empty()) {
        for (const auto& voertuig : voertuigen) {
            file << "        <div class=\"vehicle-detail\">\n"
                 << "            <strong>" << voertuig->getType() << "</strong> op " << voertuig->getBaanNaam()
                 << " @ " << std::fixed << std::setprecision(1) << voertuig->getPositie() << "m"
                 << " (snelheid: " << std::setprecision(1) << voertuig->getSnelheid() << " m/s)";

            // Zoek verkeerslichten op dezelfde baan
            for (const auto& licht : verkeerslichten) {
                if (licht.getBaan() == voertuig->getBaanNaam()) {
                    double afstand = licht.getPositie() - voertuig->getPositie();

                    // Alleen tonen als verkeerslicht voor het voertuig ligt (binnen 200m)
                    if (afstand > 0 && afstand <= 200.0) {
                        file << " → <em>" << licht.getKleurAsString() << " licht @ "
                             << std::fixed << std::setprecision(1) << licht.getPositie() << "m</em>";

                        // Extra info voor slimme lichten
                        if (licht.getIsSlim()) {
                            file << " <small>(slim";
                            if (licht.getTotaalVoertuigenVoorLicht() > 0) {
                                file << ", " << licht.getTotaalVoertuigenVoorLicht() << " wachtend";
                                if (licht.getPrioriteitsVoertuigenVoorLicht() > 0) {
                                    file << ", " << licht.getPrioriteitsVoertuigenVoorLicht() << " prioriteit";
                                }
                            }
                            file << ")</small>";
                        }
                        break; // Alleen eerste verkeerslicht tonen
                    }
                }
            }

            file << "\n        </div>\n";
        }
    } else {
        file << "        <p>Geen voertuigen in de simulatie.</p>\n";
    }

    file << "    </div>\n";

    // Schrijf statistieken
    file << "    <div class=\"statistics\">\n"
         << "        <h3>Statistieken</h3>\n"
         << "        <p><strong>Aantal banen:</strong> " << situatie.getBanen().size() << "</p>\n"
         << "        <p><strong>Aantal voertuigen:</strong> " << situatie.getVoertuigen().size() << "</p>\n"
         << "        <p><strong>Aantal verkeerslichten:</strong> " << situatie.getVerkeerslichten().size();

    // Tel slimme verkeerslichten
    int slimmeVerkeerslichten = 0;
    for (const auto& licht : verkeerslichten) {
        if (licht.getIsSlim()) {
            slimmeVerkeerslichten++;
        }
    }
    if (slimmeVerkeerslichten > 0) {
        file << " (waarvan " << slimmeVerkeerslichten << " slim)";
    }
    file << "</p>\n";

    file << "        <p><strong>Aantal bushaltes:</strong> " << situatie.getBushaltes().size() << "</p>\n"
         << "        <p><strong>Aantal kruispunten:</strong> " << situatie.getKruispunten().size() << "</p>\n";

    // Verkeerslicht status
    if (!verkeerslichten.empty()) {
        file << "        <h4>Verkeerslichten Status</h4>\n";
        for (const auto& licht : verkeerslichten) {
            try {
                file << "        <p><strong>" << licht.getBaan() << " @ " << licht.getPositie() << "m:</strong> "
                     << licht.getKleurAsString() << " licht";

                if (licht.getIsSlim()) {
                    file << " (slim, cyclus: " << licht.getCyclus() << "s)";

                    try {
                        int wachtenden = licht.getVoertuigenVoorLicht();
                        if (wachtenden > 0) {
                            file << ", " << wachtenden << " voertuigen wachtend";
                        }
                    } catch (...) {
                        // Skip wachtende voertuigen info als niet beschikbaar
                    }
                } else {
                    file << " (normaal, cyclus: " << licht.getCyclus() << "s)";
                }

                file << "</p>\n";
            } catch (...) {
                // Bij errors, toon minimale info
                file << "        <p>Verkeerslicht op " << licht.getBaan() << "</p>\n";
            }
        }
    }

    file << "    </div>\n";

    // HTML footer
    file << "</body>\n"
         << "</html>\n";

    file.close();
    return true;
}

/**
 * @brief Haal de laatste foutmelding op
 * @return De laatste foutmelding
 * @pre properlyInitialized() == true
 */
std::string output::getLastFoutmelding() const {
    REQUIRE(properlyInitialized(), "Output niet correct ingesteld");
    return lastFoutmelding;
}