

#pragma once

#include "nl_type.h"

struct NLTypeBool: NLType {
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
};
