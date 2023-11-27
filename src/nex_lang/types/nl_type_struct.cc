

#include "nl_type_struct.h"

#include <algorithm>
#include <iostream>

NLTypeStruct::NLTypeStruct(
    std::string name,
    std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types
) :
    name {name},
    child_types {child_types} {}

bool NLTypeStruct::equals(const NLType& other) const {
    if (type() != other.type()) {
        return false;
    }
    auto& other_ptr = static_cast<const NLTypeStruct&>(other);

    return name == other_ptr.name;
}

bool NLTypeStruct::less_than(const NLType& other) const {
    if (type() != other.type()) {
        return type() < other.type();
    }
    auto& other_ptr = static_cast<const NLTypeStruct&>(other);

    return name < other_ptr.name;
}

std::type_index NLTypeStruct::type() const {
    return typeid(NLTypeStruct);
}

std::string NLTypeStruct::to_string() {
    return name;
}

uint32_t NLTypeStruct::bytes() {
    uint32_t result = 0;
    for (auto child_type : child_types) {
        result += child_type.second->bytes();
    }
    return result;
}
