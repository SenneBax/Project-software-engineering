//
// Created by senne on 6/03/2025.
//

//
// Created by senne on 6/03/2025.
//

#include <gtest/gtest.h>
#include <fstream>
#include "Situatie.h"
#include "Baan.h"
#include "Voertuig.h"
#include "Verkeerslicht.h"
#include "simulatie.h"

// hulp functie om tijdelijke xml-bestanden te testen
std::string createTempXmlFile(const std::string& content) {
    std::string filename = "test_temp.xml";
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Tests voor de Baan class
TEST(BaanTest, ConstructorAndGetters) {
    Baan baan("Teststraat", 250);
    EXPECT_EQ("Teststraat", baan.getNaam());
    EXPECT_EQ(250, baan.getLengte());
}

TEST(BaanTest, DefaultConstructor) {
    Baan baan;
    EXPECT_EQ("", baan.getNaam());
    EXPECT_EQ(0, baan.getLengte());
}

// Tests voor de Voertuig class
TEST(VoertuigTest, ConstructorAndGetters) {
    Voertuig voertuig("Teststraat", 75);
    EXPECT_EQ("Teststraat", voertuig.getBaan());
    EXPECT_EQ(75, voertuig.getPositie());
}

// Tests voor de Verkeerslicht class
TEST(VerkeerslichtTest, ConstructorAndGetters) {
    Verkeerslicht verkeerslicht("Teststraat", 150, 30);
    EXPECT_EQ("Teststraat", verkeerslicht.getBaan());
    EXPECT_EQ(150, verkeerslicht.getPositie());
    EXPECT_EQ(30, verkeerslicht.getCyclus());
}

// Tests voor de VerkeersSituatie class
TEST(VerkeersSituatieTest, VoegBaanToe) {
    VerkeersSituatie situatie;
    Baan baan1("Teststraat", 250);
    Baan baan2("Hoofdweg", 500);
    Baan baan3("Bermuda", 8);

    // eerste toevoeging van de baan hoort correct te zijn (True)
    EXPECT_TRUE(situatie.voegBaanToe(baan1));


    // 2de met een andere naam hoort correct te zijn (True)
    EXPECT_TRUE(situatie.voegBaanToe(baan2));

    //3de baan moet ook bestaan (True
    EXPECT_TRUE(situatie.voegBaanToe(baan3));

    // een duplicaat moet falen (False)
    EXPECT_FALSE(situatie.voegBaanToe(baan1));
}

TEST(VerkeersSituatieTest, VoegVoertuigToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // toevoeging correcte baan
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // correct voertuig op een bepaalde weg moet (True) zijn
    Voertuig voertuig1("Teststraat", 0);
    EXPECT_TRUE(situatie.voegVoertuigToe(voertuig1));

    // correct voertuig op het einde van de weg (True)
    Voertuig voertuig2("Teststraat", 250);
    EXPECT_TRUE(situatie.voegVoertuigToe(voertuig2));

    // voertuig op een weg die niet bestaat moet (Fail) zijn
    Voertuig voertuig3("NietBestaandeBaan", 0);
    EXPECT_FALSE(situatie.voegVoertuigToe(voertuig3));

    // voertuig op een "invalid" positie moet falen
    Voertuig voertuig4("Teststraat", 251);
    EXPECT_FALSE(situatie.voegVoertuigToe(voertuig4));

    // voertuig op een negatieve positie moet falen
    Voertuig voertuig5("Teststraat", -1);
    EXPECT_FALSE(situatie.voegVoertuigToe(voertuig5));
}

TEST(VerkeersSituatieTest, VoegVerkeerslichtToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // maak eerst een correcte baan
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // correcte plaats voor een verkeerslicht moet slagen
    Verkeerslicht verkeerslicht1("Teststraat", 100, 20);
    EXPECT_TRUE(situatie.voegVerkeerslichtToe(verkeerslicht1));

    // verkeerslicht op een niet bestaande weg moet falen
    Verkeerslicht verkeerslicht2("NietBestaandeBaan", 100, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht2));

    // verkeerslicht op een niet bestaande plaats moet falen
    Verkeerslicht verkeerslicht3("Teststraat", 251, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht3));

    // verkeerslicht met een negatieve positie moet falen
    Verkeerslicht verkeerslicht4("Teststraat", -1, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht4));

    // verkeerslicht met een (niet bestaande) cyclus die 0 is moet falen
    Verkeerslicht verkeerslicht5("Teststraat", 100, 0);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht5));

    // verkeerslicht met een negatieve (niet-bestaande) cyclus moet falen
    Verkeerslicht verkeerslicht6("Teststraat", 100, -1);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht6));
}

