
#pragma once

#include <memory>
#include <string>
#include <typeindex>

struct NLType {
    virtual ~NLType() = default;
    virtual bool equals(const NLType& other) const = 0;
    virtual std::type_index type() const = 0;
    virtual std::string to_string() = 0;
};

struct NLTypeI32: NLType {
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
};

struct NLTypeBool: NLType {
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
};

struct NLTypePtr: NLType {
    std::shared_ptr<NLType> nl_type;

    explicit NLTypePtr(std::shared_ptr<NLType> nl_type);
    bool equals(const NLType& other) const override;
    std::type_index type() const override;
    std::string to_string() override;
};

bool operator==(const NLType& lhs, const NLType& rhs);
bool operator!=(const NLType& lhs, const NLType& rhs);
