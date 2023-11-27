
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "compile_error.h"
#include "duplicate_symbol_error.h"
#include "parsing_error.h"
#include "scanning_error.h"
#include "symbol_not_found_error.h"
#include "type_mismatch_error.h"
#include "utils.h"

TEST_CASE("scanning error 1", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { invalid character: ^ }";
    REQUIRE_THROWS_AS(compile_test(input), ScanningError);
}

TEST_CASE("parsing error 1", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { invalid stmt; }";
    REQUIRE_THROWS_AS(compile_test(input), ParsingError);
}

TEST_CASE("duplicate symbol error 1", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { let x: char = 'b'; }";
    REQUIRE_THROWS_AS(compile_test(input), DuplicateSymbolError);
}

TEST_CASE("missing symbol error 1", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { return z; }";
    REQUIRE_THROWS_AS(compile_test(input), SymbolNotFoundError);
}

TEST_CASE("missing symbol error 2", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { print(x); }";
    REQUIRE_THROWS_AS(compile_test(input), CompileError);
}

TEST_CASE("missing symbol error 3", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { return get(); }";
    REQUIRE_THROWS_AS(compile_test(input), CompileError);
}

TEST_CASE("type mismatch error 1", "[errors]") {
    std::string input =
        "mod a; fn main(x: i32, y: i32) -> i32 { let z: i32 = x + 'c'; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("type mismatch error 2", "[errors]") {
    std::string input = R"(
        mod a;
        
        fn get(c: char) -> i32 {
            return 0;
        }

        fn main(x: i32, y: i32) -> i32 { 
            get(x);
        }
        )";
    REQUIRE_THROWS_AS(compile_test(input), CompileError);
}

TEST_CASE("type mismatch error 3", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) -> i32 { return 'a'; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("type mismatch error 4", "[errors]") {
    std::string input = "mod a; fn main(x: i32, y: i32) { return 0; }";
    REQUIRE_THROWS_AS(compile_test(input), TypeMismatchError);
}

TEST_CASE("type mismatch error 5", "[errors]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_type_mismatch_error.nl",
    };

    REQUIRE_THROWS_AS(compile(input_file_paths), CompileError);
}
