
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

#include "compile.h"
#include "write_file.h"

int main(int argc, char* argv[]) {
    
    std::string input_file_path;
    std::string output_file_path = "a.out";
    size_t i = 1;
    while (i < argc) {
        if (std::strcmp(argv[i], "-o") == 0) {
            assert(i + 1 < argc);
            output_file_path = argv[i+1];
            i += 2;
        }
        else {
            input_file_path = argv[i];
            i += 1;
        }
    }
    std::ifstream file {input_file_path};
    if (!file) {
        std::cerr << "Invalid file path: " << input_file_path << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    auto program = compile(input);
    write_file(output_file_path, program);
}
