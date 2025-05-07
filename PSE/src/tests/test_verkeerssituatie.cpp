/**
 * @file test_verkeerssituatie.cpp
 * @brief Tests for the VerkeersSituatie class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../Situation/situatie.h"
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"
#include "../TraficObjects/bushalte.h"
#include "../TraficObjects/kruispunt.h"
#include "../TraficObjects/voertuiggenerator.h"

// Tests for the VerkeersSituatie class - Adding roads
TEST(VerkeersSituatieTest, VoegBaanToe) {
    VerkeersSituatie situatie;
    Baan baan1("Teststraat", 250);
    Baan baan2("Hoofdweg", 500);
    Baan baan3("Bermuda", 8);

    // First addition should succeed
    EXPECT_TRUE(situatie.voegBaanToe(baan1));

    // Second addition with different name should succeed
    EXPECT_TRUE(situatie.voegBaanToe(baan2));

    // Third road should also be added successfully
    EXPECT_TRUE(situatie.voegBaanToe(baan3));

    // Duplicate road should fail
    EXPECT_FALSE(situatie.voegBaanToe(baan1));

    // Check that we can get the roads back
    const auto& banen = situatie.getBanen();
    EXPECT_EQ(3, count(banen));
    EXPECT_TRUE(banen.find("Teststraat") != banen.end());
    EXPECT_TRUE(banen.find("Hoofdweg") != banen.end());
    EXPECT_TRUE(banen.find("Bermuda") != banen.end());
}

// Tests for adding vehicles
TEST(VerkeersSituatieTest, VoegVoertuigToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // Add a valid road
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // Valid vehicle on an existing road should succeed
    auto voertuig1 = Voertuig::maakVoertuig("Teststraat", 0, "auto");
    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig1)));

    // Vehicle at the end of the road should succeed
    auto voertuig2 = Voertuig::maakVoertuig("Teststraat", 250, "auto");
    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig2)));

    // Vehicle on a non-existent road should fail
    auto voertuig3 = Voertuig::maakVoertuig("NietBestaandeBaan", 0, "auto");
    EXPECT_FALSE(situatie.voegVoertuigToe(std::move(voertuig3)));

    // Vehicle with invalid position should fail
    auto voertuig4 = Voertuig::maakVoertuig("Teststraat", 251, "auto");
    EXPECT_FALSE(situatie.voegVoertuigToe(std::move(voertuig4)));

    // Vehicle with negative position should fail
    auto voertuig5 = Voertuig::maakVoertuig("Teststraat", -1, "auto");
    EXPECT_FALSE(situatie.voegVoertuigToe(std::move(voertuig5)));

    // Test vehicle count
    EXPECT_EQ(2, count(situatie.getVoertuigen()));
}

// Tests for adding traffic lights
TEST(VerkeersSituatieTest, VoegVerkeerslichtToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // Add a valid road first
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // Valid traffic light should succeed
    Verkeerslicht verkeerslicht1("Teststraat", 100, 20);
    EXPECT_TRUE(situatie.voegVerkeerslichtToe(verkeerslicht1));

    // Traffic light on non-existent road should fail
    Verkeerslicht verkeerslicht2("NietBestaandeBaan", 100, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht2));

    // Traffic light with invalid position should fail
    Verkeerslicht verkeerslicht3("Teststraat", 251, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht3));

    // Traffic light with negative position should fail
    Verkeerslicht verkeerslicht4("Teststraat", -1, 20);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht4));

    // Traffic light with zero cycle time should fail
    Verkeerslicht verkeerslicht5("Teststraat", 100, 0);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht5));

    // Traffic light with negative cycle time should fail
    Verkeerslicht verkeerslicht6("Teststraat", 100, -1);
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht6));

    // Traffic light too close to existing one should fail
    Verkeerslicht verkeerslicht7("Teststraat", 149, 20); // 49m distance, less than 50m required
    EXPECT_FALSE(situatie.voegVerkeerslichtToe(verkeerslicht7));

    // Traffic light at minimum distance should succeed
    Verkeerslicht verkeerslicht8("Teststraat", 50, 20); // 50m from position 100
    EXPECT_TRUE(situatie.voegVerkeerslichtToe(verkeerslicht8));

    // Check traffic light count
    EXPECT_EQ(2, count(situatie.getVerkeerslichten()));
}

// Tests for adding bus stops
TEST(VerkeersSituatieTest, VoegBushalteToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // Add a valid road first
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // Valid bus stop should succeed
    Bushalte bushalte1("Teststraat", 100, 20);
    EXPECT_TRUE(situatie.voegBushalteToe(bushalte1));

    // Bus stop on non-existent road should fail
    Bushalte bushalte2("NietBestaandeBaan", 100, 20);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte2));

    // Bus stop with invalid position should fail
    Bushalte bushalte3("Teststraat", 251, 20);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte3));

    // Bus stop with negative position should fail
    Bushalte bushalte4("Teststraat", -1, 20);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte4));

    // Bus stop with zero wait time should fail
    Bushalte bushalte5("Teststraat", 150, 0);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte5));

    // Bus stop with negative wait time should fail
    Bushalte bushalte6("Teststraat", 150, -1);
    EXPECT_FALSE(situatie.voegBushalteToe(bushalte6));

    // Check bus stop count
    EXPECT_EQ(1, count(situatie.getBushaltes()));
}

// Tests for adding intersections
TEST(VerkeersSituatieTest, VoegKruispuntToe) {
    VerkeersSituatie situatie;
    Baan baan1("Teststraat", 250);
    Baan baan2("Zijstraat", 300);

    // Add roads first
    EXPECT_TRUE(situatie.voegBaanToe(baan1));
    EXPECT_TRUE(situatie.voegBaanToe(baan2));

    // Create an intersection
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Teststraat", 150);
    kruispunt.voegBaanToe("Zijstraat", 200);

    // Adding should succeed
    EXPECT_TRUE(situatie.voegKruispuntToe(kruispunt));

    // Create an intersection with a non-existent road
    Kruispunt kruispunt2;
    kruispunt2.voegBaanToe("Teststraat", 150);
    kruispunt2.voegBaanToe("NietBestaandeBaan", 200);

    // Adding should fail
    EXPECT_FALSE(situatie.voegKruispuntToe(kruispunt2));

    // Create an intersection with invalid position
    Kruispunt kruispunt3;
    kruispunt3.voegBaanToe("Teststraat", 150);
    kruispunt3.voegBaanToe("Zijstraat", 301); // Beyond road length

    // Adding should fail
    EXPECT_FALSE(situatie.voegKruispuntToe(kruispunt3));

    // Check intersection count
    EXPECT_EQ(1, count(situatie.getKruispunten()));
}

// Tests for adding vehicle generators
TEST(VerkeersSituatieTest, VoegVoertuigGeneratorToe) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // Add a valid road first
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // Valid generator should succeed
    VoertuigGenerator generator1("Teststraat", 10);
    EXPECT_TRUE(situatie.voegVoertuigGeneratorToe(generator1));

    // Generator on non-existent road should fail
    VoertuigGenerator generator2("NietBestaandeBaan", 10);
    EXPECT_FALSE(situatie.voegVoertuigGeneratorToe(generator2));

    // Generator with invalid frequency should fail (tested in VoertuigGenerator tests)

    // Check generator count
    EXPECT_EQ(1, count(situatie.getVoertuigGenerators()));
}

// Test consistency verification
TEST(VerkeersSituatieTest, VerificeerConsistentie) {
    VerkeersSituatie situatie;

    // Situation with nothing in it should fail
    EXPECT_FALSE(situatie.verificeerConsistentie());

    // Situation with a valid road should succeed
    Baan baan("Teststraat", 250);
    EXPECT_TRUE(situatie.voegBaanToe(baan));
    EXPECT_TRUE(situatie.verificeerConsistentie());

    // Add a valid vehicle
    auto voertuig1 = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig1)));
    EXPECT_TRUE(situatie.verificeerConsistentie());

    // Add a valid traffic light
    Verkeerslicht verkeerslicht1("Teststraat", 150, 20);
    EXPECT_TRUE(situatie.voegVerkeerslichtToe(verkeerslicht1));
    EXPECT_TRUE(situatie.verificeerConsistentie());

    // Add a valid bus stop
    Bushalte bushalte1("Teststraat", 200, 10);
    EXPECT_TRUE(situatie.voegBushalteToe(bushalte1));
    EXPECT_TRUE(situatie.verificeerConsistentie());

    // Add a valid vehicle generator
    VoertuigGenerator generator1("Teststraat", 10);
    EXPECT_TRUE(situatie.voegVoertuigGeneratorToe(generator1));
    EXPECT_TRUE(situatie.verificeerConsistentie());
}

// Test for verwijderVoertuig
TEST(VerkeersSituatieTest, VerwijderVoertuig) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);

    // Add a valid road
    EXPECT_TRUE(situatie.voegBaanToe(baan));

    // Add three vehicles
    auto voertuig1 = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    auto voertuig2 = Voertuig::maakVoertuig("Teststraat", 100, "bus");
    auto voertuig3 = Voertuig::maakVoertuig("Teststraat", 150, "brandweerwagen");

    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig1)));
    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig2)));
    EXPECT_TRUE(situatie.voegVoertuigToe(std::move(voertuig3)));

    EXPECT_EQ(3, count(situatie.getVoertuigen()));

    // Remove second vehicle
    EXPECT_TRUE(situatie.verwijderVoertuig(1));
    EXPECT_EQ(2, count(situatie.getVoertuigen()));

    // First remaining vehicle should be an auto
    EXPECT_EQ("auto", situatie.getVoertuigen()[0]->getType());

    // Second remaining vehicle should be a brandweerwagen
    EXPECT_EQ("brandweerwagen", situatie.getVoertuigen()[1]->getType());

    // Invalid index should fail
    EXPECT_FALSE(situatie.verwijderVoertuig(-1));
    EXPECT_FALSE(situatie.verwijderVoertuig(2)); // Only indices 0,1 now valid

    // Remove all vehicles
    EXPECT_TRUE(situatie.verwijderVoertuig(0));
    EXPECT_TRUE(situatie.verwijderVoertuig(0));
    EXPECT_EQ(0, count(situatie.getVoertuigen()));
}

// Test utility methods for finding objects
TEST(VerkeersSituatieTest, ZoekMethods) {
    VerkeersSituatie situatie;

    // Add roads
    Baan baan1("Hoofdweg", 500);
    Baan baan2("Zijstraat", 300);
    situatie.voegBaanToe(baan1);
    situatie.voegBaanToe(baan2);

    // Add bus stops
    Bushalte bushalte1("Hoofdweg", 100, 10);
    Bushalte bushalte2("Hoofdweg", 300, 15);
    Bushalte bushalte3("Zijstraat", 150, 10);
    situatie.voegBushalteToe(bushalte1);
    situatie.voegBushalteToe(bushalte2);
    situatie.voegBushalteToe(bushalte3);

    // Add traffic lights
    Verkeerslicht licht1("Hoofdweg", 200, 30);
    Verkeerslicht licht2("Zijstraat", 100, 20);
    situatie.voegVerkeerslichtToe(licht1);
    situatie.voegVerkeerslichtToe(licht2);

    // Add intersections
    Kruispunt kruispunt1;
    kruispunt1.voegBaanToe("Hoofdweg", 250);
    kruispunt1.voegBaanToe("Zijstraat", 50);
    situatie.voegKruispuntToe(kruispunt1);

    // Add vehicles
    auto bus = Voertuig::maakVoertuig("Hoofdweg", 50, "bus");
    auto auto1 = Voertuig::maakVoertuig("Hoofdweg", 150, "auto");
    auto auto2 = Voertuig::maakVoertuig("Zijstraat", 80, "auto");

    situatie.voegVoertuigToe(std::move(bus));
    situatie.voegVoertuigToe(std::move(auto1));
    situatie.voegVoertuigToe(std::move(auto2));

    // Test zoekBushaltesOpBaan
    auto bushaltes = situatie.zoekBushaltesOpBaan("Hoofdweg");
    EXPECT_EQ(2, count(bushaltes));
    // Check they are sorted by position
    EXPECT_EQ(100, bushaltes[0]->getPositie());
    EXPECT_EQ(300, bushaltes[1]->getPositie());

    // Test zoekEerstvolgendeHalte for a bus
    auto eersteHalte = situatie.zoekEerstvolgendeHalte(*situatie.getVoertuigen()[0]);
    EXPECT_NE(nullptr, eersteHalte);
    EXPECT_EQ(100, eersteHalte->getPositie());

    // Test zoekEerstvolgendeHalte for non-bus (should return nullptr)
    auto geenHalte = situatie.zoekEerstvolgendeHalte(*situatie.getVoertuigen()[1]);
    EXPECT_EQ(nullptr, geenHalte);

    // Test zoekEerstvolgendeVerkeerslicht
    auto eersteVerkeerslicht = situatie.zoekEerstvolgendeVerkeerslicht(*situatie.getVoertuigen()[1]);
    EXPECT_NE(nullptr, eersteVerkeerslicht);
    EXPECT_EQ(200, eersteVerkeerslicht->getPositie());

    // Test zoekKruispuntenOpBaan
    auto kruispunten = situatie.zoekKruispuntenOpBaan("Hoofdweg");
    EXPECT_EQ(1, count(kruispunten));
    EXPECT_EQ(250, kruispunten[0]->getPositieOpBaan("Hoofdweg"));

    // Test zoekEerstvolgendeKruispunt
    auto eersteKruispunt = situatie.zoekEerstvolgendeKruispunt(*situatie.getVoertuigen()[0]);
    EXPECT_NE(nullptr, eersteKruispunt);
    EXPECT_EQ(250, eersteKruispunt->getPositieOpBaan("Hoofdweg"));

    // Test when next bus stop is behind the bus (should return nullptr)
    auto bus2 = Voertuig::maakVoertuig("Hoofdweg", 350, "bus");
    situatie.voegVoertuigToe(std::move(bus2));
    auto geenVolgendeHalte = situatie.zoekEerstvolgendeHalte(*situatie.getVoertuigen()[3]);
    EXPECT_EQ(nullptr, geenVolgendeHalte);
}

// Test vehicle cloning
TEST(VerkeersSituatieTest, KopieVoertuigen) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    auto voertuig1 = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    auto voertuig2 = Voertuig::maakVoertuig("Teststraat", 100, "bus");
    situatie.voegVoertuigToe(std::move(voertuig1));
    situatie.voegVoertuigToe(std::move(voertuig2));

    // Test kopieVoertuigen
    auto kopieën = situatie.kopieVoertuigen();

    // Should have same number of vehicles
    EXPECT_EQ(count(situatie.getVoertuigen()), count(kopieën));

    // Check that properties are copied correctly
    EXPECT_EQ(situatie.getVoertuigen()[0]->getBaanNaam(), kopieën[0]->getBaanNaam());
    EXPECT_EQ(situatie.getVoertuigen()[0]->getPositie(), kopieën[0]->getPositie());
    EXPECT_EQ(situatie.getVoertuigen()[0]->getType(), kopieën[0]->getType());

    EXPECT_EQ(situatie.getVoertuigen()[1]->getBaanNaam(), kopieën[1]->getBaanNaam());
    EXPECT_EQ(situatie.getVoertuigen()[1]->getPositie(), kopieën[1]->getPositie());
    EXPECT_EQ(situatie.getVoertuigen()[1]->getType(), kopieën[1]->getType());

    // Change properties in the copy to verify it's a deep copy
    kopieën[0]->setPositie(150);
    EXPECT_NE(situatie.getVoertuigen()[0]->getPositie(), kopieën[0]->getPositie());
}