#pragma once

#include <memory>
#include <string>
#include <typeindex>

#include "nl_type.h"

struct NLTypePtr: NLType {
    std::shared_ptr<NLType> nl_type;

    explicit NLTypePtr(std::shared_ptr<NLType> nl_type);
    bool equals(const NLType& other) const override;
    bool less_than(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
};
