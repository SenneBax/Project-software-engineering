/**
 * @file test_helpers.h
 * @brief Common helper functions for tests
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <string>
#include <fstream>
#include <gtest/gtest.h>
#include "../TraficObjects/voertuig.h"
#include "../Situation/situatie.h"

// Helper function to convert size_t to int for safer comparisons in tests
template<typename Container>
int count(const Container& c) {
    return static_cast<int>(c.size());
}

// Helper function to create a temporary XML file for testing
inline std::string createTempXmlFile(const std::string& content) {
    std::string filename = "test_temp.xml";
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Helper function to check if a file exists
inline bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Helper function to create a temporary file and return its name
inline std::string createTempFile(const std::string& content, const std::string& extension = ".txt") {
    std::string filename = "test_temp" + extension;
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Helper function to read a file into a string
inline std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

// Helper function to create a test VerkeersSituatie with multiple components
inline VerkeersSituatie createTestSituatie() {
    VerkeersSituatie situatie;

    // Add some roads
    Baan baan1("Hoofdweg", 500);
    Baan baan2("Zijstraat", 300);
    Baan baan3("Kruisweg", 400);
    situatie.voegBaanToe(baan1);
    situatie.voegBaanToe(baan2);
    situatie.voegBaanToe(baan3);

    // Add some vehicles
    auto auto1 = Voertuig::maakVoertuig("Hoofdweg", 50, "auto");
    auto bus1 = Voertuig::maakVoertuig("Hoofdweg", 150, "bus");
    auto brandweer = Voertuig::maakVoertuig("Zijstraat", 100, "brandweerwagen");
    auto ziekenwagen = Voertuig::maakVoertuig("Kruisweg", 200, "ziekenwagen");
    auto politie = Voertuig::maakVoertuig("Kruisweg", 50, "politiecombi");

    situatie.voegVoertuigToe(std::move(auto1));
    situatie.voegVoertuigToe(std::move(bus1));
    situatie.voegVoertuigToe(std::move(brandweer));
    situatie.voegVoertuigToe(std::move(ziekenwagen));
    situatie.voegVoertuigToe(std::move(politie));

    // Add some traffic lights
    Verkeerslicht licht1("Hoofdweg", 300, 30, true);  // With orange light
    Verkeerslicht licht2("Zijstraat", 200, 20, false, true);  // Smart traffic light
    situatie.voegVerkeerslichtToe(licht1);
    situatie.voegVerkeerslichtToe(licht2);

    // Add some bus stops
    Bushalte halte1("Hoofdweg", 250, 15);
    Bushalte halte2("Kruisweg", 150, 10);
    situatie.voegBushalteToe(halte1);
    situatie.voegBushalteToe(halte2);

    // Add an intersection
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 400);
    kruispunt.voegBaanToe("Kruisweg", 300);
    situatie.voegKruispuntToe(kruispunt);

    // Add a vehicle generator
    VoertuigGenerator generator("Zijstraat", 10, "auto");
    situatie.voegVoertuigGeneratorToe(generator);

    return situatie;
}

#endif // TEST_HELPERS_H