
#pragma once

#include "code_visit.h"
#include "label.h"

struct DefineLabel: CodeVisit<DefineLabel> {
    const std::shared_ptr<Label> label;
    explicit DefineLabel(std::shared_ptr<Label> label);
};

std::shared_ptr<DefineLabel> make_define(std::shared_ptr<Label> label);
