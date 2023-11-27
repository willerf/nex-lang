#include <catch2/catch_test_macros.hpp>
#include <string>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

class PrintModuleFixture {
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
    PrintModuleFixture() {
        initialize();
    }

    std::string test_print_function(int test_code, int test_value) {
        return emulate(file_name, test_code, test_value);
    }
};

std::string PrintModuleFixture::file_name = "test_print_module.bin";
std::vector<std::string> PrintModuleFixture::input_file_paths = {
    examples_dir + "/test_print_module.nl"};
bool PrintModuleFixture::initialized = false;

TEST_CASE_METHOD(PrintModuleFixture, "print char", "[print]") {
    REQUIRE(test_print_function(1, static_cast<int>('a')) == "a0\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "println empty", "[print]") {
    REQUIRE(test_print_function(2, 0) == "\n0\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "println char", "[print]") {
    REQUIRE(test_print_function(3, static_cast<int>('b')) == "b\n0\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "print number", "[print]") {
    REQUIRE(test_print_function(4, 123) == "1230\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "println number", "[print]") {
    REQUIRE(test_print_function(5, -456) == "-456\n0\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "print array", "[print]") {
    REQUIRE(test_print_function(6, 3) == "1 2 3 0\n");
}

TEST_CASE_METHOD(PrintModuleFixture, "println array", "[print]") {
    REQUIRE(test_print_function(7, 2) == "1 2 \n0\n");
}
