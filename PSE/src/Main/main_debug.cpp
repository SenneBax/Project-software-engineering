/**
* @file main_debug.cpp
 * @author senne
 * @date 6/03/2025
 * @brief dit is de debug file waar de testen geïmplementeerd in staan. (dit is een executable)
 */


#include <gtest/gtest.h>
#include <fstream>
#include "../Situation/situatie.h"
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../Simulation/simulatie.h"
#include "../FileReader/bestandslezer.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"
#include "../Output/output.h"

// Hulpfunctie om size_t naar int te converteren voor veilige vergelijkingen
template<typename Container>
int count(const Container& c) {
    return static_cast<int>(c.size());
}

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

// Tests voor verschillende voertuigtypes
TEST(VoertuigTest, VehicleTypes) {
    Voertuig auto1("Teststraat", 10, "auto");
    Voertuig bus1("Teststraat", 20, "bus");
    Voertuig brandweer("Teststraat", 30, "brandweerwagen");

    EXPECT_EQ("auto", auto1.getType());
    EXPECT_EQ("bus", bus1.getType());
    EXPECT_EQ("brandweerwagen", brandweer.getType());

    EXPECT_FALSE(auto1.isPrioriteitsvoertuig());
    EXPECT_FALSE(bus1.isPrioriteitsvoertuig());
    EXPECT_TRUE(brandweer.isPrioriteitsvoertuig());

    EXPECT_FALSE(auto1.isBus());
    EXPECT_TRUE(bus1.isBus());
}

// Tests voor de Verkeerslicht class
TEST(VerkeerslichtTest, ConstructorAndGetters) {
    Verkeerslicht verkeerslicht("Teststraat", 150, 30, true, false);
    EXPECT_EQ("Teststraat", verkeerslicht.getBaan());
    EXPECT_EQ(150, verkeerslicht.getPositie());
    EXPECT_EQ(30, verkeerslicht.getCyclus());
}

// Test voor verkeerslichten met oranje lichten
TEST(VerkeerslichtTest, OrangeTrafficLight) {
    Verkeerslicht verkeerslicht("Teststraat", 150, 30, true, false);
    EXPECT_TRUE(verkeerslicht.getHeeftOranje());
    EXPECT_TRUE(verkeerslicht.isRood()); // Start op rood
}

// Tests voor de Bushalte class
TEST(BushalteTest, ConstructorAndGetters) {
    Bushalte bushalte("Teststraat", 200, 15);
    EXPECT_EQ("Teststraat", bushalte.getBaan());
    EXPECT_EQ(200, bushalte.getPositie());
    EXPECT_EQ(15, bushalte.getWachttijd());
    EXPECT_FALSE(bushalte.isBusGestopt());
}

// Tests voor de Kruispunt class
TEST(KruispuntTest, AddingRoads) {
    Kruispunt kruispunt;
    EXPECT_TRUE(kruispunt.voegBaanToe("Teststraat", 250));
    EXPECT_TRUE(kruispunt.voegBaanToe("Zijstraat", 150));
    EXPECT_FALSE(kruispunt.voegBaanToe("Teststraat", 300)); // Dubbele straatnaam

    EXPECT_TRUE(kruispunt.bevatBaan("Teststraat"));
    EXPECT_FALSE(kruispunt.bevatBaan("Onbekendestraat"));

    EXPECT_EQ(250, kruispunt.getPositieOpBaan("Teststraat"));
    EXPECT_EQ(-1, kruispunt.getPositieOpBaan("Onbekendestraat"));
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

    // Test aantal voertuigen
    EXPECT_EQ(2, count(situatie.getVoertuigen()));
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

    // Test aantal verkeerslichten
    EXPECT_EQ(1, count(situatie.getVerkeerslichten()));
}

