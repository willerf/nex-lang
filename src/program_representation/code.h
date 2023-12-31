
#pragma once

#include <vector>

#include "visitor.h"

struct Code {
    virtual void accept(Visitor<void>& visitor) = 0;
    virtual std::shared_ptr<Code> accept(Visitor<std::shared_ptr<Code>>& visitor
    ) = 0;
    virtual std::shared_ptr<Code> clone() const = 0;

    virtual ~Code() {}
};
