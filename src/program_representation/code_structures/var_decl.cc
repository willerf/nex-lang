
#include "var_decl.h"

VarDecl::VarDecl(VarDef var_def, std::shared_ptr<Code> rhs) : var_def{var_def}, rhs{rhs} {}