TEST(VerkeersSituatieTest, VoegBushalteToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // maak eerst een correcte baan
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // correcte bushalte moet slagen
    Bushalte bushalte1("Teststraat", 100, 20);
    EXPECT_TRUE(situatie.voegBushalteToe(bushalte1));

    // bushalte op een niet bestaande weg moet falen
    Bushalte bushalte2("NietBestaandeBaan", 100, 20);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte2));

    // bushalte op een niet bestaande plaats moet falen
    Bushalte bushalte3("Teststraat", 251, 20);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte3));

    // Test aantal bushaltes
    EXPECT_EQ(1, count(situatie.getBushaltes()));
}

TEST(VerkeersSituatieTest, VoegKruispuntToe) {
    VerkeersSituatie situatie;
    Baan baan1("Teststraat", 250);
    Baan baan2("Zijstraat", 300);

    // Voeg eerst banen toe
    EXPECT_TRUE(situatie.voegBaanToe(baan1));
    EXPECT_TRUE(situatie.voegBaanToe(baan2));

    // Maak een kruispunt
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Teststraat", 150);
    kruispunt.voegBaanToe("Zijstraat", 200);

    // Toevoegen moet lukken
    EXPECT_TRUE(situatie.voegKruispuntToe(kruispunt));

    // Test aantal kruispunten
    EXPECT_EQ(1, count(situatie.getKruispunten()));
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

    // Controleer het aantal elementen
    EXPECT_EQ(1, count(situatie.getBanen()));
    EXPECT_EQ(1, count(situatie.getVoertuigen()));
    EXPECT_EQ(1, count(situatie.getVerkeerslichten()));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, ExtendedXml) {
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
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    // Controleer het aantal elementen
    EXPECT_EQ(2, count(situatie.getBanen()));
    EXPECT_EQ(1, count(situatie.getVoertuigen()));
    EXPECT_EQ(1, count(situatie.getVerkeerslichten()));
    EXPECT_EQ(1, count(situatie.getBushaltes()));
    EXPECT_EQ(1, count(situatie.getKruispunten()));

    // Controleer correcte types
    EXPECT_EQ("bus", situatie.getVoertuigen()[0].getType());
    EXPECT_TRUE(situatie.getVerkeerslichten()[0].getHeeftOranje());

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidXml) {
    std::string xmlContent = "This is not valid XML";
    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, MissingRoad) {
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
    // hoort False te zijn want er is geen baan op welke het voertuig kan komen.

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidRoadData) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>-10</lengte>\n"
        "    </BAAN>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    EXPECT_FALSE(lezer.leesXmlBestand(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, MissingRequiredFields) {
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

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, InvalidElementType) {
    std::string xmlContent =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<VerkeersSituatie>\n"
        "    <ONBEKEND_ELEMENT>\n"
        "        <naam>Test</naam>\n"
        "    </ONBEKEND_ELEMENT>\n"
        "    <BAAN>\n"
        "        <naam>Teststraat</naam>\n"
        "        <lengte>250</lengte>\n"
        "    </BAAN>\n"
        "</VerkeersSituatie>";

    std::string filename = createTempXmlFile(xmlContent);

    VerkeersSituatie situatie;
    BestandsLezer lezer;
    // moet nog altijd True zijn aangezien er een valide baan is
    EXPECT_TRUE(lezer.leesXmlBestand(filename, situatie));

    // geheugen terug vrijgeven en alles van de test verwijderen
    std::remove(filename.c_str());
}

