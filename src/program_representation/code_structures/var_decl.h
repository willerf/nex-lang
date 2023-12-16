
#pragma once

#include <memory>
#include <optional>

#include "code.h"
#include "code_visit.h"
#include "nl_type.h"
#include "var_def.h"

struct VarDecl: CodeVisit<VarDecl> {
    VarDef var_def;
    std::shared_ptr<Code> rhs;
    VarDecl(VarDef var_def, std::shared_ptr<Code> rhs);
};
