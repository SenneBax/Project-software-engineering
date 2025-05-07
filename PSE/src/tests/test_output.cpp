/**
 * @file test_output.cpp
 * @brief Tests for the output class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../Output/output.h"
#include "../Situation/situatie.h"
#include "../FileReader/bestandslezer.h"

// Tests for the TextRapport (Simple Output) functionality
TEST(OutputTest, TextRapportTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Generate a text report
    std::string report = uitvoer.genereerTekstRapport(situatie);

    // Check if the report contains basic information
    EXPECT_TRUE(report.find("Banen (3):") != std::string::npos);
    EXPECT_TRUE(report.find("Voertuigen (5):") != std::string::npos);
    EXPECT_TRUE(report.find("Verkeerslichten (2):") != std::string::npos);
    EXPECT_TRUE(report.find("Bushaltes (2):") != std::string::npos);
    EXPECT_TRUE(report.find("Kruispunten (1):") != std::string::npos);

    // Check for specific information
    EXPECT_TRUE(report.find("Hoofdweg") != std::string::npos);
    EXPECT_TRUE(report.find("bus") != std::string::npos);
    EXPECT_TRUE(report.find("brandweerwagen") != std::string::npos);
    EXPECT_TRUE(report.find("ziekenwagen") != std::string::npos);
    EXPECT_TRUE(report.find("politiecombi") != std::string::npos);
}

// Tests for the GraphicalImpression functionality
TEST(OutputTest, GraphicalImpressionTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Generate a graphical impression
    std::string impression = uitvoer.genereerGrafischeImpressie(situatie);

    // Check if the impression contains basic road representations
    EXPECT_TRUE(impression.find("Hoofdweg |") != std::string::npos);
    EXPECT_TRUE(impression.find("Zijstraat |") != std::string::npos);
    EXPECT_TRUE(impression.find("Kruisweg |") != std::string::npos);

    // Check for vehicle markings (A for auto, B for bus, etc.)
    EXPECT_TRUE(impression.find("A") != std::string::npos); // Auto
    EXPECT_TRUE(impression.find("B") != std::string::npos); // Bus
    EXPECT_TRUE(impression.find("F") != std::string::npos); // Brandweerwagen
    EXPECT_TRUE(impression.find("Z") != std::string::npos); // Ziekenwagen
    EXPECT_TRUE(impression.find("P") != std::string::npos); // Politiecombi

    // Check for traffic light, bus stop, and intersection displays
    EXPECT_TRUE(impression.find("> verkeerslichten |") != std::string::npos);
    EXPECT_TRUE(impression.find("> bushaltes") != std::string::npos);
    EXPECT_TRUE(impression.find("> kruispunten") != std::string::npos);

    // Check for legend
    EXPECT_TRUE(impression.find("Legende:") != std::string::npos);
}

// Tests for XML output functionality
TEST(OutputTest, XmlOutputTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Create a temporary XML file
    std::string xmlFilename = "test_output.xml";

    // Write the traffic situation to XML
    EXPECT_TRUE(uitvoer.schrijfNaarXml(situatie, xmlFilename));
    EXPECT_TRUE(fileExists(xmlFilename));

    // Read the XML file
    std::string xmlContent = readFile(xmlFilename);

    // Check basic XML structure
    EXPECT_TRUE(xmlContent.find("<?xml") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VerkeersSituatie>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("</VerkeersSituatie>") != std::string::npos);

    // Check for elements
    EXPECT_TRUE(xmlContent.find("<BAAN>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VOERTUIG>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VERKEERSLICHT>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<BUSHALTE>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<KRUISPUNT>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VOERTUIGGENERATOR>") != std::string::npos);

    // Test if we can read the XML back into a new situation
    VerkeersSituatie newSituation;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(xmlFilename, newSituation));

    // Check if the new situation has the same elements
    EXPECT_EQ(situatie.getBanen().size(), newSituation.getBanen().size());
    EXPECT_EQ(situatie.getVoertuigen().size(), newSituation.getVoertuigen().size());
    EXPECT_EQ(situatie.getVerkeerslichten().size(), newSituation.getVerkeerslichten().size());
    EXPECT_EQ(situatie.getBushaltes().size(), newSituation.getBushaltes().size());
    EXPECT_EQ(situatie.getKruispunten().size(), newSituation.getKruispunten().size());
    EXPECT_EQ(situatie.getVoertuigGenerators().size(), newSituation.getVoertuigGenerators().size());

    // Clean up
    std::remove(xmlFilename.c_str());
}

// Tests for HTML output functionality
TEST(OutputTest, HtmlOutputTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Create a temporary HTML file
    std::string htmlFilename = "test_output.html";

    // Write the traffic situation to HTML
    EXPECT_TRUE(uitvoer.schrijfNaarHtml(situatie, htmlFilename));
    EXPECT_TRUE(fileExists(htmlFilename));

    // Read the HTML file
    std::string htmlContent = readFile(htmlFilename);

    // Check basic HTML structure
    EXPECT_TRUE(htmlContent.find("<!DOCTYPE html>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<html>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<head>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<body>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("</html>") != std::string::npos);

    // Check for CSS styles
    EXPECT_TRUE(htmlContent.find("<style>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("body {") != std::string::npos);
    EXPECT_TRUE(htmlContent.find(".road {") != std::string::npos);
    EXPECT_TRUE(htmlContent.find(".vehicle {") != std::string::npos);

    // Check for road displays
    EXPECT_TRUE(htmlContent.find("Hoofdweg") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("Zijstraat") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("Kruisweg") != std::string::npos);

    // Check for vehicle displays
    EXPECT_TRUE(htmlContent.find("class=\"vehicle auto\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle bus\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle brandweerwagen\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle ziekenwagen\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle politiecombi\"") != std::string::npos);

    // Check for traffic light, bus stop, and intersection displays
    EXPECT_TRUE(htmlContent.find("class=\"traffic-light") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"bus-stop\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"intersection\"") != std::string::npos);

    // Check for legend and statistics
    EXPECT_TRUE(htmlContent.find("class=\"legend\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"statistics\"") != std::string::npos);

    // Clean up
    std::remove(htmlFilename.c_str());
}

// Tests for error handling in output
TEST(OutputTest, ErrorHandlingXmlOutput) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Try to write to an invalid location (directory that doesn't exist)
    std::string invalidPath = "/invalid/path/test.xml";
    EXPECT_FALSE(uitvoer.schrijfNaarXml(situatie, invalidPath));

    // Check the exact error message
    std::string expectedError = "Kan bestand '" + invalidPath + "' niet schrijven";
    EXPECT_EQ(expectedError, uitvoer.getLastFoutmelding());
}

// Test error handling for HTML output
TEST(OutputTest, ErrorHandlingHtmlOutput) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Try to write to an invalid location (directory that doesn't exist)
    std::string invalidPath = "/invalid/path/test.html";
    EXPECT_FALSE(uitvoer.schrijfNaarHtml(situatie, invalidPath));

    // Check the exact error message
    std::string expectedError = "Kan bestand '" + invalidPath + "' niet openen";
    EXPECT_EQ(expectedError, uitvoer.getLastFoutmelding());
}

// Test empty filename
TEST(OutputTest, EmptyFilename) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Try to write to an empty filename
    EXPECT_FALSE(uitvoer.schrijfNaarXml(situatie, ""));

    // Check the exact error message - this depends on how your code handles empty filenames
    // Adjust the expected message based on your implementation
    EXPECT_EQ("BestandsNaam mag niet leeg zijn.", uitvoer.getLastFoutmelding());

    // Test for HTML output
    EXPECT_FALSE(uitvoer.schrijfNaarHtml(situatie, ""));
    EXPECT_EQ("BestandNaam mag niet leeg zijn.", uitvoer.getLastFoutmelding());
}

// Test proper initialization
TEST(OutputTest, ProperlyInitialized) {
    output uitvoer;
    EXPECT_TRUE(uitvoer.properlyInitialized());
}

// Test using an invalid situation
TEST(OutputTest, InvalidSituation) {
    VerkeersSituatie situatie; // Empty situation, not initialized properly
    output uitvoer;

    // These should fail if your code checks for properlyInitialized() in the situatie parameter
    // If your code doesn't check this, these tests might need to be adjusted
    EXPECT_ANY_THROW(uitvoer.genereerTekstRapport(situatie));
    EXPECT_ANY_THROW(uitvoer.genereerGrafischeImpressie(situatie));
    EXPECT_ANY_THROW(uitvoer.schrijfNaarXml(situatie, "test.xml"));
    EXPECT_ANY_THROW(uitvoer.schrijfNaarHtml(situatie, "test.html"));
}