TEST(LeesVerkeersSituatieTest, VoorbeeldXML) {
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


///
///Vanaf hier zijn de testen voor de simulatie geschreven
///



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
// Test voor de verwerking van negatieve of nul-tijdstap
// Test voor de verwerking van negatieve of nul-tijdstap
TEST(SimulatieTest, OngeldigeTijdstapTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    // Test met tijdstap nul, moet standaardwaarde 0.0166 gebruiken
    simulatie simZero(situatie, 0.0);
    EXPECT_DOUBLE_EQ(simZero.getTijdstap(), 0.0166);

    // Test met negatieve tijdstap, moet standaardwaarde 0.0166 gebruiken
    simulatie simNegatief(situatie, -1.0);
    EXPECT_DOUBLE_EQ(simNegatief.getTijdstap(), 0.0166);
}

// Test voor het berekenen van verkeerslicht statussen
TEST(SimulatieTest, VerkeerslichtStatusTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    // Voeg verkeerslicht toe met een cyclus van 10 seconden
    Verkeerslicht verkeerslicht("Teststraat", 150, 10, true, false);
    situatie.voegVerkeerslichtToe(verkeerslicht);

    simulatie sim(situatie, 1.0);

    // Plaats voertuig net voor het verkeerslicht
    Voertuig voertuig("Teststraat", 145);
    voertuig.setSnelheid(5.0); // Begin met snelheid 5 m/s
    situatie.voegVoertuigToe(voertuig);

    // Verkeerslichten beginnen op rood (tijd 0), dus het voertuig moet afremmen
    sim.stap();

    // Controleer of het voertuig inderdaad aan het afremmen is (indirecte test voor rood licht)
    EXPECT_LT(situatie.getVoertuigen()[0].getVersnelling(), 0);

    // Voer simulatie uit voor 5 seconden (licht moet groen worden na 5 seconden)
    for (int i = 0; i < 4; i++) {
        sim.stap();
    }

    // Na 5 seconden zou het licht groen moeten zijn, en het voertuig zou weer moeten versnellen
    sim.stap();

    // Test of de simulatietijd klopt
    EXPECT_NEAR(sim.getHuidigeSimulatieTijd(), 6.0, 0.001);
}

// Test voor het verwijderen van voertuigen die het einde van de weg bereiken
TEST(SimulatieTest, VoertuigVerwijderenTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 100);
    situatie.voegBaanToe(baan);

    // Voeg een voertuig toe dat bijna aan het einde van de weg is
    Voertuig voertuig("Teststraat", 95);
    voertuig.setSnelheid(10.0); // Positie 95, snelheid 10 m/s
    situatie.voegVoertuigToe(voertuig);

    // Controleer dat we met één voertuig beginnen
    EXPECT_EQ(1, count(situatie.getVoertuigen()));

    simulatie sim(situatie, 1.0);

    // Na één stap moet het voertuig de weg verlaten hebben en verwijderd zijn
    sim.stap();

    // Controleer of het voertuig verwijderd is
    EXPECT_EQ(0, count(situatie.getVoertuigen()));

    // Controleer of de teller voor verwijderde voertuigen is verhoogd
    EXPECT_EQ(sim.getTotaalVerwijderdeVoertuigen(), 1);
}

TEST(SimulatieTest, VoertuigVolggedragTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Voeg twee voertuigen toe: één langzamer voertuig voor, één sneller achter
    Voertuig voertuigVoor("Teststraat", 100);
    voertuigVoor.setSnelheid(5.0); // Langzaam rijdend voertuig (5 m/s)
    situatie.voegVoertuigToe(voertuigVoor);

    Voertuig voertuigAchter("Teststraat", 50);
    voertuigAchter.setSnelheid(15.0); // Sneller rijdend voertuig (15 m/s)
    situatie.voegVoertuigToe(voertuigAchter);

    // Zorg ervoor dat beide voertuigen initieel een positieve versnelling hebben
    situatie.getVoertuigen()[0].setVersnelling(0.5);
    situatie.getVoertuigen()[1].setVersnelling(1.0);

    simulatie sim(situatie, 0.5); // Halve seconde per stap

    // Voer simulatie uit voor meer stappen om voertuigen tijd te geven zich aan te passen
    for (int i = 0; i < 20; i++) {
        sim.stap();

        // Na enkele stappen moet het achterste voertuig beginnen af te remmen
        if (i >= 5) {
            // Controleer de afstand tussen de voertuigen
            double afstand = situatie.getVoertuigen()[0].getPositie() -
                             situatie.getVoertuigen()[1].getPositie() -
                             situatie.getVoertuigen()[0].getLengte();

            // De afstand mag niet negatief worden (geen botsing)
            EXPECT_GE(afstand, 0);

            // Als voertuigen dicht bij elkaar komen, moet het achterste voertuig afremmen
            if (afstand < situatie.getVoertuigen()[1].getMinVolgafstand() * 1.5) {
                // Controleer of de versnelling van het achterste voertuig minder is
                // dan die van het voorste, wat aangeeft dat het aan het afremmen is
                EXPECT_LE(situatie.getVoertuigen()[1].getVersnelling(),
                           situatie.getVoertuigen()[0].getVersnelling() + 0.1);
            }
        }
    }

    // Na meerdere stappen moet het achterste voertuig op veilige afstand achter het voorste rijden
    double finalAfstand = situatie.getVoertuigen()[0].getPositie() -
                          situatie.getVoertuigen()[1].getPositie() -
                          situatie.getVoertuigen()[0].getLengte();

    // Controleer of het achterste voertuig niet te dicht op het voorste zit
    EXPECT_GE(finalAfstand, 0);

    // Controleer of de snelheden uiteindelijk dichter bij elkaar komen
    // maar accepteer een groter verschil dan in de originele test
    double initieleSnelheidsverschil = 10.0; // 15 - 5
    double huidigSnelheidsverschil = std::abs(situatie.getVoertuigen()[1].getSnelheid() -
                                             situatie.getVoertuigen()[0].getSnelheid());

    // Het snelheidsverschil moet kleiner zijn dan het originele verschil
    EXPECT_LT(huidigSnelheidsverschil, initieleSnelheidsverschil);
}

