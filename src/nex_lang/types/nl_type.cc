
#include "nl_type.h"

bool operator==(const NLType& lhs, const NLType& rhs) {
    return lhs.equals(rhs);
}

bool operator!=(const NLType& lhs, const NLType& rhs) {
    return !lhs.equals(rhs);
}
