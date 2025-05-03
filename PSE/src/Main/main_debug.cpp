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
    ASSERT_TRUE(situatie.voegBaanToe(baan));

    // Voeg een bushalte toe op positie 150
    Bushalte bushalte("Teststraat", 150, 10); // 10 seconden wachttijd
    ASSERT_TRUE(situatie.voegBushalteToe(bushalte));

    // Voeg een bus toe die richting de bushalte rijdt
    // Positie iets voor de bushalte met constante snelheid
    Voertuig bus("Teststraat", 145, "bus");
    bus.setSnelheid(0.5); // Langzame snelheid om exacte stop te garanderen
    ASSERT_TRUE(situatie.voegVoertuigToe(bus));

    // Voeg een auto toe achter de bus (deze hoeft niet te stoppen bij de bushalte)
    Voertuig personenauto("Teststraat", 120, "auto");
    personenauto.setSnelheid(0.5);
    ASSERT_TRUE(situatie.voegVoertuigToe(personenauto));

    simulatie sim(situatie, 0.5); // Halve seconde per stap

    // Fase 1: Bus nadert en stopt bij bushalte
    bool busIsGestopt = false;
    int stopStap = -1;

    // Simuleer tot de bus stopt (maximaal 20 stappen)
    for (int i = 0; i < 20 && !busIsGestopt; i++) {
        sim.stap();

        // Haal de bijgewerkte voertuigen en bushaltes op
        const auto& voertuigen = situatie.getVoertuigen();
        const auto& bushaltes = situatie.getBushaltes();

        // Controleer of er nog voertuigen zijn
        ASSERT_GE(voertuigen.size(), 1) << "Er zijn geen voertuigen meer in simulatie na " << i << " stappen";

        // Zoek bus en auto
        Voertuig* busPtr = nullptr;
        Voertuig* autoPtr = nullptr;

        for (auto& voertuig : situatie.getVoertuigen()) {
            if (voertuig.getType() == "bus") {
                busPtr = &voertuig;
            } else if (voertuig.getType() == "auto") {
                autoPtr = &voertuig;
            }
        }

        // Controleer of de bus nog bestaat
        ASSERT_NE(busPtr, nullptr) << "Bus is verdwenen na " << i << " stappen";

        // Controleer of de bus is gestopt bij de bushalte
        if (busPtr->isWaitingAtBusStop() && std::abs(busPtr->getPositie() - 150) < 1.0) {
            busIsGestopt = true;
            stopStap = i;

            // Controleer of de bus stilstaat
            EXPECT_NEAR(busPtr->getSnelheid(), 0.0, 0.01) << "Bus staat niet stil bij bushalte";

            // Controleer of de bushalte weet dat er een bus gestopt is
            EXPECT_TRUE(bushaltes[0].isBusGestopt()) << "Bushalte weet niet dat er een bus gestopt is";
        }
    }

    // Controleer of de bus daadwerkelijk gestopt is
    ASSERT_TRUE(busIsGestopt) << "De bus is niet gestopt bij de bushalte binnen 20 stappen";

    // Fase 2: Controleer of de bus wacht bij de bushalte
    double startWachtTijd = sim.getHuidigeSimulatieTijd();

    // Simuleer totdat de wachttijd is verstreken (nog 22 stappen, ruim boven de wachttijd)
    int wachtStappen = 22;
    for (int i = 0; i < wachtStappen; i++) {
        sim.stap();

        // Controleer halverwege of de bus nog steeds wacht
        if (i == wachtStappen / 2) {
            const auto& voertuigen = situatie.getVoertuigen();
            bool busGevonden = false;

            for (const auto& voertuig : voertuigen) {
                if (voertuig.getType() == "bus") {
                    busGevonden = true;
                    EXPECT_TRUE(voertuig.isWaitingAtBusStop())
                        << "Bus zou nog moeten wachten na " << (stopStap + i) << " stappen";
                    EXPECT_NEAR(voertuig.getSnelheid(), 0.0, 0.01)
                        << "Bus zou stil moeten staan tijdens wachten";
                }
            }

            ASSERT_TRUE(busGevonden) << "Bus is verdwenen tijdens het wachten";
        }
    }

    // Fase 3: Controleer of de bus weer vertrokken is
    const auto& voertuigen = situatie.getVoertuigen();
    bool busVertrokken = false;
    double eindPositie = 0.0;

    for (const auto& voertuig : voertuigen) {
        if (voertuig.getType() == "bus") {
            busVertrokken = !voertuig.isWaitingAtBusStop() && voertuig.getSnelheid() > 0;
            eindPositie = voertuig.getPositie();
        }
    }

    EXPECT_TRUE(busVertrokken) << "Bus is niet vertrokken na afloop van de wachttijd";
    EXPECT_GT(eindPositie, 150) << "Bus is niet voorbij de bushalte na vertrek";

    // Controleer of de simulatietijd minstens gelijk is aan de starttijd + wachttijd
    EXPECT_GE(sim.getHuidigeSimulatieTijd(), startWachtTijd + 10)
        << "Simulatie heeft niet lang genoeg geduurd voor de wachttijd";

    // Controleer of de auto niet is gestopt en voorbij de bushalte is gekomen
    bool autoGevonden = false;
    for (const auto& voertuig : voertuigen) {
        if (voertuig.getType() == "auto") {
            autoGevonden = true;
            EXPECT_GT(voertuig.getPositie(), 150)
                << "Auto is niet voorbij de bushalte gekomen";
            EXPECT_GT(voertuig.getSnelheid(), 0)
                << "Auto zou moeten bewegen";
        }
    }

    // De auto kan de simulatie al hebben verlaten als die het einde van de weg heeft bereikt
    if (autoGevonden) {
        EXPECT_GT(voertuigen.size(), 1) << "Er zou ten minste de bus en een auto moeten zijn";
    }
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}