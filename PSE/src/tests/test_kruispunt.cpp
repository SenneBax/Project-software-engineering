/**
 * @file test_kruispunt.cpp
 * @brief Tests for the Kruispunt class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../TraficObjects/kruispunt.h"

// Test for adding roads to an intersection
TEST(KruispuntTest, AddingRoads) {
    Kruispunt kruispunt;
    EXPECT_TRUE(kruispunt.voegBaanToe("Teststraat", 250));
    EXPECT_TRUE(kruispunt.voegBaanToe("Zijstraat", 150));
    EXPECT_FALSE(kruispunt.voegBaanToe("Teststraat", 300)); // Duplicate road name

    EXPECT_TRUE(kruispunt.bevatBaan("Teststraat"));
    EXPECT_FALSE(kruispunt.bevatBaan("Onbekendestraat"));

    EXPECT_EQ(250, kruispunt.getPositieOpBaan("Teststraat"));
    EXPECT_EQ(-1, kruispunt.getPositieOpBaan("Onbekendestraat"));
}

// Test for getting roads connected to the intersection
TEST(KruispuntTest, GetBanen) {
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Teststraat", 250);
    kruispunt.voegBaanToe("Zijstraat", 150);

    auto banen = kruispunt.getBanen();
    EXPECT_EQ(2, banen.size());

    // Check that the roads have the correct names and positions
    bool foundTeststraat = false;
    bool foundZijstraat = false;

    for (const auto& baan : banen) {
        if (baan.first == "Teststraat") {
            foundTeststraat = true;
            EXPECT_EQ(250, baan.second);
        } else if (baan.first == "Zijstraat") {
            foundZijstraat = true;
            EXPECT_EQ(150, baan.second);
        }
    }

    EXPECT_TRUE(foundTeststraat);
    EXPECT_TRUE(foundZijstraat);
}

// Test for randomly selecting a road
TEST(KruispuntTest, KiesRandomBaan) {
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 500);
    kruispunt.voegBaanToe("Zijstraat", 300);
    kruispunt.voegBaanToe("Kruisweg", 400);

    // Test kiesRandomBaan
    std::string newRoad = kruispunt.kiesRandomBaan("Hoofdweg");

    // Should return one of the other roads
    EXPECT_TRUE(newRoad == "Zijstraat" || newRoad == "Kruisweg");
    EXPECT_FALSE(newRoad == "Hoofdweg");

    // Test with a road that doesn't exist
    std::string result = kruispunt.kiesRandomBaan("OnbekendWeg");

    // Should still return one of the existing roads
    EXPECT_TRUE(result == "Hoofdweg" || result == "Zijstraat" || result == "Kruisweg");
}

// Test for intersection with only one road
TEST(KruispuntTest, SingleRoadIntersection) {
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 500);

    // Trying to choose a random road when there's only one should return empty string
    std::string newRoad = kruispunt.kiesRandomBaan("Hoofdweg");
    EXPECT_TRUE(newRoad.empty());
}

// Test proper initialization
TEST(KruispuntTest, ProperlyInitialized) {
    Kruispunt kruispunt;
    EXPECT_TRUE(kruispunt.properlyInitialized());

    // Adding a road should not affect proper initialization
    kruispunt.voegBaanToe("Teststraat", 250);
    EXPECT_TRUE(kruispunt.properlyInitialized());
}

// Test intersection functionality with multiple roads
TEST(KruispuntTest, IntersectionFunctionalityTest) {
    // Create an intersection with three roads
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 500);
    kruispunt.voegBaanToe("Zijstraat", 300);
    kruispunt.voegBaanToe("Kruisweg", 400);

    // Check correct number of roads
    EXPECT_EQ(kruispunt.getBanen().size(), 3);

    // Test for each road position
    EXPECT_EQ(500, kruispunt.getPositieOpBaan("Hoofdweg"));
    EXPECT_EQ(300, kruispunt.getPositieOpBaan("Zijstraat"));
    EXPECT_EQ(400, kruispunt.getPositieOpBaan("Kruisweg"));

    // Test that bevatBaan works correctly for all roads
    EXPECT_TRUE(kruispunt.bevatBaan("Hoofdweg"));
    EXPECT_TRUE(kruispunt.bevatBaan("Zijstraat"));
    EXPECT_TRUE(kruispunt.bevatBaan("Kruisweg"));
    EXPECT_FALSE(kruispunt.bevatBaan("OnbekendWeg"));
}