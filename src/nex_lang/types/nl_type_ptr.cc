
#include "nl_type_ptr.h"

NLTypePtr::NLTypePtr(std::shared_ptr<NLType> nl_type) : nl_type {nl_type} {}

bool NLTypePtr::equals(const NLType& other) const {
    if (type() != other.type()) {
        return false;
    }
    auto& other_ptr = static_cast<const NLTypePtr&>(other);
    return nl_type->equals(*other_ptr.nl_type);
}

bool NLTypePtr::less_than(const NLType& other) const {
    if (type() != other.type()) {
        return type() < other.type();
    }
    auto& other_ptr = static_cast<const NLTypePtr&>(other);
    return nl_type->less_than(*other_ptr.nl_type);
}

std::type_index NLTypePtr::type() const {
    return typeid(NLTypePtr);
}

std::string NLTypePtr::to_string() {
    return "*" + nl_type->to_string();
}
