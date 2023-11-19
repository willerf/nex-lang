
#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "define_label.h"
#include "elim_labels.h"
#include "label.h"
#include "print.h"
#include "reg.h"
#include "word.h"
#include "write_file.h"

struct Code;

const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

int main() {
    std::shared_ptr<Label> equal_label = std::make_shared<Label>("equal_label");
    std::shared_ptr<Label> end_label = std::make_shared<Label>("end_label");

    std::vector<std::shared_ptr<Code>> program = {
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
        make_jr(Reg::TargetPC)};

    Print print_v;

    std::make_shared<Block>(program)->accept(print_v);
    std::vector<std::shared_ptr<Code>> program2 = elim_labels(program);
    std::make_shared<Block>(program2)->accept(print_v);

    write_file("test_write_file.bin", program2);
    return 0;
}
