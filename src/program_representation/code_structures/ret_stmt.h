
#pragma once

#include "code_visit.h"

struct RetStmt : CodeVisit<RetStmt> {
    std::shared_ptr<Code> code;
    RetStmt(std::shared_ptr<Code> code);
};

