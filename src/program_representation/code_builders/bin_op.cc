
#include "bin_op.h"

#include <vector>

#include "block.h"
#include "scope.h"
#include "var_access.h"
#include "variable.h"
#include "reg.h"

std::shared_ptr<Code> bin_op(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> op,
    std::shared_ptr<Code> e2
) {
    std::shared_ptr<Variable> v1 =
        std::make_shared<Variable>("bin_op variable");
    return make_scope(
        {v1},
        make_block(
            {e1,
             make_write(v1, Reg::Result),
             e2,
             make_read(Reg::Scratch, v1),
             op}
        )
    );
}
