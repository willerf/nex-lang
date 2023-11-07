
#pragma once

#include <memory>

#include "nl_type.h"
#include "procedure.h"
#include "variable.h"

struct TypedID {
    virtual ~TypedID() {}
};

struct TypedVariable: TypedID {
    std::shared_ptr<Variable> variable;
    std::shared_ptr<NLType> nl_type;
    explicit TypedVariable(
        std::shared_ptr<Variable> variable = nullptr,
        std::shared_ptr<NLType> nl_type = nullptr
    );
};

struct TypedProcedure: TypedID {
    std::shared_ptr<Procedure> procedure;
    std::shared_ptr<NLType> ret_type;
    std::vector<std::shared_ptr<NLType>> param_types;
    explicit TypedProcedure(
        std::shared_ptr<Procedure> procedure = nullptr,
        std::shared_ptr<NLType> ret_type = nullptr,
        std::vector<std::shared_ptr<NLType>> param_types = {}
    );
};
