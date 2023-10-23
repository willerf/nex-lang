
#pragma once

#include "code_visit.h"

#include "procedure.h"

struct Call : CodeVisit<Call> {
    std::shared_ptr<Procedure> procedure;
    std::vector<std::shared_ptr<Code>> arguments;
    explicit Call(std::shared_ptr<Procedure> procedure, std::vector<std::shared_ptr<Code>> arguments);
};

std::shared_ptr<Code> make_call(std::shared_ptr<Procedure> procedure, std::vector<std::shared_ptr<Code>> arguments);

