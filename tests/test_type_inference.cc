

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_type_inference.bin";

TEST_CASE("type inference", "[type_inference]") {
    std::vector<std::string> input_file_paths = {
        examples_dir + "/test_type_inference.nl",
    };
    auto program = compile(input_file_paths);
    write_file(file_name, program);

    REQUIRE(emulate(file_name, 0, 0) == "fhello world\nok0\n");
}
