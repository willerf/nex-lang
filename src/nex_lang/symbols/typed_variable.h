
#pragma once
#include <memory>

#include "typed_id.h"
#include "nl_type.h"
#include "variable.h"

struct TypedVariable: TypedID {
    std::shared_ptr<Variable> variable;
    std::shared_ptr<NLType> nl_type;
    explicit TypedVariable(
        std::shared_ptr<Variable> variable = nullptr,
        std::shared_ptr<NLType> nl_type = nullptr
    );
};