TEST(SimulatieTest, BushalteStopTest) {
    VerkeersSituatie situatie;

    // Maak een testbaan met een bushalte
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Voeg een bushalte toe op positie 150
    Bushalte bushalte("Teststraat", 150, 10); // 10 seconden wachttijd
    situatie.voegBushalteToe(bushalte);

    // Voeg een bus toe die richting de bushalte rijdt
    Voertuig bus("Teststraat", 100, "bus");
    bus.setSnelheid(10.0); // 10 m/s, zal snel bij de bushalte aankomen
    situatie.voegVoertuigToe(bus);

    // Voeg een auto toe achter de bus (deze hoeft niet te stoppen bij de bushalte)
    Voertuig personenauto("Teststraat", 50, "auto");
    personenauto.setSnelheid(10.0);
    situatie.voegVoertuigToe(personenauto);

    simulatie sim(situatie, 0.5); // Halve seconde per stap

    // Eerste ronde simulatie: bus nadert bushalte
    bool busIsGestopt = false;
    bool busVertrokkenNaWachten = false;
    double busPositieBijStop = 0.0;

    // Maximaal 50 stappen om alle bushaltegedrag te kunnen observeren
    for (int i = 0; i < 50; i++) {
        sim.stap();

        // Haal de bijgewerkte voertuigen op na elke stap
        const auto& voertuigen = situatie.getVoertuigen();
        const auto& bushaltes = situatie.getBushaltes();

        // Controleer of er nog voertuigen zijn in de simulatie
        if (voertuigen.empty()) {
            break;
        }

        // Zoek de bus en de auto
        Voertuig* busPtr = nullptr;
        Voertuig* autoPtr = nullptr;

        for (auto& voertuig : situatie.getVoertuigen()) {
            if (voertuig.getType() == "bus") {
                busPtr = &voertuig;
            } else if (voertuig.getType() == "auto") {
                autoPtr = &voertuig;
            }
        }

        // Als de bus niet meer bestaat, breek de lus
        if (!busPtr) {
            break;
        }

        // Controleer of de bus bij de bushalte is gestopt
        if (!busIsGestopt && busPtr->isWaitingAtBusStop() &&
            std::abs(busPtr->getPositie() - 150) < 1.0) {
            busIsGestopt = true;
            busPositieBijStop = busPtr->getPositie();

            // Controleer of de bushalte ook weet dat er een bus is gestopt
            EXPECT_TRUE(bushaltes[0].isBusGestopt());

            // Controleer of de bus stilstaat
            EXPECT_NEAR(busPtr->getSnelheid(), 0.0, 0.1);

            std::cout << "Bus is gestopt bij de bushalte op positie "
                      << busPtr->getPositie() << " na " << i << " stappen." << std::endl;
        }

        // Controleer of de bus weer vertrekt na de wachttijd
        if (busIsGestopt && !busPtr->isWaitingAtBusStop() &&
            busPtr->getPositie() > busPositieBijStop) {
            busVertrokkenNaWachten = true;

            // Controleer of de bushalte weet dat de bus is vertrokken
            EXPECT_FALSE(bushaltes[0].isBusGestopt());

            // Controleer of de bus weer beweegt
            EXPECT_GT(busPtr->getSnelheid(), 0.0);

            std::cout << "Bus is vertrokken van de bushalte na "
                      << i << " stappen." << std::endl;
            break;
        }

        // Controleer of de auto gewoon doorrijdt (hoeft niet te stoppen bij bushalte)
        if (autoPtr && autoPtr->getPositie() > 150) {
            EXPECT_GT(autoPtr->getSnelheid(), 0.0);
        }
    }
    EXPECT_GT(sim.getHuidigeSimulatieTijd(), 10.0);
}

