
#include "nl_type.h"

bool NLTypeI32::equals(const NLType& other) const {
    return type() == other.type();
}

std::type_index NLTypeI32::type() const {
    return typeid(NLTypeI32);
}

NLTypePtr::NLTypePtr(std::shared_ptr<NLType> nl_type) : nl_type {nl_type} {}

bool NLTypePtr::equals(const NLType& other) const {
    if (type() != other.type()) {
        return false;
    }
    auto& other_ptr = static_cast<const NLTypePtr&>(other);
    return nl_type->equals(*other_ptr.nl_type);
}

std::type_index NLTypePtr::type() const {
    return typeid(NLTypePtr);
}

bool operator==(const NLType& lhs, const NLType& rhs) {
    return lhs.equals(rhs);
}

bool operator!=(const NLType& lhs, const NLType& rhs) {
    return !lhs.equals(rhs);
}
