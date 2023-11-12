
#include "while_loop.h"

#include <vector>

#include "beq_label.h"
#include "block.h"
#include "define_label.h"
#include "if_stmt.h"
#include "label.h"
#include "reg.h"

std::shared_ptr<Code> make_while(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> body
) {
    std::shared_ptr<Label> top_of_loop =
        std::make_shared<Label>("top of while loop");
    return make_block(
        {make_define(top_of_loop),
         make_if(
             e1,
             comp,
             e2,
             make_block({body, make_beq(Reg::Zero, Reg::Zero, top_of_loop)})
         )}
    );
}
