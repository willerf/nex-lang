#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_modules.bin";

// Function to run and test each math function
std::string testMathFunction(int functionCode, int y) {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_math_module.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    return emulate(file_name, functionCode, y);
}

// Test for Absolute Value Function
TEST_CASE("Test abs.", "[math]") {
    REQUIRE(stoi(testMathFunction(1, -5)) == 5);
    REQUIRE(stoi(testMathFunction(1, 5)) == 5);
    REQUIRE(stoi(testMathFunction(1, 0)) == 0);
}

// Test for Power Function
TEST_CASE("Test pow.", "[math]") {
    REQUIRE(stoi(testMathFunction(2, 3)) == 9);  // 3^2
    REQUIRE(stoi(testMathFunction(2, 5)) == 25); // 5^2
}

// Test for Factorial Function
TEST_CASE("Test factorial.", "[math]") {
    REQUIRE(stoi(testMathFunction(3, 5)) == 120);  // 5!
    REQUIRE(stoi(testMathFunction(3, 0)) == 1);    // 0!
}

// Test for GCD Function
TEST_CASE("Test gcd.", "[math]") {
    REQUIRE(stoi(testMathFunction(4, 12)) == 2);   // gcd(12, 10)
    REQUIRE(stoi(testMathFunction(4, 14)) == 2);   // gcd(14, 10)
}

// Test for LCM Function
TEST_CASE("Test lcm.", "[math]") {
    REQUIRE(stoi(testMathFunction(5, 12)) == 60);  // lcm(12, 10)
    REQUIRE(stoi(testMathFunction(5, 14)) == 70);  // lcm(14, 10)
}

// Test for Square Root Function
TEST_CASE("Test sqrt.", "[math]") {
    REQUIRE(stoi(testMathFunction(6, 9)) == 3);    // sqrt(9)
    REQUIRE(stoi(testMathFunction(6, 16)) == 4);   // sqrt(16)
}

// Test for Is Prime Function
TEST_CASE("Test is_prime.", "[math]") {
    REQUIRE(testMathFunction(7, 5) == "true\n0\n");
    REQUIRE(testMathFunction(7, 4) == "false\n0\n");
}

// Test for Fibonacci Function
TEST_CASE("Test fibonacci.", "[math]") {
    REQUIRE(stoi(testMathFunction(8, 5)) == 5);    // fibonacci(5)
    REQUIRE(stoi(testMathFunction(8, 10)) == 55);  // fibonacci(10)
}

