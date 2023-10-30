
#pragma once

#include <functional>

#include "assembly.h"
#include "block.h"
#include "code_visit.h"
#include "label.h"

struct IfStmt: CodeVisit<IfStmt> {
    std::shared_ptr<Code> e1;
    std::shared_ptr<Code> comp;
    std::shared_ptr<Code> e2;
    std::shared_ptr<Code> thens;
    std::shared_ptr<Code> elses;
    IfStmt(
        std::shared_ptr<Code> e1,
        std::shared_ptr<Code> comp,
        std::shared_ptr<Code> e2,
        std::shared_ptr<Code> thens,
        std::shared_ptr<Code> elses
    );
};

std::shared_ptr<Code> make_if(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> thens,
    std::shared_ptr<Code> elses = make_block({})
);
