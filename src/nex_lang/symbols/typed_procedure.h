#pragma once
#include <memory>
#include <vector>

#include "nl_type.h"
#include "procedure.h"
#include "typed_id.h"
#include "typed_variable.h"

struct TypedVariable;

struct TypedProcedure: TypedID {
    std::shared_ptr<Procedure> procedure;
    std::shared_ptr<NLType> ret_type;
    std::vector<std::shared_ptr<TypedVariable>> params;
    explicit TypedProcedure(
        std::shared_ptr<Procedure> procedure = nullptr,
        std::shared_ptr<NLType> ret_type = nullptr,
        std::vector<std::shared_ptr<TypedVariable>> params = {}
    );
};
