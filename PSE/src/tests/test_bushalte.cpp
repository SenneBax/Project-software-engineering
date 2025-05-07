/**
 * @file test_bushalte.cpp
 * @brief Tests for the Bushalte class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../TraficObjects/bushalte.h"

// Test constructor and getters
TEST(BushalteTest, ConstructorAndGetters) {
    Bushalte bushalte("Teststraat", 200, 15);
    EXPECT_EQ("Teststraat", bushalte.getBaan());
    EXPECT_EQ(200, bushalte.getPositie());
    EXPECT_EQ(15, bushalte.getWachttijd());
    EXPECT_FALSE(bushalte.isBusGestopt());
}

// Test bus stop and leave functionality
TEST(BushalteTest, BusStopAndLeave) {
    Bushalte bushalte("Teststraat", 200, 5);

    // Initially no bus is stopped
    EXPECT_FALSE(bushalte.isBusGestopt());

    // Mark that a bus has stopped
    bushalte.setBusStopped();
    EXPECT_TRUE(bushalte.isBusGestopt());

    // Update wait time (not enough to leave yet)
    bool timeUp = bushalte.updateWachttijd(2.0);
    EXPECT_FALSE(timeUp);
    EXPECT_TRUE(bushalte.isBusGestopt());

    // Update wait time (now the full wait time has elapsed)
    timeUp = bushalte.updateWachttijd(3.0);
    EXPECT_TRUE(timeUp);
    EXPECT_TRUE(bushalte.isBusGestopt()); // Still stopped until setBusLeft is called

    // Mark that the bus has left
    bushalte.setBusLeft();
    EXPECT_FALSE(bushalte.isBusGestopt());
}

// Test reset wait time functionality
TEST(BushalteTest, ResetWaitTime) {
    Bushalte bushalte("Teststraat", 200, 10);

    // Mark that a bus has stopped
    bushalte.setBusStopped();
    EXPECT_TRUE(bushalte.isBusGestopt());

    // Update wait time partially
    bool timeUp = bushalte.updateWachttijd(5.0);
    EXPECT_FALSE(timeUp);

    // Reset the wait time
    bushalte.resetWachttijd();

    // Even a full wait time update should not trigger time up now
    timeUp = bushalte.updateWachttijd(5.0);
    EXPECT_FALSE(timeUp);

    // But the full wait time should trigger it
    timeUp = bushalte.updateWachttijd(5.0);
    EXPECT_TRUE(timeUp);
}

// Test bus stop with multiple updates
TEST(BushalteTest, MultipleUpdates) {
    Bushalte bushalte("Teststraat", 200, 10);

    bushalte.setBusStopped();

    // Multiple small updates
    for (int i = 0; i < 9; i++) {
        bool timeUp = bushalte.updateWachttijd(1.0);
        EXPECT_FALSE(timeUp);
    }

    // Last update should trigger time up
    bool timeUp = bushalte.updateWachttijd(1.0);
    EXPECT_TRUE(timeUp);
}

// Test that update wait time only works when bus is stopped
TEST(BushalteTest, UpdateOnlyWhenStopped) {
    Bushalte bushalte("Teststraat", 200, 5);

    // Bus is not stopped, update should have no effect
    bool timeUp = bushalte.updateWachttijd(10.0);
    EXPECT_FALSE(timeUp);

    // Now stop a bus and update
    bushalte.setBusStopped();
    timeUp = bushalte.updateWachttijd(5.0);
    EXPECT_TRUE(timeUp);
}