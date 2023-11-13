
#include <stdlib.h>

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    auto program = compile(input_file_paths);
    write_file(output_file_path, program);

    return 0;
}
