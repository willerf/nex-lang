
#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::vector<uint32_t> word_to_uint(std::vector<std::shared_ptr<Code>> program) {
    std::vector<uint32_t> result;
    for (auto code : program) {
        if (auto word = std::dynamic_pointer_cast<Word>(code)) {
            result.push_back(word->bits);
        }
    }
    return result;
}

std::string emulate(std::string file_name, int32_t input1, int32_t input2) {
    const std::string emulator_path(EMULATOR_PATH);
    const std::string output_file("test_output.txt");

    std::system((emulator_path + " " + file_name + " " + std::to_string(input1)
                 + " " + std::to_string(input2) + " > " + output_file)
                    .c_str());

    std::ifstream file(output_file);
    if (!file) {
        std::cerr << "Output file not found!" << std::endl;
        exit(1);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
