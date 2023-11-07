
#pragma once

#include <memory>
#include <typeindex>

struct NLType {
    virtual ~NLType() = default;
    virtual bool equals(const NLType& other) const = 0;
    virtual std::type_index type() const = 0;
};

struct NLTypeI32: NLType {
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
};

struct NLTypePtr: NLType {
    std::shared_ptr<NLType> nl_type;

    explicit NLTypePtr(std::shared_ptr<NLType> nl_type);
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
};

bool operator==(const NLType& lhs, const NLType& rhs);
bool operator!=(const NLType& lhs, const NLType& rhs);
