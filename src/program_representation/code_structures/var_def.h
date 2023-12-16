#pragma once

#include <string>
#include <optional>
#include <memory>

#include "nl_type.h"

struct VarDef {
    std::string name;
    std::shared_ptr<NLType> nl_type;
};
