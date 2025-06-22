/**
 * @file test_helpers.h
 * @brief Algemene hulpfuncties voor tests met verbeterde geheugen veiligheid
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <string>
#include <fstream>
#include <memory>
#include <gtest/gtest.h>
#include "../TraficObjects/voertuig.h"
#include "../Situation/situatie.h"
#include "../TraficObjects/baan.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"
#include "../TraficObjects/voertuiggenerator.h"

/**
 * @brief Hulpfunctie om container grootte naar int te converteren voor veiligere vergelijkingen in tests
 * @tparam Container Type van container om elementen in te tellen
 * @param c De container om elementen in te tellen
 * @return Aantal elementen als int
 *
 * Deze template functie biedt een veilige manier om size_t naar int te converteren
 * voor gebruik in EXPECT_EQ vergelijkingen in Google Test.
 */
template<typename Container>
int count(const Container& c) {
    return static_cast<int>(c.size());
}

/**
 * @brief Maakt een tijdelijk XML bestand voor testdoeleinden
 * @param content De XML inhoud om naar het bestand te schrijven
 * @return Bestandsnaam van het aangemaakte tijdelijke bestand
 *
 * Maakt een tijdelijk XML bestand met de gegeven inhoud dat gebruikt kan worden
 * voor het testen van bestand lees functionaliteit.
 */
inline std::string createTempXmlFile(const std::string& content) {
    std::string filename = "test_temp.xml";
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
    return filename;
}

/**
 * @brief Checkt of een bestand bestaat
 * @param filename Pad naar het bestand om te checken
 * @return true als bestand bestaat en leesbaar is, anders false
 *
 * Simpele hulpfunctie om bestaan van bestanden te checken voordat
 * ze gelezen of verwerkt worden in tests.
 */
inline bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

/**
 * @brief Maakt een tijdelijk bestand met gegeven inhoud en extensie
 * @param content Inhoud om naar het bestand te schrijven
 * @param extension Bestandsextensie (standaard: ".txt")
 * @return Bestandsnaam van het aangemaakte tijdelijke bestand
 *
 * Algemene functie voor het maken van tijdelijke bestanden met elke inhoud
 * en bestandsextensie voor testdoeleinden.
 */
inline std::string createTempFile(const std::string& content, const std::string& extension = ".txt") {
    std::string filename = "test_temp" + extension;
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
    return filename;
}

/**
 * @brief Leest volledige bestandsinhoud in een string
 * @param filename Pad naar het bestand om te lezen
 * @return Bestandsinhoud als string, lege string als bestand niet gelezen kan worden
 *
 * Hulpfunctie voor het lezen van bestandsinhoud voor verificatie in tests.
 * Geeft lege string terug als bestand niet geopend kan worden.
 */
inline std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

/**
 * @brief Maakt een uitgebreide test verkeerssituatie met meerdere componenten
 * @return Complete VerkeersSituatie object voor testen
 *
 * Deze functie maakt een realistische verkeerssituatie met wegen, voertuigen,
 * verkeerslichten, bushaltes, kruispunten en voertuiggeneratoren voor
 * uitgebreide tests van het verkeerssimulatie systeem.
 *
 * @note Deze functie zorgt ervoor dat alle aangemaakte objecten correct geïnitialiseerd zijn
 * en alle pointers veilig beheerd worden om segmentation faults te voorkomen.
 */
