

#include "nl_type_struct.h"

NLTypeStruct::NLTypeStruct(
    std::vector<std::pair<std::string, std::shared_ptr<NLType>>> child_types
) :
    child_types {child_types} {}

bool NLTypeStruct::equals(const NLType& other) const {
    if (type() != other.type()) {
        return false;
    }
    auto& other_ptr = static_cast<const NLTypeStruct&>(other);
    for (size_t i = 0;
         i < std::min(child_types.size(), other_ptr.child_types.size());
         ++i) {
        if (child_types.at(i).second->type()
            != other_ptr.child_types.at(i).second->type()) {
            return false;
        }
    }
    return true;
}

std::type_index NLTypeStruct::type() const {
    return typeid(NLTypeStruct);
}

std::string NLTypeStruct::to_string() {
    return "struct";
}

uint32_t NLTypeStruct::bytes() {
    uint32_t result = 0;
    for (auto child_type : child_types) {
        result += child_type.second->bytes();
    }
    return result;
}
