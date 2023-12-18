#include "procedure.h"

Procedure::Procedure(std::string name, std::vector<VarDef> args, std::shared_ptr<NLType> ret_type, std::shared_ptr<Code> code) 
    : name{name}, args{args}, ret_type{ret_type}, code{code} {}
