#include "module.h"


Module::Module(std::string name, std::vector<Procedure> procedures)
    : name{name}, procedures{procedures} {}