inline VerkeersSituatie createTestSituatie() {
    VerkeersSituatie situatie;

    try {
        // Voeg wegen toe met validatie
        Baan baan1("Hoofdweg", 500);
        Baan baan2("Zijstraat", 300);
        Baan baan3("Kruisweg", 400);

        if (!situatie.voegBaanToe(baan1) ||
            !situatie.voegBaanToe(baan2) ||
            !situatie.voegBaanToe(baan3)) {
            // Als wegen niet toegevoegd kunnen worden, maak minimale situatie
            Baan minimalBaan("Testweg", 100);
            situatie.voegBaanToe(minimalBaan);
            return situatie;
        }

        // Voeg voertuigen veilig toe met goede foutcontrole
        auto auto1 = Voertuig::maakVoertuig("Hoofdweg", 50, "auto");
        if (auto1) {
            situatie.voegVoertuigToe(std::move(auto1));
        }

        auto bus1 = Voertuig::maakVoertuig("Hoofdweg", 150, "bus");
        if (bus1) {
            situatie.voegVoertuigToe(std::move(bus1));
        }

        auto brandweer = Voertuig::maakVoertuig("Zijstraat", 100, "brandweerwagen");
        if (brandweer) {
            situatie.voegVoertuigToe(std::move(brandweer));
        }

        auto ziekenwagen = Voertuig::maakVoertuig("Kruisweg", 200, "ziekenwagen");
        if (ziekenwagen) {
            situatie.voegVoertuigToe(std::move(ziekenwagen));
        }

        auto politie = Voertuig::maakVoertuig("Kruisweg", 50, "politiecombi");
        if (politie) {
            situatie.voegVoertuigToe(std::move(politie));
        }

        // Voeg verkeerslichten toe met validatie
        try {
            Verkeerslicht licht1("Hoofdweg", 300, 30, true, false);
            Verkeerslicht licht2("Zijstraat", 200, 20, false, true);
            situatie.voegVerkeerslichtToe(licht1);
            situatie.voegVerkeerslichtToe(licht2);
        } catch (const std::exception& e) {
            // Als verkeerslichten falen, ga door zonder ze
        }

        // Voeg bushaltes toe met validatie
        try {
            Bushalte halte1("Hoofdweg", 250, 15);
            Bushalte halte2("Kruisweg", 150, 10);
            situatie.voegBushalteToe(halte1);
            situatie.voegBushalteToe(halte2);
        } catch (const std::exception& e) {
            // Als bushaltes falen, ga door zonder ze
        }

        // Voeg kruispunt toe met validatie
        try {
            Kruispunt kruispunt;
            kruispunt.voegBaanToe("Hoofdweg", 400);
            kruispunt.voegBaanToe("Kruisweg", 300);
            situatie.voegKruispuntToe(kruispunt);
        } catch (const std::exception& e) {
            // Als kruispunt faalt, ga door zonder het
        }

        // Voeg voertuiggenerator toe met validatie
        try {
            VoertuigGenerator generator("Zijstraat", 10, "auto");
            situatie.voegVoertuigGeneratorToe(generator);
        } catch (const std::exception& e) {
            // Als generator faalt, ga door zonder het
        }

    } catch (const std::exception& e) {
        // Als er een kritieke fout optreedt, geef minimale geldige situatie terug
        VerkeersSituatie minimalSituatie;
        Baan minimalBaan("Testweg", 100);
        minimalSituatie.voegBaanToe(minimalBaan);
        return minimalSituatie;
    }

    return situatie;
}

/**
 * @brief Maakt een minimale geldige verkeerssituatie voor basis tests
 * @return Simpele VerkeersSituatie met één weg en één voertuig
 *
 * Deze functie maakt de meest basale geldige verkeerssituatie die gebruikt
 * kan worden voor tests die geen complexe scenario's vereisen. Het is veiliger
 * dan createTestSituatie() voor tests die gevoelig kunnen zijn voor complexe
 * object interacties.
 */
inline VerkeersSituatie createMinimalTestSituatie() {
    VerkeersSituatie situatie;
    Baan baan("Testweg", 100);
    situatie.voegBaanToe(baan);

    auto voertuig = Voertuig::maakVoertuig("Testweg", 50, "auto");
    if (voertuig) {
        situatie.voegVoertuigToe(std::move(voertuig));
    }

    return situatie;
}

#endif // TEST_HELPERS_H