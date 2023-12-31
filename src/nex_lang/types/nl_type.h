
#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <typeindex>

struct NLType {
    virtual ~NLType() = default;
    virtual bool equals(const NLType& other) const = 0;
    virtual bool less_than(const NLType& other) const = 0;
    virtual std::type_index type() const = 0;
    virtual std::string to_string() = 0;
    virtual uint32_t bytes();
};

bool operator==(const NLType& lhs, const NLType& rhs);
bool operator!=(const NLType& lhs, const NLType& rhs);
bool operator<(const NLType& lhs, const NLType& rhs);
