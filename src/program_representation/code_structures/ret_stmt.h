
#pragma once

#include <memory>

#include "code_visit.h"
#include "code.h"

struct RetStmt: CodeVisit<RetStmt> {
    std::shared_ptr<Code> code;
    RetStmt(std::shared_ptr<Code> code);
};
