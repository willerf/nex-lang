
#include "../../src/program_representation/code_structures/block.h"
#include "../../src/program_representation/code_structures/word.h"
#include "../../src/program_representation/code_structures/beq_label.h"
#include "../../src/program_representation/code_structures/bne_label.h"
#include "../../src/program_representation/code_structures/define_label.h"
#include "../../src/program_representation/code_structures/use_label.h"
#include "../../src/program_representation/label.h"
#include "../../src/program_representation/assembly.h"

#include "../../src/transformations/print.h"
#include "../../src/transformations/elim_labels.h"
#include "../../src/transformations/write_file.h"
#include "../../src/utils/reg.h"

using namespace std;


const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

void run() {

    shared_ptr<Label> equal_label = make_shared<Label>("equal_label");
    shared_ptr<Label> end_label = make_shared<Label>("end_label");

    vector<shared_ptr<Code>> program = {
        make_beq(Reg::Input1, Reg::Input2, equal_label),
        make_lis(Reg::Result),
        make_word(3),
        make_beq(Reg::Zero, Reg::Zero, end_label),
        make_define(equal_label),
        make_lis(Reg::Result),
        make_word(5),
        make_define(end_label),
        make_lis(Reg::TargetPC),
        make_word(TERMINATION_PC),
        make_jr(Reg::TargetPC)
    }; 

    Print print_v;

    make_shared<Block>(program)->accept(print_v);
    std::vector<std::shared_ptr<Code>> program2 = elim_labels(program);
    make_shared<Block>(program2)->accept(print_v);

    write_file("test_write_file.bin", program2);
}

int main() {
    run();
    return 0;
}


