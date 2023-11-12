
#include "bne_label.h"

enum class Reg;

BneLabel::BneLabel(Reg s, Reg t, std::shared_ptr<Label> label) :
    s {s},
    t {t},
    label {label} {}

std::shared_ptr<BneLabel> make_bne(Reg s, Reg t, std::shared_ptr<Label> label) {
    return std::make_shared<BneLabel>(s, t, label);
}
