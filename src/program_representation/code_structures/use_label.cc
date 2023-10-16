
#include "use_label.h"

UseLabel::UseLabel(std::shared_ptr<Label> label) : label{label} {}

std::shared_ptr<UseLabel> make_use(std::shared_ptr<Label> label) {
    return std::make_shared<UseLabel>(label);
}

