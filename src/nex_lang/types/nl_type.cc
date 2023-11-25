
#include "nl_type.h"

uint32_t NLType::bytes() {
    return 4;
}

bool operator==(const NLType& lhs, const NLType& rhs) {
    return lhs.equals(rhs);
}

bool operator!=(const NLType& lhs, const NLType& rhs) {
    return !lhs.equals(rhs);
}

bool operator<(const NLType& lhs, const NLType& rhs) {
    return lhs.less_than(rhs);
}
