#include <catch2/catch_test_macros.hpp>
#include <string>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

class ListModuleFixture {
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
    ListModuleFixture() {
        initialize();
    }

    std::string test_list_function(int test_code, int test_value) {
        return emulate(file_name, test_code, test_value);
    }
};

std::string ListModuleFixture::file_name = "test_list_module.bin";
std::vector<std::string> ListModuleFixture::input_file_paths = {
    examples_dir + "/test_list_module.nl"};
bool ListModuleFixture::initialized = false;

TEST_CASE_METHOD(ListModuleFixture, "list operations", "[list]") {
    REQUIRE(test_list_function(1, 0) == "[]\n[1, 2, 3]\n0\n");
    REQUIRE(test_list_function(2, 0) == "[1, 2]\n2\n[1]\n1\n[]\n0\n");
    REQUIRE(test_list_function(3, 0) == "[4, 5, 6]\n4\n6\n0\n");
    REQUIRE(
        test_list_function(4, 0)
        == "[7, 8, 5, 12]\n7\n8\n5\n12\n[7, 8, 17, 12]\n[25, 8, 17, 12]\n0\n"
    );
    REQUIRE(
        test_list_function(5, 0) == "[5, 7, 9]\n[8, 7, 9]\n[8, 7, 12]\n0\n"
    );
    REQUIRE(
        test_list_function(6, 0)
        == "true\n0\nfalse\n1\nfalse\n2\nfalse\n3\nfalse\n2\nfalse\n1\ntrue\n0\n0\n"
    );
    REQUIRE(test_list_function(7, 0) == "1\n1\n2\n4\n4\n8\n5\n0\n");
    REQUIRE(
        test_list_function(8, 0)
        == "1\n[]\n1\n[3]\n2\n[3, 5]\n4\n[3, 5, 4]\n9\n[3, 5, 4, 0, 0, 0, 0, 0, 0]\n1\n[3]\n0\n"
    );
}
