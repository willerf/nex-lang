
#include "../../src/program_representation/code_structures/block.h"
#include "../../src/program_representation/code_structures/word.h"
#include "../../src/program_representation/code_structures/beq_label.h"
#include "../../src/program_representation/code_structures/bne_label.h"
#include "../../src/program_representation/code_structures/define_label.h"
#include "../../src/program_representation/code_structures/use_label.h"
#include "../../src/program_representation/code_structures/var_access.h"

#include "../../src/program_representation/label.h"
#include "../../src/program_representation/variable.h"
#include "../../src/program_representation/assembly.h"

#include "../../src/transformations/print.h"
#include "../../src/transformations/elim_labels.h"
#include "../../src/transformations/elim_vars.h"
#include "../../src/transformations/flatten.h"
#include "../../src/transformations/write_file.h"

#include "../../src/memory_management/chunk.h"
#include "../../src/memory_management/stack.h"

#include "../../src/utils/reg.h"

using namespace std;

const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

void run() {

    std::shared_ptr<Variable> var1 = std::make_shared<Variable>("test variable1");
    std::shared_ptr<Variable> var2 = std::make_shared<Variable>("test variable2");

    Chunk chunk(std::vector<std::shared_ptr<Variable>>{ var1, var2 });
    
    std::shared_ptr<Code> program = make_block(
        std::vector<std::shared_ptr<Code>> {
            stack::allocate(chunk),
            make_add(Reg::FramePtr, Reg::StackPtr, Reg::Zero),
            make_write(var1, Reg::Input1),
            make_write(var2, Reg::Input2),
            make_read(Reg::Result, var1),
            make_read(Reg::Scratch, var2),
            make_add(Reg::Result, Reg::Result, Reg::Scratch),
            stack::pop(),
            make_lis(Reg::TargetPC),
            make_word(TERMINATION_PC),
            make_jr(Reg::TargetPC)
        }
    );
    


    Print print_v;
    Flatten flatten;
    ElimVars elim_vars(chunk);

    program->accept(print_v);
    std::shared_ptr<Code> program2 = program->accept(elim_vars);
    program2->accept(print_v);
    program2->accept(flatten);
    
    auto program3 = flatten.get();
    write_file("test_write_file.bin", program3);
}

int main() {
    run();
    return 0;
}


