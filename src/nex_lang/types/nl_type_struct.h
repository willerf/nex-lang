

#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

#include "nl_type.h"

struct NLTypeStruct: NLType {
    std::string name;
    std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types;

    explicit NLTypeStruct(
        std::string name,
        std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types
    );
    bool equals(const NLType& other) const override;
    bool less_than(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
    uint32_t bytes() override;
};
