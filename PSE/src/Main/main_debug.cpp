/**
* @file main_test.cpp
 * @brief Main test runner file that includes all test modules
 */

#include <gtest/gtest.h>


int main(int argc, char **argv) {
    std::cout << "Starting traffic simulation tests..." << std::endl;

    // Initializeren van google test framework
    ::testing::InitGoogleTest(&argc, argv);

    // Run alle testen
    int result = RUN_ALL_TESTS();

    std::cout << "Tests completed with result: " << result << std::endl;

    return result;
}