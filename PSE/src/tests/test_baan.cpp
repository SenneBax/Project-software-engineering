/**
* @file test_baan.cpp
 * @brief Tests for the Baan class
 */

#include <gtest/gtest.h>
#include "./test_helpers.h"
#include "../TraficObjects/baan.h"

// Tests for the Baan class
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

// Additional test to verify proper initialization
TEST(BaanTest, ProperlyInitialized) {
    Baan baan("Teststraat", 250);
    EXPECT_TRUE(baan.properlyInitialized());

    // Test that empty name initialization fails REQUIRE in debug mode
    // This would cause REQUIRE to fail, but we can't test assertions directly in GTest
    // So we're just testing the positive case
}