
#include "nl_type_bool.h"

bool NLTypeBool::equals(const NLType& other) const {
    return type() == other.type();
}

bool NLTypeBool::less_than(const NLType& other) const {
    return type() < other.type();
}

std::type_index NLTypeBool::type() const {
    return typeid(NLTypeBool);
}

std::string NLTypeBool::to_string() {
    return "bool";
}
