
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <string>

#include "code.h"
#include "code_visit.h"
#include "module.h"

struct Program: CodeVisit<Program> {
    std::string name;
    std::vector<Module> modules;
    Program(std::string name, std::vector<Module> modules);
};
