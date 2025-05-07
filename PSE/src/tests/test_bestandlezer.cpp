/**
 * @file test_bestandslezer.cpp
 * @brief Tests for the BestandsLezer class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../FileReader/bestandslezer.h"
#include "../Situation/situatie.h"

// Test for reading a valid XML file
TEST(BestandsLezerTest, ValidXml) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "    <VOERTUIG>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>50</positie>\n"
        "    </VOERTUIG>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>200</positie>\n"
        "        <cyclus>30</cyclus>\n"
        "    </VERKEERSLICHT>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    // Check the number of elements
    EXPECT_EQ(1, count(situatie.getBanen()));
    EXPECT_EQ(1, count(situatie.getVoertuigen()));
    EXPECT_EQ(1, count(situatie.getVerkeerslichten()));

    // Clean up
    std::remove(filename.c_str());
}

// Test for reading an XML with all extended elements
TEST(BestandsLezerTest, ExtendedXml) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "    <BAAN>\n"
        "        <naam>Zijstraat</naam>\n"
        "        <lengte>200</lengte>\n"
        "    </BAAN>\n"
        "    <VOERTUIG>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>50</positie>\n"
        "        <type>bus</type>\n"
        "    </VOERTUIG>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>200</positie>\n"
        "        <cyclus>30</cyclus>\n"
        "        <oranje>true</oranje>\n"
        "    </VERKEERSLICHT>\n"
        "    <BUSHALTE>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>150</positie>\n"
        "        <wachttijd>15</wachttijd>\n"
        "    </BUSHALTE>\n"
        "    <KRUISPUNT>\n"
        "        <baan positie=\"200\">Teststraat</baan>\n"
        "        <baan positie=\"100\">Zijstraat</baan>\n"
        "    </KRUISPUNT>\n"
        "    <VOERTUIGGENERATOR>\n"
        "        <baan>Zijstraat</baan>\n"
        "        <frequentie>15</frequentie>\n"
        "        <type>auto</type>\n"
        "    </VOERTUIGGENERATOR>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    // Check the number of elements
    EXPECT_EQ(2, count(situatie.getBanen()));
    EXPECT_EQ(1, count(situatie.getVoertuigen()));
    EXPECT_EQ(1, count(situatie.getVerkeerslichten()));
    EXPECT_EQ(1, count(situatie.getBushaltes()));
    EXPECT_EQ(1, count(situatie.getKruispunten()));
    EXPECT_EQ(1, count(situatie.getVoertuigGenerators()));

    // Check correct types
    EXPECT_EQ("bus", situatie.getVoertuigen()[0]->getType());
    EXPECT_TRUE(situatie.getVerkeerslichten()[0].getHeeftOranje());

    // Clean up
    std::remove(filename.c_str());
}

// Test for reading an invalid XML file
TEST(BestandsLezerTest, InvalidXml) {
    std::string xmlContent = "This is not valid XML";
    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test for reading an XML file without a root element
TEST(BestandsLezerTest, XmlNoRoot) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    // Should succeed because the parser adds a root element if missing
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    // Check the number of elements
    EXPECT_EQ(1, count(situatie.getBanen()));

    // Clean up
    std::remove(filename.c_str());
}

// Test for reading an XML with missing required fields
TEST(BestandsLezerTest, MissingRequiredFields) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <!-- Missing lengte field -->\n"
        "    </BAAN>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test for reading an XML with a missing road that vehicles require
TEST(BestandsLezerTest, MissingRoad) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <VOERTUIG>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>50</positie>\n"
        "    </VOERTUIG>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test om een XML in te lezen met ongeldige data
TEST(BestandsLezerTest, InvalidRoadData) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>-10</lengte>\n"  // Lengte mag niet negatief zijn
        "    </BAAN>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test om een XML in te lezen met ongeldige data
TEST(BestandsLezerTest, InvalidTrafficLightData) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>200</positie>\n"
        "        <cyclus>-30</cyclus>\n" // Cycle mag niet negatief zijn
        "    </VERKEERSLICHT>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test om XML in te lezen met een Unknown type
TEST(BestandsLezerTest, UnknownElementType) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <UNKNOWN_ELEMENT>\n"
        "        <naam>Test</naam>\n"
        "    </UNKNOWN_ELEMENT>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    // Moet nog altijd slagen want er is een geldige baan
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_EQ(1, count(situatie.getBanen()));

    // Clean up
    std::remove(filename.c_str());
}

// Test om een XML in te lezen met een niet compleet verkeerslicht
TEST(BestandsLezerTest, IncompleteTrafficLight) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Teststraat</baan>\n"
        "        <!-- Missing position and cycle time -->\n"
        "    </VERKEERSLICHT>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());

    // Clean up
    std::remove(filename.c_str());
}

// Test om een verkeerslicht in te lezen op een niet bestaande weg
TEST(BestandsLezerTest, TrafficLightOnNonExistentRoad) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>NonExistent</baan>\n"
        "        <positie>100</positie>\n"
        "        <cyclus>30</cyclus>\n"
        "    </VERKEERSLICHT>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));
    EXPECT_EQ(1, count(situatie.getBanen()));
    EXPECT_EQ(0, count(situatie.getVerkeerslichten())); // verkeerslicht niet toegevoegd

    // Clean up
    std::remove(filename.c_str());
}

// Test leest een niet bestaande file
TEST(BestandsLezerTest, NonExistentFile) {
    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand("non_existent_file.xml", situatie));
    EXPECT_FALSE(lezer.getLastFoutmelding().empty());
}

// Test leest een complexe xml-file in
TEST(BestandsLezerTest, ComplexExample) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Middelheimlaan</naam>\n"
        "        <lengte>500</lengte>\n"
        "    </BAAN>\n"
        "    <BAAN>\n"
        "        <naam>Groenenborgerlaan</naam>\n"
        "        <lengte>800</lengte>\n"
        "    </BAAN>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Middelheimlaan</baan>\n"
        "        <positie>400</positie>\n"
        "        <cyclus>20</cyclus>\n"
        "    </VERKEERSLICHT>\n"
        "    <VERKEERSLICHT>\n"
        "        <baan>Groenenborgerlaan</baan>\n"
        "        <positie>350</positie>\n"
        "        <cyclus>30</cyclus>\n"
        "    </VERKEERSLICHT>\n"
        "    <VOERTUIG>\n"
        "        <baan>Middelheimlaan</baan>\n"
        "        <positie>20</positie>\n"
        "    </VOERTUIG>\n"
        "    <VOERTUIG>\n"
        "        <baan>Middelheimlaan</baan>\n"
        "        <positie>0</positie>\n"
        "    </VOERTUIG>\n"
        "    <VOERTUIG>\n"
        "        <baan>Groenenborgerlaan</baan>\n"
        "        <positie>150</positie>\n"
        "    </VOERTUIG>\n"
        "    <VOERTUIG>\n"
        "        <baan>Groenenborgerlaan</baan>\n"
        "        <positie>500</positie>\n"
        "    </VOERTUIG>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    //checken van de hoeveelheid elementen
    EXPECT_EQ(2, count(situatie.getBanen()));
    EXPECT_EQ(4, count(situatie.getVoertuigen()));
    EXPECT_EQ(2, count(situatie.getVerkeerslichten()));

    // Clean up
    std::remove(filename.c_str());
}