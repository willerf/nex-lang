
#pragma once

#include <memory>

#include "code_visit.h"
#include "label.h"

struct UseLabel: CodeVisit<UseLabel> {
    const std::shared_ptr<Label> label;
    explicit UseLabel(std::shared_ptr<Label> label);
};

std::shared_ptr<UseLabel> make_use(std::shared_ptr<Label> label);
