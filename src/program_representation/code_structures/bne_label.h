
#pragma once

#include "../code_visit.h"
#include "../label.h"
#include "../../utils/reg.h"

struct BneLabel : CodeVisit<BneLabel> {
    Reg s;
    Reg t;
    std::shared_ptr<Label> label;
    BneLabel(Reg s, Reg t, std::shared_ptr<Label> label);
};

std::shared_ptr<BneLabel> make_bne(Reg s, Reg t, std::shared_ptr<Label> label);