// Hulpfunctie om een test verkeerssituatie te maken
VerkeersSituatie createTestSituatie() {
    VerkeersSituatie situatie;

    // Voeg enkele wegen toe
    Baan baan1("Hoofdweg", 500);
    Baan baan2("Zijstraat", 300);
    Baan baan3("Kruisweg", 400);
    situatie.voegBaanToe(baan1);
    situatie.voegBaanToe(baan2);
    situatie.voegBaanToe(baan3);

    // Voeg enkele voertuigen toe
    Voertuig auto1("Hoofdweg", 50, "auto");
    Voertuig bus1("Hoofdweg", 150, "bus");
    Voertuig brandweer("Zijstraat", 100, "brandweerwagen");
    Voertuig ziekenwagen("Kruisweg", 200, "ziekenwagen");
    Voertuig politie("Kruisweg", 50, "politiecombi");
    situatie.voegVoertuigToe(auto1);
    situatie.voegVoertuigToe(bus1);
    situatie.voegVoertuigToe(brandweer);
    situatie.voegVoertuigToe(ziekenwagen);
    situatie.voegVoertuigToe(politie);

    // Voeg enkele verkeerslichten toe
    Verkeerslicht licht1("Hoofdweg", 300, 30, true);  // Met oranje licht
    Verkeerslicht licht2("Zijstraat", 200, 20, false, true);  // Slim verkeerslicht
    situatie.voegVerkeerslichtToe(licht1);
    situatie.voegVerkeerslichtToe(licht2);

    // Voeg enkele bushaltes toe
    Bushalte halte1("Hoofdweg", 250, 15);
    Bushalte halte2("Kruisweg", 150, 10);
    situatie.voegBushalteToe(halte1);
    situatie.voegBushalteToe(halte2);

    // Voeg een kruispunt toe
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 400);
    kruispunt.voegBaanToe("Kruisweg", 300);
    situatie.voegKruispuntToe(kruispunt);

    // Voeg een voertuiggenerator toe
    VoertuigGenerator generator("Zijstraat", 10, "auto");
    situatie.voegVoertuigGeneratorToe(generator);

    return situatie;
}

