
#include "typed_procedure.h"

TypedProcedure::TypedProcedure(
    std::shared_ptr<Procedure> procedure,
    std::shared_ptr<NLType> ret_type,
    std::vector<std::shared_ptr<NLType>> param_types
) :
    procedure {procedure},
    ret_type {ret_type},
    param_types {param_types} {}
