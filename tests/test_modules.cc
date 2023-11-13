

#include <catch2/catch_test_macros.hpp>
#include <string>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_modules.bin";

TEST_CASE("Test max.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_max.nl",
        tests_dir + "/math_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == std::max(3, 5));
    REQUIRE(stoi(emulate(file_name, 5, 5)) == std::max(5, 5));
    REQUIRE(stoi(emulate(file_name, 8, 5)) == std::max(8, 5));
    REQUIRE(stoi(emulate(file_name, -3, -5)) == std::max(-3, -5));
    REQUIRE(stoi(emulate(file_name, -5, -5)) == std::max(-5, -5));
    REQUIRE(stoi(emulate(file_name, -8, -5)) == std::max(-8, -5));
}

TEST_CASE("Test min.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_min.nl",
        tests_dir + "/math_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == std::min(3, 5));
    REQUIRE(stoi(emulate(file_name, 5, 5)) == std::min(5, 5));
    REQUIRE(stoi(emulate(file_name, 8, 5)) == std::min(8, 5));
    REQUIRE(stoi(emulate(file_name, -3, -5)) == std::min(-3, -5));
    REQUIRE(stoi(emulate(file_name, -5, -5)) == std::min(-5, -5));
    REQUIRE(stoi(emulate(file_name, -8, -5)) == std::min(-8, -5));
}

TEST_CASE("Test printchar.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_printchar.nl",
        tests_dir + "/print_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello\n0\n");
}

TEST_CASE("Test print.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_print.nl",
        tests_dir + "/print_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello World!\nHello World Again!\n0\n");
}

TEST_CASE("Test println.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_println.nl",
        tests_dir + "/print_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    std::string result = emulate(file_name, 0, 0);
    REQUIRE(result == "Hello World!\nHello World Again!\n0\n");
}

TEST_CASE("Test printnum.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_printnum.nl",
        tests_dir + "/print_module.nl"};
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

TEST_CASE("Test fibonacci.", "[modules]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_fibonacci.nl",
        tests_dir + "/print_module.nl",
        tests_dir + "/fibonacci_module.nl"};
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 5, 0) == "0 1 1 2 3 \n0\n");
    REQUIRE(
        emulate(file_name, 17, 0)
        == "0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 \n0\n"
    );
}