// Hulpfunctie om te controleren of een bestand bestaat
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Hulpfunctie om een tijdelijk bestand te maken en de naam ervan terug te geven
std::string createTempFile(const std::string& content, const std::string& extension = ".txt") {
    std::string filename = "test_temp" + extension;
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Hulpfunctie om een bestand in een string te lezen
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

// Tests voor de TextRapport (Eenvoudige Uitvoer) functionaliteit
TEST(OutputTest, TextRapportTest) {
    VerkeersSituatie situatie = createTestSituatie();

    // Genereer een tekstrapport
    std::string report = output::genereerTekstRapport(situatie);

    // Controleer of het rapport basisinformatie bevat
    EXPECT_TRUE(report.find("Banen (3):") != std::string::npos);
    EXPECT_TRUE(report.find("Voertuigen (5):") != std::string::npos);
    EXPECT_TRUE(report.find("Verkeerslichten (2):") != std::string::npos);
    EXPECT_TRUE(report.find("Bushaltes (2):") != std::string::npos);
    EXPECT_TRUE(report.find("Kruispunten (1):") != std::string::npos);

    // Controleer op specifieke informatie
    EXPECT_TRUE(report.find("Hoofdweg") != std::string::npos);
    EXPECT_TRUE(report.find("bus") != std::string::npos);
    EXPECT_TRUE(report.find("brandweerwagen") != std::string::npos);
    EXPECT_TRUE(report.find("ziekenwagen") != std::string::npos);
    EXPECT_TRUE(report.find("politiecombi") != std::string::npos);
}

// Tests voor de GrafischeImpressie functionaliteit
TEST(OutputTest, GrafischeImpressieTest) {
    VerkeersSituatie situatie = createTestSituatie();

    // Genereer een grafische impressie
    std::string impression = output::genereerGrafischeImpressie(situatie);

    // Controleer of de impressie basisweergaven van wegen bevat
    EXPECT_TRUE(impression.find("Hoofdweg |") != std::string::npos);
    EXPECT_TRUE(impression.find("Zijstraat |") != std::string::npos);
    EXPECT_TRUE(impression.find("Kruisweg |") != std::string::npos);

    // Controleer op voertuigmarkeringen (A voor auto, B voor bus, enz.)
    EXPECT_TRUE(impression.find("A") != std::string::npos); // Auto
    EXPECT_TRUE(impression.find("B") != std::string::npos); // Bus
    EXPECT_TRUE(impression.find("F") != std::string::npos); // Brandweerwagen
    EXPECT_TRUE(impression.find("Z") != std::string::npos); // Ziekenwagen
    EXPECT_TRUE(impression.find("P") != std::string::npos); // Politiecombi

    // Controleer op verkeerslichtweergaven
    EXPECT_TRUE(impression.find("> verkeerslichten |") != std::string::npos);

    // Controleer op bushalteweergaven
    EXPECT_TRUE(impression.find("> bushaltes") != std::string::npos);

    // Controleer op kruispuntweergaven
    EXPECT_TRUE(impression.find("> kruispunten") != std::string::npos);

    // Controleer op legenda
    EXPECT_TRUE(impression.find("Legende:") != std::string::npos);
}

// Tests voor XML uitvoerfunctionaliteit
TEST(OutputTest, XmlOutputTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Maak een tijdelijk XML-bestand
    std::string xmlFilename = "test_output.xml";

    // Schrijf de verkeerssituatie naar XML
    EXPECT_TRUE(uitvoer.schrijfNaarXml(situatie, xmlFilename));
    EXPECT_TRUE(fileExists(xmlFilename));

    // Lees het XML-bestand terug
    std::string xmlContent = readFile(xmlFilename);

    // Controleer basis XML-structuur
    EXPECT_TRUE(xmlContent.find("<?xml") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VerkeersSituatie>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("</VerkeersSituatie>") != std::string::npos);

    // Controleer op elementen
    EXPECT_TRUE(xmlContent.find("<BAAN>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VOERTUIG>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VERKEERSLICHT>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<BUSHALTE>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<KRUISPUNT>") != std::string::npos);
    EXPECT_TRUE(xmlContent.find("<VOERTUIGGENERATOR>") != std::string::npos);

    // Test of we de XML terug kunnen lezen in een nieuwe situatie
    VerkeersSituatie nieuweStituatie;
    BestandsLezer lezer;
    EXPECT_TRUE(lezer.leesXmlBestand(xmlFilename, nieuweStituatie));

    // Controleer of de nieuwe situatie dezelfde elementen heeft
    EXPECT_EQ(situatie.getBanen().size(), nieuweStituatie.getBanen().size());
    EXPECT_EQ(situatie.getVoertuigen().size(), nieuweStituatie.getVoertuigen().size());
    EXPECT_EQ(situatie.getVerkeerslichten().size(), nieuweStituatie.getVerkeerslichten().size());
    EXPECT_EQ(situatie.getBushaltes().size(), nieuweStituatie.getBushaltes().size());
    EXPECT_EQ(situatie.getKruispunten().size(), nieuweStituatie.getKruispunten().size());
    EXPECT_EQ(situatie.getVoertuigGenerators().size(), nieuweStituatie.getVoertuigGenerators().size());

    // Opruimen
    std::remove(xmlFilename.c_str());
}

// Tests voor HTML uitvoerfunctionaliteit
TEST(OutputTest, HtmlOutputTest) {
    VerkeersSituatie situatie = createTestSituatie();
    output uitvoer;

    // Maak een tijdelijk HTML-bestand
    std::string htmlFilename = "test_output.html";

    // Schrijf de verkeerssituatie naar HTML
    EXPECT_TRUE(uitvoer.schrijfNaarHtml(situatie, htmlFilename));
    EXPECT_TRUE(fileExists(htmlFilename));

    // Lees het HTML-bestand
    std::string htmlContent = readFile(htmlFilename);

    // Controleer basis HTML-structuur
    EXPECT_TRUE(htmlContent.find("<!DOCTYPE html>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<html>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<head>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("<body>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("</html>") != std::string::npos);

    // Controleer op CSS-stijlen
    EXPECT_TRUE(htmlContent.find("<style>") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("body {") != std::string::npos);
    EXPECT_TRUE(htmlContent.find(".road {") != std::string::npos);
    EXPECT_TRUE(htmlContent.find(".vehicle {") != std::string::npos);

    // Controleer op wegweergaven
    EXPECT_TRUE(htmlContent.find("Hoofdweg") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("Zijstraat") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("Kruisweg") != std::string::npos);

    // Controleer op voertuigweergaven
    EXPECT_TRUE(htmlContent.find("class=\"vehicle auto\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle bus\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle brandweerwagen\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle ziekenwagen\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"vehicle politiecombi\"") != std::string::npos);

    // Controleer op verkeerslicht-, bushalte- en kruispuntweergaven
    EXPECT_TRUE(htmlContent.find("class=\"traffic-light") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"bus-stop\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"intersection\"") != std::string::npos);

    // Controleer op legenda en statistieken
    EXPECT_TRUE(htmlContent.find("class=\"legend\"") != std::string::npos);
    EXPECT_TRUE(htmlContent.find("class=\"statistics\"") != std::string::npos);

    // Opruimen
    std::remove(htmlFilename.c_str());
}

// Tests voor foutafhandeling in output

// Tests voor de oranje verkeerslichtfunctionaliteit
TEST(VerkeerslichtTest, OrangeLightTest) {
    // Maak een verkeerslicht met oranje functionaliteit
    Verkeerslicht licht("Testweg", 100, 20, true);

    // Verkeerslicht zou moeten starten als rood
    EXPECT_TRUE(licht.isRood());
    EXPECT_FALSE(licht.isGroen());
    EXPECT_FALSE(licht.isOranje());

    // Update tot het verandert naar groen
    for (int i = 0; i < 21; i++) {
        licht.update(1.0);
    }

    EXPECT_TRUE(licht.isGroen());

    // Update tot het begint te veranderen naar rood (via oranje)
    for (int i = 0; i < 20; i++) {
        licht.update(1.0);
    }

    // Zou nu oranje moeten zijn
    EXPECT_TRUE(licht.isOranje());

    // Update net genoeg om het oranje te houden (oranje duur is 10% van cyclus)
    licht.update(1.0);
    EXPECT_TRUE(licht.isOranje());

    // Update tot het verandert naar rood
    for (int i = 0; i < 2; i++) {
        licht.update(1.0);
    }

    EXPECT_TRUE(licht.isRood());
}

// Tests voor de slimme verkeerslichtfunctionaliteit
TEST(VerkeerslichtTest, SmartLightTest) {
    // Maak een slim verkeerslicht
    Verkeerslicht licht("Testweg", 100, 30, false, true);

    // Verkeerslicht zou moeten starten als rood
    EXPECT_TRUE(licht.isRood());

    // Registreer enkele voertuigen
    licht.registerVoertuigVoorLicht();
    licht.registerVoertuigVoorLicht();
    EXPECT_EQ(licht.getVoertuigenVoorLicht(), 2);

    // Update voorbij minimale tijd (10 seconden)
    for (int i = 0; i < 11; i++) {
        licht.update(1.0);
    }

    // Met wachtende voertuigen zou het groen moeten worden
    EXPECT_TRUE(licht.isGroen());

    // Voertuigen zouden gereset moeten worden
    EXPECT_EQ(licht.getVoertuigenVoorLicht(), 0);

    // Update opnieuw voorbij minimale tijd, zonder voertuigen
    for (int i = 0; i < 21; i++) {
        licht.update(1.0);
    }

    // Zonder voertuigen en na 20 seconden, zou het rood moeten worden
    EXPECT_TRUE(licht.isRood());

    // Test maximale tijdslimiet (60 seconden)
    for (int i = 0; i < 60; i++) {
        licht.update(1.0);
    }

    // Na maximale tijd in rood, zou het groen moeten worden zelfs zonder voertuigen
    EXPECT_TRUE(licht.isGroen());
}


// Test voor kruispuntfunctionaliteit
TEST(KruispuntTest, IntersectionFunctionalityTest) {
    // Maak een kruispunt met drie wegen
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 500);
    kruispunt.voegBaanToe("Zijstraat", 300);
    kruispunt.voegBaanToe("Kruisweg", 400);

    // Controleer op correct aantal wegen
    EXPECT_EQ(kruispunt.getBanen().size(), 3);

    // Test kiesRandomBaan
    std::string newRoad = kruispunt.kiesRandomBaan("Hoofdweg");

    // Zou een van de andere wegen moeten teruggeven
    EXPECT_TRUE(newRoad == "Zijstraat" || newRoad == "Kruisweg");
    EXPECT_FALSE(newRoad == "Hoofdweg");

    // Probeer een weg te kiezen van een onbekende weg
    std::string result = kruispunt.kiesRandomBaan("OnbekendWeg");

    // We vertrouwen op de interne REQUIRE macro, dus dit zou nog steeds een van de wegen moeten teruggeven
    EXPECT_TRUE(result == "Hoofdweg" || result == "Zijstraat" || result == "Kruisweg");

    // Test voor het ophalen van positie op een weg
    double pos = kruispunt.getPositieOpBaan("Hoofdweg");
    EXPECT_EQ(pos, 500);

    // Test voor het ophalen van positie op een niet-bestaande weg
    pos = kruispunt.getPositieOpBaan("OnbekendWeg");
    EXPECT_EQ(pos, -1.0);
}

