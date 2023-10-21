
#include "write_file.h"

#include <fstream>
#include <iostream>

#include "word.h"

void write_file(
    std::string file_name,
    std::vector<std::shared_ptr<Code>>& program
) {
    std::ofstream out {file_name, std::ios::binary};
    if (!out) {
        throw "Error opening file for writing.";
    }
    for (auto& code : program) {
        std::shared_ptr<Word> word = std::dynamic_pointer_cast<Word>(code);
        if (word) {
            uint8_t reversed[4];
            uint8_t* data = reinterpret_cast<uint8_t*>(&word->bits);
            reversed[0] = data[3];
            reversed[1] = data[2];
            reversed[2] = data[1];
            reversed[3] = data[0];
            out.write(reinterpret_cast<char*>(&reversed), sizeof(word->bits));
        } else {
            auto& c = *code.get();
            std::cerr << "Invalid code structure while writing: "
                      << typeid(c).name() << std::endl;
        }
    }
    out.close();
}
