
#include "nl_type_i32.h"

bool NLTypeI32::equals(const NLType& other) const {
    return type() == other.type();
}

std::type_index NLTypeI32::type() const {
    return typeid(NLTypeI32);
}

std::string NLTypeI32::to_string() {
    return "i32";
}
