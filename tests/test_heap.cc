
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_heap.bin";

TEST_CASE("simple heap", "[heap]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_heap.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 5, 5) == "1\n1\n1\n0\n");
    REQUIRE(emulate(file_name, 5, 1) == "1\n1\n1\n0\n");
    REQUIRE(emulate(file_name, 5, 6) == "1\n1\n0\n0\n");
    REQUIRE(emulate(file_name, 5, 10) == "1\n1\n0\n0\n");
}

TEST_CASE("array", "[heap]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_arr.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 0, 0) == "0 1 1 2 3 5 8 13 21 34 \n0\n");
}
