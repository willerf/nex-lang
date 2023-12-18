
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <string>

#include "code.h"
#include "code_visit.h"
#include "procedure.h"

struct Module: CodeVisit<Module> {
    std::string name;
    std::vector<Procedure> procedures;
    Module(std::string name, std::vector<Procedure> procedures);
};