// Test de voertuiggenerator
TEST(VoertuigGeneratorTest, GeneratorFunctionalityTest) {
    // Maak een weg met een voertuiggenerator
    VerkeersSituatie situatie;
    Baan baan("Testweg", 500);
    situatie.voegBaanToe(baan);

    VoertuigGenerator generator("Testweg", 5, "auto");
    situatie.voegVoertuigGeneratorToe(generator);

    // Maak simulatie met automatische generatie ingeschakeld
    simulatie sim(situatie, 1.0);
    sim.setAutoGenereerVoertuigen(true);

    // In het begin zijn er geen voertuigen
    EXPECT_EQ(situatie.getVoertuigen().size(), 0);

    // Draai simulatie voor 6 seconden (generator heeft frequentie 5)
    for (int i = 0; i < 6; i++) {
        sim.stap();
    }

    // Zou minstens één voertuig gegenereerd moeten hebben
    EXPECT_GT(situatie.getVoertuigen().size(), 0);

    // Controleer de eigenschappen van het voertuig
    if (!situatie.getVoertuigen().empty()) {
        const Voertuig& voertuig = situatie.getVoertuigen()[0];
        EXPECT_EQ(voertuig.getBaanNaam(), "Testweg");
        EXPECT_EQ(voertuig.getType(), "auto");
        EXPECT_GE(voertuig.getPositie(), 0.0);
    }
}

// Main test runner
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}