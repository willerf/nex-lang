
#include <catch2/catch_test_macros.hpp>
#include <string>

#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_operators.bin";

TEST_CASE("plus", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return x + y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == 3 + 5);
    REQUIRE(stoi(emulate(file_name, 15, 6)) == 15 + 6);
    REQUIRE(stoi(emulate(file_name, 0, -8)) == 0 + -8);
    REQUIRE(stoi(emulate(file_name, -4, 3)) == -4 + 3);
    REQUIRE(stoi(emulate(file_name, -21, -6)) == -21 + -6);
}

TEST_CASE("minus", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return x - y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == 3 - 5);
    REQUIRE(stoi(emulate(file_name, 15, 6)) == 15 - 6);
    REQUIRE(stoi(emulate(file_name, 0, -8)) == 0 - -8);
    REQUIRE(stoi(emulate(file_name, -4, 3)) == -4 - 3);
    REQUIRE(stoi(emulate(file_name, -21, -6)) == -21 - -6);
}

TEST_CASE("times", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return x * y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == 3 * 5);
    REQUIRE(stoi(emulate(file_name, 15, 6)) == 15 * 6);
    REQUIRE(stoi(emulate(file_name, 0, -8)) == 0 * -8);
    REQUIRE(stoi(emulate(file_name, -4, 3)) == -4 * 3);
    REQUIRE(stoi(emulate(file_name, -21, -6)) == -21 * -6);
}

TEST_CASE("divide", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return x / y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == 3 / 5);
    REQUIRE(stoi(emulate(file_name, 15, 6)) == 15 / 6);
    REQUIRE(stoi(emulate(file_name, 0, -8)) == 0 / -8);
    REQUIRE(stoi(emulate(file_name, -4, 3)) == -4 / 3);
    REQUIRE(stoi(emulate(file_name, -21, -6)) == -21 / -6);
}

TEST_CASE("mod", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return x % y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 3, 5)) == 3 % 5);
    REQUIRE(stoi(emulate(file_name, 15, 6)) == 15 % 6);
    REQUIRE(stoi(emulate(file_name, 0, -8)) == 0 % -8);
    REQUIRE(stoi(emulate(file_name, -4, 3)) == -4 % 3);
    REQUIRE(stoi(emulate(file_name, -21, -6)) == -21 % -6);
}

TEST_CASE("equal", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x == y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 == 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 == 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 == 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 == 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 == -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 == 3));
}

TEST_CASE("not equal", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x != y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 != 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 != 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 != 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 != 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 != -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 != 3));
}

TEST_CASE("greater than", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x > y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 > 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 > 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 > 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 > 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 > -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 > 3));
}

TEST_CASE("greater than or equal to", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x >= y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 >= 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 >= 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 >= 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 >= 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 >= -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 >= 3));
}

TEST_CASE("less than", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x < y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 < 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 < 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 < 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 < 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 < -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 < 3));
}

TEST_CASE("less than or equal to", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: i32, y: i32) -> bool {"
        "    return x <= y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 4, 5)) == (4 <= 5));
    REQUIRE(stoi(emulate(file_name, 7, 7)) == (7 <= 7));
    REQUIRE(stoi(emulate(file_name, 9, 6)) == (9 <= 6));
    REQUIRE(stoi(emulate(file_name, -4, 4)) == (-4 <= 4));
    REQUIRE(stoi(emulate(file_name, -15, -15)) == (-15 <= -15));
    REQUIRE(stoi(emulate(file_name, -12, 3)) == (-12 <= 3));
}

TEST_CASE("and", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: bool, y: bool) -> bool {"
        "    return x && y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 0, 0)) == (0 && 0));
    REQUIRE(stoi(emulate(file_name, 0, 1)) == (0 && 1));
    REQUIRE(stoi(emulate(file_name, 1, 0)) == (1 && 0));
    REQUIRE(stoi(emulate(file_name, 1, 1)) == (1 && 1));
    REQUIRE(stoi(emulate(file_name, 15, 0)) == (15 && 0));
    REQUIRE(stoi(emulate(file_name, 0, 27)) == (0 && 27));
    REQUIRE(stoi(emulate(file_name, -29, 12)) == (-29 && 12));
}

TEST_CASE("or", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: bool, y: bool) -> bool {"
        "    return x || y;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 0, 0)) == (0 || 0));
    REQUIRE(stoi(emulate(file_name, 0, 1)) == (0 || 1));
    REQUIRE(stoi(emulate(file_name, 1, 0)) == (1 || 0));
    REQUIRE(stoi(emulate(file_name, 1, 1)) == (1 || 1));
    REQUIRE(stoi(emulate(file_name, 15, 0)) == (15 || 0));
    REQUIRE(stoi(emulate(file_name, 0, 27)) == (0 || 27));
    REQUIRE(stoi(emulate(file_name, -29, 12)) == (-29 || 12));
}

TEST_CASE("not", "[operators]") {
    std::string input =
        "mod main;"
        "fn main(x: bool, y: bool) -> bool {"
        "    return !x;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 0, 0)) == (!0));
    REQUIRE(stoi(emulate(file_name, 1, 0)) == (!1));
    REQUIRE(stoi(emulate(file_name, -5, 0)) == (!-5));
    REQUIRE(stoi(emulate(file_name, 25, 0)) == (!25));
}

TEST_CASE("ampersand and star", "[operators]") {
    std::string input =
        "mod main;"
        "fn test_func(x: *i32, y: i32) -> i32 {"
        "    return *x;"
        "}"
        "fn main(x: i32, y: i32) -> i32 {"
        "    return test_func(&x, y);"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 15, 0)) == 15);
    REQUIRE(stoi(emulate(file_name, 6341, 0)) == 6341);
}
