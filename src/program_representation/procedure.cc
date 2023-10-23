
#include "procedure.h"

Procedure::Procedure(std::string name, std::vector<std::shared_ptr<Variable>> parameters) : name{name}, parameters{parameters} {
    param_ptr = std::make_shared<Variable>("param ptr for " + name, true);
    dynamic_link = std::make_shared<Variable>("dynamic link for " + name, true);
    saved_pc = std::make_shared<Variable>("saved pc for " + name, true);
    label = std::make_shared<Label>("procedure " + name);
}

