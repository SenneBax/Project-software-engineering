/**
 * @file test_verkeerslicht.cpp
 * @brief Tests for the Verkeerslicht class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../TraficObjects/verkeerslicht.h"

// Basic constructor and getters tests
TEST(VerkeerslichtTest, ConstructorAndGetters) {
    Verkeerslicht licht("Teststraat", 150, 30, true, false);
    EXPECT_EQ("Teststraat", licht.getBaan());
    EXPECT_EQ(150, licht.getPositie());
    EXPECT_EQ(30, licht.getCyclus());
    EXPECT_TRUE(licht.getHeeftOranje());
    EXPECT_FALSE(licht.getIsSlim());
}

// Test for traffic lights with orange lights
TEST(VerkeerslichtTest, OrangeTrafficLight) {
    Verkeerslicht licht("Teststraat", 150, 30, true, false);
    EXPECT_TRUE(licht.getHeeftOranje());
    EXPECT_TRUE(licht.isRood()); // Starts on red

    // String representation
    EXPECT_EQ("rood", licht.getKleurAsString());

    // Update light to green (cycle duration is 30 seconds)
    for (int i = 0; i < 31; i++) {
        licht.update(1.0);
    }

    EXPECT_TRUE(licht.isGroen());
    EXPECT_EQ("groen", licht.getKleurAsString());

    // Update until it changes to orange (after 30 more seconds)
    for (int i = 0; i < 30; i++) {
        licht.update(1.0);
    }

    // Should now be orange
    EXPECT_TRUE(licht.isOranje());
    EXPECT_EQ("oranje", licht.getKleurAsString());

    // Orange phase lasts 10% of cycle (3 seconds), then turns red
    for (int i = 0; i < 3; i++) {
        licht.update(1.0);
    }

    EXPECT_TRUE(licht.isRood());
}

// Test traffic light without orange phase
TEST(VerkeerslichtTest, NonOrangeTrafficLight) {
    Verkeerslicht licht("Teststraat", 150, 30, false, false);
    EXPECT_FALSE(licht.getHeeftOranje());
    EXPECT_TRUE(licht.isRood()); // Starts on red

    // Update to green
    for (int i = 0; i < 31; i++) {
        licht.update(1.0);
    }

    EXPECT_TRUE(licht.isGroen());

    // Update for one more cycle
    for (int i = 0; i < 30; i++) {
        licht.update(1.0);
    }

    // Should go directly to red, no orange phase
    EXPECT_TRUE(licht.isRood());
    EXPECT_FALSE(licht.isOranje());
}

// Tests for the smart traffic light functionality
TEST(VerkeerslichtTest, SmartLightTest) {
    // Create a smart traffic light
    Verkeerslicht licht("Testweg", 100, 30, false, true);

    // Traffic light should start as red
    EXPECT_TRUE(licht.isRood());
    EXPECT_EQ(0, licht.getVoertuigenVoorLicht());

    // Register some vehicles
    licht.registerVoertuigVoorLicht();
    licht.registerVoertuigVoorLicht();
    EXPECT_EQ(2, licht.getVoertuigenVoorLicht());

    // Update past minimum time (10 seconds)
    for (int i = 0; i < 11; i++) {
        licht.update(1.0);
    }

    // With waiting vehicles, it should turn green
    EXPECT_TRUE(licht.isGroen());

    // Vehicles should be reset
    EXPECT_EQ(0, licht.getVoertuigenVoorLicht());

    // Update beyond minimum time, without vehicles
    for (int i = 0; i < 21; i++) {
        licht.update(1.0);
    }

    // Without vehicles and after 20 seconds, it should turn red
    EXPECT_TRUE(licht.isRood());

    // Test maximum time limit (60 seconds)
    for (int i = 0; i < 60; i++) {
        licht.update(1.0);
    }

    // After maximum time in red, it should turn green even without vehicles
    EXPECT_TRUE(licht.isGroen());

    // Test unregistering vehicle
    licht.registerVoertuigVoorLicht();
    licht.registerVoertuigVoorLicht();
    EXPECT_EQ(2, licht.getVoertuigenVoorLicht());

    licht.unregisterVoertuigVoorLicht();
    EXPECT_EQ(1, licht.getVoertuigenVoorLicht());

    licht.resetVoertuigenVoorLicht();
    EXPECT_EQ(0, licht.getVoertuigenVoorLicht());
}

// Test the color setters directly
TEST(VerkeerslichtTest, ColorSetters) {
    Verkeerslicht licht("Teststraat", 150, 30);

    licht.setKleur(Verkeerslicht::Kleur::GROEN);
    EXPECT_TRUE(licht.isGroen());

    licht.setKleur(Verkeerslicht::Kleur::ORANJE);
    EXPECT_TRUE(licht.isOranje());

    licht.setKleur(Verkeerslicht::Kleur::ROOD);
    EXPECT_TRUE(licht.isRood());
}