TEST(VerkeersSituatieTest, VerificeerConsistentie) {
    VerkeersSituatie situatie;

    // situatie met niets erin moet falen aangezien deze niet consistent is
    EXPECT_FALSE(situatie.verificeerConsistentie());

    // situatie met een baan moet slagen want deze is wel consistent
    Baan baan("Teststraat", 250);
    EXPECT_TRUE(situatie.voegBaanToe(baan));
    EXPECT_TRUE(situatie.verificeerConsistentie());
}

// Tests voor de XML file te parsen
TEST(LeesVerkeersSituatieTest, ValidXml) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
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
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_TRUE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidXml) {
    std::string xmlContent = "This is not valid XML";
    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_FALSE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, MissingRoad) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
        "    <VOERTUIG>\n"
        "        <baan>Teststraat</baan>\n"
        "        <positie>50</positie>\n"
        "    </VOERTUIG>\n"
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_FALSE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidRoadData) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>-10</lengte>\n"
        "    </BAAN>\n"
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_FALSE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, MissingRequiredFields) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <!-- Missing lengte field -->\n"
        "    </BAAN>\n"
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_FALSE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidElementType) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
        "    <ONBEKEND_ELEMENT>\n"
        "        <naam>Test</naam>\n"
        "    </ONBEKEND_ELEMENT>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    // moet nog altijd True zijn aangezien er een valide baan is
    EXPECT_TRUE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, VoorbeeldXML) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Verkeerssituatie>\n"
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
        "</Verkeerssituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    EXPECT_TRUE(leesVerkeersSituatie(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

// Test voor het aanmaken van een simulatie-object
TEST(SimulatieTest, ConstructorTest) {
    VerkeersSituatie situatie;
    simulatie sim(situatie, 1.0);
    EXPECT_EQ(sim.getTijdstap(), 1.0);
    EXPECT_EQ(sim.getHuidigeSimulatieTijd(), 0.0);
}


//vanaf hier zijn de testen voor de simulatie


// Test voor het updaten van de simulatie
TEST(SimulatieTest, StapTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    Voertuig voertuig("Teststraat", 0);
    situatie.voegVoertuigToe(voertuig);

    simulatie sim(situatie, 1.0);
    sim.stap();

    EXPECT_GT(sim.getHuidigeSimulatieTijd(), 0.0);
}

// Test voor automatisch genereren van voertuigen
TEST(SimulatieTest, AutoGenereerVoertuigenTest) {
    VerkeersSituatie situatie;
    simulatie sim(situatie, 1.0);
    sim.setAutoGenereerVoertuigen(true);

    EXPECT_TRUE(sim.getAantalVoertuigen() >= 0);
}

// Test voor het verzamelen van statistieken
TEST(SimulatieTest, StatistiekenTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    Voertuig voertuig1("Teststraat", 0);
    Voertuig voertuig2("Teststraat", 100);
    situatie.voegVoertuigToe(voertuig1);
    situatie.voegVoertuigToe(voertuig2);

    simulatie sim(situatie, 1.0);
    sim.stap();

    EXPECT_GE(sim.getGemiddeldeSnelheid(), 0.0);
    EXPECT_GE(sim.getTotaalVerwijderdeVoertuigen(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
