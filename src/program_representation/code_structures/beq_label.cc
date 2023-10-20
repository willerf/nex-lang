
#include "beq_label.h"

BeqLabel::BeqLabel(Reg s, Reg t, std::shared_ptr<Label> label) :
    s {s},
    t {t},
    label {label} {}

std::shared_ptr<BeqLabel> make_beq(Reg s, Reg t, std::shared_ptr<Label> label) {
    return std::make_shared<BeqLabel>(s, t, label);
}
