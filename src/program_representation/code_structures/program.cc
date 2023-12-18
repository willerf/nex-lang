#include "program.h"

Program::Program(std::string name, std::vector<Module> modules)
    : name{name}, modules{modules} {}
