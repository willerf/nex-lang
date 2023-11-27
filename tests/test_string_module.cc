#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

class StringModuleFixture {
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
    StringModuleFixture() {
        initialize();
    }

    std::string test_string_function(int test_code, int test_value) {
        return emulate(file_name, test_code, test_value);
    }
};

std::string StringModuleFixture::file_name = "test_string_module.bin";
std::vector<std::string> StringModuleFixture::input_file_paths = {
    examples_dir + "/test_string_module.nl"};
bool StringModuleFixture::initialized = false;

TEST_CASE_METHOD(StringModuleFixture, "string_operations", "[string]") {
    REQUIRE(test_string_function(1, 0) == "Hello\nHelloWor\n0\n");
    REQUIRE(test_string_function(2, 0) == "HelloWo\no\nHelloW\nW\nHello\n0\n");
    REQUIRE(test_string_function(3, 0) == "HelloWor\nH\nr\n0\n");
    REQUIRE(
        test_string_function(4, 0)
        == "HelloWorl\nH\ne\nl\nW\nHemloWorl\nHemloWopl\n0\n"
    );
    REQUIRE(test_string_function(5, 0) == "HelloWor\nkelloWor\nkefloWor\n0\n");
    REQUIRE(
        test_string_function(6, 0)
        == "true\n0\nfalse\n1\nfalse\n2\nfalse\n3\nfalse\n2\nfalse\n1\ntrue\n0\n0\n"
    );
    REQUIRE(test_string_function(7, 0) == "HelloWorld\nHelloWorld\n0\n");
    REQUIRE(test_string_function(8, 0) == "8\n1\n1\n2\n4\n4\n8\n5\n0\n");
    REQUIRE(test_string_function(9, 0) == "Hello\nHel\nHelaaaaa\n0\n");
}
