
#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "code.h"
#include "code_visit.h"
#include "nl_type.h"
#include "var_def.h"

struct Procedure: CodeVisit<Procedure> {
    std::string name;
    std::vector<VarDef> args;
    std::shared_ptr<NLType> ret_type;
    std::shared_ptr<Code> code;
    Procedure(std::string name, std::vector<VarDef> args, std::shared_ptr<NLType> ret_type, std::shared_ptr<Code> code);
};

