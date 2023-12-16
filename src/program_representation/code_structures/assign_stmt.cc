
#include "assign_stmt.h"

AssignStmt::AssignStmt(std::shared_ptr<Code> lhs, std::shared_ptr<Code> rhs) : lhs{lhs}, rhs{rhs} {}
