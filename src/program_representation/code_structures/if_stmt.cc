
#include "if_stmt.h"

IfStmt::IfStmt(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> thens,
    std::shared_ptr<Code> elses
) :
    e1 {e1},
    comp {comp},
    e2 {e2},
    thens {thens},
    elses {elses} {}

std::shared_ptr<Code> make_if(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> thens,
    std::shared_ptr<Code> elses
) {
    return std::make_shared<IfStmt>(e1, comp, e2, thens, elses);
}
