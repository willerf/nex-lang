
#include "nl_type_char.h"

bool NLTypeChar::equals(const NLType& other) const {
    return type() == other.type();
}

bool NLTypeChar::less_than(const NLType& other) const {
    return type() < other.type();
}

std::type_index NLTypeChar::type() const {
    return typeid(NLTypeChar);
}

std::string NLTypeChar::to_string() {
    return "char";
}
