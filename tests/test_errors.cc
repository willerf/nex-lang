
#include <catch2/catch_test_macros.hpp>
#include <string>

#include "compile.h"
#include "duplicate_symbol_error.h"
#include "parsing_error.h"
#include "scanning_error.h"
#include "symbol_not_found_error.h"
#include "type_mismatch_error.h"
#include "utils.h"

TEST_CASE("Test scanning error 1.", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { invalid character: ^ }";
    REQUIRE_THROWS_AS(compile_test(input), ScanningError);
}

TEST_CASE("Test parsing error 1.", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { invalid stmt; }";
    REQUIRE_THROWS_AS(compile_test(input), ParsingError);
}

TEST_CASE("Test duplicate symbol error 1.", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { let x: char = 'b'; }";
    REQUIRE_THROWS_AS(compile_test(input), DuplicateSymbolError);
}

TEST_CASE("Test missing symbol error 1.", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { return z; }";
    REQUIRE_THROWS_AS(compile_test(input), SymbolNotFoundError);
}

TEST_CASE("Test missing symbol error 2.", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { print(x); }";
    REQUIRE_THROWS_AS(compile_test(input), SymbolNotFoundError);
}

TEST_CASE("Test missing symbol error 3.", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { return a::get(); }";
    REQUIRE_THROWS_AS(compile_test(input), SymbolNotFoundError);
}

TEST_CASE("Test type mismatch error 1.", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { let z: i32 = x + 'c'; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("Test type mismatch error 2.", "[errors]") {
    std::string input = R"(
        mod a;
        
        fn get(c: char) -> i32 {
            return 0;
        }

        fn main(x: i32, y: i32) -> i32 { 
            get(x);
        }
        )";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("Test type mismatch error 3.", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { return 'a'; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("Test type mismatch error 4.", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) { return 0; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("Test type mismatch error 5.", "[errors]") {
    const std::string tests_dir(NL_TESTS_PATH);
    std::vector<std::string> input_file_paths = {
        tests_dir + "/test_type_mismatch_error.nl",
        tests_dir + "/print_module.nl"};

    REQUIRE_THROWS_AS(compile(input_file_paths), TypeMismatchError);
}
