
#include <stdlib.h>

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "compile.h"
#include "compile_error.h"
#include "write_file.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> input_file_paths;
    std::string output_file_path = "a.out";
    size_t i = 1;
    while (i < argc) {
        if (std::strcmp(argv[i], "-o") == 0) {
            assert(i + 1 < argc);
            output_file_path = argv[i + 1];
            i += 2;
        } else {
            input_file_paths.push_back(argv[i]);
            i += 1;
        }
    }

    try {
        auto program = compile(input_file_paths);
        write_file(output_file_path, program);
    } catch (CompileError& compile_error) {
        std::cerr << compile_error.what() << std::endl;

        std::ifstream file {compile_error.input_file_path};
        if (!file) {
            std::cerr << "Invalid file path: " << compile_error.input_file_path
                      << std::endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();

        size_t line_error = compile_error.get_line_no();
        size_t line_no = 1;
        std::string line;
        while (getline(buffer, line)) {
            if (line_no == line_error) {
                line.erase(0, line.find_first_not_of(" \n\r\t"));
                std::cerr << "Line " << line_error << ": " << line << std::endl;
            }
            ++line_no;
        }
    }

    return 0;
}
