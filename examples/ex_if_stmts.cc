
#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "chunk.h"
#include "define_label.h"
#include "elim_if_stmts.h"
#include "elim_labels.h"
#include "elim_scopes.h"
#include "elim_vars.h"
#include "flatten.h"
#include "if_stmt.h"
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
    std::shared_ptr<Variable> var3 =
        std::make_shared<Variable>("test variable3");

    auto program = make_scope(
        {var1, var2, var3},
        make_block(
            {make_write(var1, Reg::Input1),
             make_write(var2, Reg::Input2),
             make_if(
                 make_read(Reg::Result, var1),
                 eq_cmp,
                 make_read(Reg::Result, var2),
                 make_block(
                     {make_lis(Reg::Scratch),
                      make_word(83),
                      make_write(var3, Reg::Scratch)}
                 ),
                 make_block(
                     {make_lis(Reg::Scratch),
                      make_word(87),
                      make_write(var3, Reg::Scratch)}
                 )
             ),
             make_read(Reg::Result, var3)}
        )
    );

    Print p;
    program->accept(p);
    ElimIfStmts elim_if_stmts;
    auto program2 = program->accept(elim_if_stmts);
    program2->accept(p);

    ElimScopes elim_scopes;
    auto program3 = program2->accept(elim_scopes);
    auto chunk = Chunk {elim_scopes.get()};
    ElimVars elim_vars(chunk);

    std::shared_ptr<Code> program4 = program3->accept(elim_vars);

    auto program5 = make_block(
        {stack::allocate(chunk),
         make_add(Reg::FramePtr, Reg::StackPtr, Reg::Zero),
         program4,
         stack::pop()}
    );

    Flatten flatten;
    program5->accept(flatten);

    auto program6 = flatten.get();

    program6.push_back(make_lis(Reg::TargetPC));
    program6.push_back(make_word(TERMINATION_PC));
    program6.push_back(make_jr(Reg::TargetPC));

    auto program7 = elim_labels(program6);

    write_file("test_write_file.bin", program7);

    return 0;
}
