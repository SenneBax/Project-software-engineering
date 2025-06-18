/**
* @file main_test.cpp
 * @brief Main test runner file that includes all test modules
 */

#include <gtest/gtest.h>

// // This file serves as the entry point for all tests
// // The actual tests are in their respective test_*.cpp files
//
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }




int main(int argc, char **argv) {
    std::cout << "Starting traffic simulation tests..." << std::endl;

    // Initialize Google Test framework
    ::testing::InitGoogleTest(&argc, argv);

    // Run all tests
    int result = RUN_ALL_TESTS();

    std::cout << "Tests completed with result: " << result << std::endl;

    return result;
}