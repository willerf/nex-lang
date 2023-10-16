
#pragma once

#include "../transformations/visitor.h"

#include <vector>

struct Code {
    virtual void accept(Visitor<void>& visitor) = 0;
    virtual std::shared_ptr<Code> accept(Visitor<std::shared_ptr<Code>>& visitor) = 0;
    virtual std::shared_ptr<Code> clone() const = 0;
    virtual ~Code() {}
};

