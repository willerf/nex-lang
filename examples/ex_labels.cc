
#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "define_label.h"
#include "elim_labels.h"
#include "label.h"
#include "print.h"
#include "reg.h"
#include "use_label.h"
#include "word.h"

int main() {
    std::shared_ptr<Label> label = std::make_shared<Label>("mylabel");

    std::cout << label.get() << std::endl;
    std::cout << std::make_shared<DefineLabel>(label)->label.get() << std::endl;
    std::cout << std::make_shared<UseLabel>(label)->label.get() << std::endl;

    std::vector<std::shared_ptr<Code>> program = {
        std::make_shared<BeqLabel>(Reg::Result, Reg::Scratch, label),
        make_word(100),
        std::make_shared<BneLabel>(Reg::Result, Reg::Scratch, label),
        std::make_shared<UseLabel>(label),
        make_word(200),
        std::make_shared<DefineLabel>(label),
        make_word(300),
        make_word(400),
        std::make_shared<BeqLabel>(Reg::Result, Reg::Scratch, label),
        std::make_shared<UseLabel>(label),
        make_word(500),
        std::make_shared<BneLabel>(Reg::Result, Reg::Scratch, label)};

    Print print_v;

    std::make_shared<Block>(program)->accept(print_v);
    std::vector<std::shared_ptr<Code>> program2 = elim_labels(program);
    std::make_shared<Block>(program2)->accept(print_v);

    return 0;
}
