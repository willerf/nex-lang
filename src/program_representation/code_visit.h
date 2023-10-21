
#pragma once

#include <vector>

#include "code_clone.h"

template<typename T>
struct CodeVisit: CodeClone<T>, std::enable_shared_from_this<CodeVisit<T>> {
    void accept(Visitor<void>& visitor) override {
        visitor.visit(std::static_pointer_cast<T>(this->shared_from_this()));
    }

    std::shared_ptr<Code> accept(Visitor<std::shared_ptr<Code>>& visitor
    ) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }
};
