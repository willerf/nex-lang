
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <iostream>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_while_loop.bin";

TEST_CASE("Test plus.", "[loops]") {
    std::string input =
        "fn main(x: i32, y: i32) -> i32 {"
        "    let z: i32 = 0;"
        "    while (z < x) {"
        "        z = z + 1;"
        "    }"
        "    return z;"
        "}";

    auto program = compile(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 10, 0)) == 10);
}

