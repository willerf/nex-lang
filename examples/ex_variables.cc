
#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "chunk.h"
#include "define_label.h"
#include "elim_labels.h"
#include "elim_vars.h"
#include "flatten.h"
#include "label.h"
#include "print.h"
#include "reg.h"
#include "stack.h"
#include "use_label.h"
#include "var_access.h"
#include "variable.h"
#include "word.h"
#include "write_file.h"

const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

int main() {
    std::shared_ptr<Variable> var1 =
        std::make_shared<Variable>("test variable1");
    std::shared_ptr<Variable> var2 =
        std::make_shared<Variable>("test variable2");

    Chunk chunk(std::vector<std::shared_ptr<Variable>> {var1, var2});

    std::shared_ptr<Code> program =
        make_block(std::vector<std::shared_ptr<Code>> {
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
            make_jr(Reg::TargetPC)});

    Print print_v;
    Flatten flatten;
    ElimVars elim_vars(chunk);

    program->accept(print_v);
    std::shared_ptr<Code> program2 = program->accept(elim_vars);
    program2->accept(print_v);
    program2->accept(flatten);

    auto program3 = flatten.get();
    write_file("test_write_file.bin", program3);

    return 0;
}
