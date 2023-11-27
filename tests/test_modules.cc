

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_modules.bin";

TEST_CASE("max", "[math]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_max.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == std::max(3, 5));
    REQUIRE(stoi(emulate(file_name, 5, 5)) == std::max(5, 5));
    REQUIRE(stoi(emulate(file_name, 8, 5)) == std::max(8, 5));
    REQUIRE(stoi(emulate(file_name, -3, -5)) == std::max(-3, -5));
    REQUIRE(stoi(emulate(file_name, -5, -5)) == std::max(-5, -5));
    REQUIRE(stoi(emulate(file_name, -8, -5)) == std::max(-8, -5));
}

TEST_CASE("min", "[math]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_min.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == std::min(3, 5));
    REQUIRE(stoi(emulate(file_name, 5, 5)) == std::min(5, 5));
    REQUIRE(stoi(emulate(file_name, 8, 5)) == std::min(8, 5));
    REQUIRE(stoi(emulate(file_name, -3, -5)) == std::min(-3, -5));
    REQUIRE(stoi(emulate(file_name, -5, -5)) == std::min(-5, -5));
    REQUIRE(stoi(emulate(file_name, -8, -5)) == std::min(-8, -5));
}

TEST_CASE("print char", "[modules]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_print_char.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello\n0\n");
}

TEST_CASE("print", "[modules]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_print.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello World!\nHello World Again!\n0\n");
}

TEST_CASE("println", "[modules]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_println.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello World!\nHello World Again!\n0\n");
}

TEST_CASE("print i32", "[modules]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_print_num.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 17, 0) == "17\n0\n");
    REQUIRE(emulate(file_name, 1927850, 0) == "1927850\n0\n");
    REQUIRE(emulate(file_name, 1000, 0) == "1000\n0\n");
    REQUIRE(emulate(file_name, 0, 0) == "0\n0\n");
    REQUIRE(emulate(file_name, -17, 0) == "-17\n0\n");
    REQUIRE(emulate(file_name, -1927850, 0) == "-1927850\n0\n");
    REQUIRE(emulate(file_name, -1000, 0) == "-1000\n0\n");
}

TEST_CASE("fibonacci module", "[modules]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_fibonacci.nl",
        examples_dir + "/fibonacci_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 5, 0) == "0 1 1 2 3 \n0\n");
    REQUIRE(
        emulate(file_name, 17, 0)
        == "0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 \n0\n"
    );
}
