

#pragma once

#include <string>
#include <typeindex>

#include "nl_type.h"

struct NLTypeStruct: NLType {
    std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types;

    explicit NLTypeStruct(
        std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types
    );
    bool equals(const NLType& other) const override;
    bool less_than(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
    uint32_t bytes() override;
};
