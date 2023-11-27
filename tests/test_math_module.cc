#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

class MathModuleFixture {
  private:
    static std::string file_name;
    static std::vector<std::string> input_file_paths;
    static bool initialized;

    static void initialize() {
        if (!initialized) {
            auto program = compile(input_file_paths);
            write_file(file_name, program);
            initialized = true;
        }
    }

  public:
    MathModuleFixture() {
        initialize();
    }

    std::string test_math_function(int function_code, int y) {
        return emulate(file_name, function_code, y);
    }
};

std::string MathModuleFixture::file_name = "test_math_module.bin";
std::vector<std::string> MathModuleFixture::input_file_paths = {
    examples_dir + "/test_math_module.nl"};
bool MathModuleFixture::initialized = false;

TEST_CASE_METHOD(MathModuleFixture, "abs", "[math]") {
    REQUIRE(stoi(test_math_function(1, -5)) == 5);
    REQUIRE(stoi(test_math_function(1, 5)) == 5);
    REQUIRE(stoi(test_math_function(1, 0)) == 0);
}

TEST_CASE_METHOD(MathModuleFixture, "pow", "[math]") {
    REQUIRE(stoi(test_math_function(2, 3)) == 9);
    REQUIRE(stoi(test_math_function(2, 5)) == 25);
}

TEST_CASE_METHOD(MathModuleFixture, "factorial", "[math]") {
    REQUIRE(stoi(test_math_function(3, 5)) == 120);
    REQUIRE(stoi(test_math_function(3, 0)) == 1);
}

TEST_CASE_METHOD(MathModuleFixture, "gcd", "[math]") {
    REQUIRE(stoi(test_math_function(4, 12)) == 2);
    REQUIRE(stoi(test_math_function(4, 14)) == 2);
}

TEST_CASE_METHOD(MathModuleFixture, "lcm", "[math]") {
    REQUIRE(stoi(test_math_function(5, 12)) == 60);
    REQUIRE(stoi(test_math_function(5, 14)) == 70);
}

TEST_CASE_METHOD(MathModuleFixture, "sqrt", "[math]") {
    REQUIRE(stoi(test_math_function(6, 9)) == 3);
    REQUIRE(stoi(test_math_function(6, 16)) == 4);
}

TEST_CASE_METHOD(MathModuleFixture, "is_prime", "[math]") {
    REQUIRE(test_math_function(7, 5) == "true\n0\n");
    REQUIRE(test_math_function(7, 4) == "false\n0\n");
}

TEST_CASE_METHOD(MathModuleFixture, "fibonacci", "[math]") {
    REQUIRE(stoi(test_math_function(8, 5)) == 5);
    REQUIRE(stoi(test_math_function(8, 10)) == 55);
}
