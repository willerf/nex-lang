#pragma once
#include <memory>
#include <vector>

#include "typed_id.h"
#include "nl_type.h"
#include "procedure.h"

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
