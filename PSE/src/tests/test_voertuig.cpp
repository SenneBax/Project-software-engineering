/**
 * @file test_voertuig.cpp
 * @brief Tests for the Voertuig class and its subclasses
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../TraficObjects/voertuig.h"

// Tests for the basic Voertuig functionality
TEST(VoertuigTest, ConstructorAndGetters) {
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 75, "auto");
    EXPECT_EQ("Teststraat", voertuig->getBaan());
    EXPECT_EQ(75, voertuig->getPositie());
}

// Tests for different vehicle types
TEST(VoertuigTest, VehicleTypes) {
    auto auto1 = Voertuig::maakVoertuig("Teststraat", 10, "auto");
    auto bus1 = Voertuig::maakVoertuig("Teststraat", 20, "bus");
    auto brandweer = Voertuig::maakVoertuig("Teststraat", 30, "brandweerwagen");

    EXPECT_EQ("auto", auto1->getType());
    EXPECT_EQ("bus", bus1->getType());
    EXPECT_EQ("brandweerwagen", brandweer->getType());

    EXPECT_FALSE(auto1->isPrioriteitsvoertuig());
    EXPECT_FALSE(bus1->isPrioriteitsvoertuig());
    EXPECT_TRUE(brandweer->isPrioriteitsvoertuig());

    EXPECT_FALSE(auto1->isBus());
    EXPECT_TRUE(bus1->isBus());
}

// Test priority vehicle functionality
TEST(VoertuigTest, PriorityVehicles) {
    auto brandweer = Voertuig::maakVoertuig("Teststraat", 10, "brandweerwagen");
    auto zieken = Voertuig::maakVoertuig("Teststraat", 20, "ziekenwagen");
    auto politie = Voertuig::maakVoertuig("Teststraat", 30, "politiecombi");

    EXPECT_TRUE(brandweer->isPrioriteitsvoertuig());
    EXPECT_TRUE(zieken->isPrioriteitsvoertuig());
    EXPECT_TRUE(politie->isPrioriteitsvoertuig());

    EXPECT_EQ("brandweerwagen", brandweer->getType());
    EXPECT_EQ("ziekenwagen", zieken->getType());
    EXPECT_EQ("politiecombi", politie->getType());
}

// Test vehicle position and speed updates
TEST(VoertuigTest, PositionAndSpeedUpdates) {
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    voertuig->setSnelheid(10.0);
    voertuig->setVersnelling(1.0);

    // Initial values
    EXPECT_EQ(50, voertuig->getPositie());
    EXPECT_EQ(10.0, voertuig->getSnelheid());
    EXPECT_EQ(1.0, voertuig->getVersnelling());

    // Update with 1 second timestep
    voertuig->updatePositieEnSnelheid(1.0);

    // Expected values after update:
    // position = 50 + 10*1 + 0.5*1*1 = 60.5
    // speed = 10 + 1*1 = 11
    EXPECT_NEAR(60.5, voertuig->getPositie(), 0.001);
    EXPECT_NEAR(11.0, voertuig->getSnelheid(), 0.001);
}

// Test vehicle braking behavior
TEST(VoertuigTest, BrakingBehavior) {
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    voertuig->setSnelheid(10.0);

    // Test emergency stop
    voertuig->noodStop();
    EXPECT_LT(voertuig->getVersnelling(), 0); // Should be negative acceleration

    // Test that speed never becomes negative
    voertuig->setVersnelling(-20.0); // Extreme braking
    voertuig->updatePositieEnSnelheid(1.0);
    EXPECT_GE(voertuig->getSnelheid(), 0.0);
}

// Test vehicle following behavior
TEST(VoertuigTest, FollowingBehavior) {
    auto leadVehicle = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    auto followingVehicle = Voertuig::maakVoertuig("Teststraat", 50, "auto");

    // Lead vehicle is slow
    leadVehicle->setSnelheid(5.0);
    // Following vehicle is faster
    followingVehicle->setSnelheid(15.0);

    // Calculate acceleration when following
    followingVehicle->berekenVersnelling(leadVehicle.get(), false);

    // Should be decelerating to avoid collision
    EXPECT_LT(followingVehicle->getVersnelling(), 0);
}

// Test vehicle cloning functionality
TEST(VoertuigTest, CloneFunctionality) {
    auto original = Voertuig::maakVoertuig("Teststraat", 50, 10.0, 1.0, "auto");

    auto clone = original->clone();

    // Check that the clone has the same properties
    EXPECT_EQ(original->getBaanNaam(), clone->getBaanNaam());
    EXPECT_EQ(original->getPositie(), clone->getPositie());
    EXPECT_EQ(original->getSnelheid(), clone->getSnelheid());
    EXPECT_EQ(original->getVersnelling(), clone->getVersnelling());
    EXPECT_EQ(original->getType(), clone->getType());
}

// Test different vehicle types have different properties
TEST(VoertuigTest, VehicleTypeProperties) {
    auto auto1 = Voertuig::maakVoertuig("Teststraat", 10, "auto");
    auto bus1 = Voertuig::maakVoertuig("Teststraat", 20, "bus");
    auto brandweer = Voertuig::maakVoertuig("Teststraat", 30, "brandweerwagen");
    auto zieken = Voertuig::maakVoertuig("Teststraat", 40, "ziekenwagen");
    auto politie = Voertuig::maakVoertuig("Teststraat", 50, "politiecombi");

    // Check they have different lengths
    EXPECT_EQ(4.0, auto1->getLengte());
    EXPECT_EQ(12.0, bus1->getLengte());
    EXPECT_EQ(10.0, brandweer->getLengte());
    EXPECT_EQ(8.0, zieken->getLengte());
    EXPECT_EQ(6.0, politie->getLengte());

    // Check they have different max speeds
    EXPECT_NE(auto1->getMaxSnelheid(), bus1->getMaxSnelheid());
    EXPECT_NE(brandweer->getMaxSnelheid(), zieken->getMaxSnelheid());
    EXPECT_NE(politie->getMaxSnelheid(), auto1->getMaxSnelheid());

    // Check bus stop functionality
    bus1->setIsWaitingAtBusStop(true);
    EXPECT_TRUE(bus1->isWaitingAtBusStop());
    bus1->setIsWaitingAtBusStop(false);
    EXPECT_FALSE(bus1->isWaitingAtBusStop());
}