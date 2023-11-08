
#pragma once

#include <memory>

#include "code.h"
#include "nl_type.h"

struct TypedExpr {
    std::shared_ptr<Code> code;
    std::shared_ptr<NLType> nl_type;
};
