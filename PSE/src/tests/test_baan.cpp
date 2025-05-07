/**
* @file test_baan.cpp
 * @brief Tests voor de Baan class
 */

#include <gtest/gtest.h>
#include "./test_helpers.h"
#include "../TraficObjects/baan.h"

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

// extra test om de initialisatie nog is na te kijken
TEST(BaanTest, ProperlyInitialized) {
    Baan baan("Teststraat", 250);
    EXPECT_TRUE(baan.properlyInitialized());

}
// Omdat er veel REQUIRES en ENSURES zijn, valt er niet veel te testen.
// Dit aangezien de initialisatie en de aanwezigheid van naam etc. al gecontroleerd worden.