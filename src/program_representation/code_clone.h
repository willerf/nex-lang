
#pragma once

#include "code.h"

#include <memory>

template <typename T>
struct CodeClone : Code {
    std::shared_ptr<Code> clone() const override {
        return std::make_shared<T>(static_cast<const T&>(*this));
    }
};
