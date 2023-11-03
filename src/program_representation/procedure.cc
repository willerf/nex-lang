
#include "procedure.h"

Procedure::Procedure(
    std::string name,
    std::vector<std::shared_ptr<Variable>> parameters
) :
    name {name},
    parameters {parameters} {
    param_ptr = std::make_shared<Variable>("param ptr for " + name);
    dynamic_link = std::make_shared<Variable>("dynamic link for " + name);
    saved_pc = std::make_shared<Variable>("saved pc for " + name);
    start_label = std::make_shared<Label>("procedure " + name);
    end_label = std::make_shared<Label>("procedure " + name + " end");
}
