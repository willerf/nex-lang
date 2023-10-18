
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
#include "../../src/utils/reg.h"

using namespace std;

void run() {

    shared_ptr<Label> label = make_shared<Label>("mylabel");
    
    cout << label.get() << endl;
    cout << make_shared<DefineLabel>(label)->label.get() << endl;
    cout << make_shared<UseLabel>(label)->label.get() << endl;

    vector<shared_ptr<Code>> program = {
        make_shared<BeqLabel>(Reg::Result, Reg::Scratch, label),
        make_word(100),
        make_shared<BneLabel>(Reg::Result, Reg::Scratch, label),
        make_shared<UseLabel>(label),
        make_word(200),
        make_shared<DefineLabel>(label),
        make_word(300),
        make_word(400),
        make_shared<BeqLabel>(Reg::Result, Reg::Scratch, label),
        make_shared<UseLabel>(label),
        make_word(500),
        make_shared<BneLabel>(Reg::Result, Reg::Scratch, label)
    };

    Print print_v;

    make_shared<Block>(program)->accept(print_v);
    std::vector<std::shared_ptr<Code>> program2 = elim_labels(program);
    make_shared<Block>(program2)->accept(print_v);

}

int main() {
    try {
        run();
    }
    catch (char const* error_msg) {
        std::cerr << error_msg << std::endl;
    }

    return 0;
}

