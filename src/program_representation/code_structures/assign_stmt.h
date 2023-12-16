
#pragma once

#include <memory>
#include <optional>

#include "code.h"
#include "code_visit.h"
#include "nl_type.h"

struct AssignStmt: CodeVisit<AssignStmt> {
    std::shared_ptr<Code> lhs;
    std::shared_ptr<Code> rhs;
    AssignStmt(std::shared_ptr<Code> lhs, std::shared_ptr<Code> rhs);
};
