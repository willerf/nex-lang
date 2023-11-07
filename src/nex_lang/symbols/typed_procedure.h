#pragma once
#include "typed_id.h"

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
