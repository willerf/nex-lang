
#include "define_label.h"

DefineLabel::DefineLabel(std::shared_ptr<Label> label) : label{label} {}

std::shared_ptr<DefineLabel> make_define(std::shared_ptr<Label> label) {
    return std::make_shared<DefineLabel>(label);
}


