
#include "typed_procedure.h"

TypedProcedure::TypedProcedure(
    std::shared_ptr<Procedure> procedure,
    std::shared_ptr<NLType> ret_type,
    std::vector<std::shared_ptr<TypedVariable>> params
) :
    procedure {procedure},
    ret_type {ret_type},
    params {params} {}
