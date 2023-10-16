
#include "../../src/program_representation/code_structures/block.h"
#include "../../src/program_representation/code_structures/word.h"
#include "../../src/program_representation/assembly.h"
#include "../../src/program_representation/assembly.h"

#include "../../src/transformations/write_file.h"

using namespace std;

const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

int main() {

    vector<shared_ptr<Code>> program = {
        make_add(Reg::Result, Reg::Input1, Reg::Input2),
        make_lis(Reg::TargetPC),
        make_word(TERMINATION_PC),
        make_jr(Reg::TargetPC)
    };

    write_file("test_write_file.bin", program);

    return 0;
}
