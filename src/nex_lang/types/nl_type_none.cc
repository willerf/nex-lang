
#include "nl_type_none.h"

bool NLTypeNone::equals(const NLType& other) const {
    return type() == other.type();
}

bool NLTypeNone::less_than(const NLType& other) const {
    return type() < other.type();
}

std::type_index NLTypeNone::type() const {
    return typeid(NLTypeNone);
}

std::string NLTypeNone::to_string() {
    return "none";
}
