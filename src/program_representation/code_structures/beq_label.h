
#pragma once

#include <memory>

#include "code_visit.h"
#include "label.h"
#include "reg.h"

enum class Reg;

struct BeqLabel: CodeVisit<BeqLabel> {
    Reg s;
    Reg t;
    std::shared_ptr<Label> label;
    explicit BeqLabel(Reg s, Reg t, std::shared_ptr<Label> label);
};

std::shared_ptr<BeqLabel> make_beq(Reg s, Reg t, std::shared_ptr<Label> label);
