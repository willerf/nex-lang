
#include "elim_calls.h"

#include "assembly.h"
#include "block.h"
#include "pseudo_assembly.h"
#include "scope.h"
#include "stack.h"
#include "use_label.h"
#include "var_access.h"

ElimCalls::ElimCalls(
    std::shared_ptr<Procedure> current_procedure,
    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks
) :
    current_procedure {current_procedure},
    param_chunks {param_chunks} {}

std::shared_ptr<Code> ElimCalls::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimCalls::visit(std::shared_ptr<Block> block) {
    std::vector<std::shared_ptr<Code>> result;
    for (auto c : block->code) {
        result.push_back(c->accept(*this));
    }
    return make_block(result);
}

std::shared_ptr<Code> ElimCalls::visit(std::shared_ptr<Call> call) {
    std::shared_ptr<Procedure> caller = current_procedure;
    std::shared_ptr<Procedure> callee = call->procedure;
    std::vector<std::shared_ptr<Code>> arguments = call->arguments;

    std::vector<std::shared_ptr<Variable>> tmp_vars;
    for (auto var : callee->parameters) {
        tmp_vars.push_back(std::make_shared<Variable>(
            "tmp for " + current_procedure->name + "." + var->name,
            var->is_pointer
        ));
    }

    std::shared_ptr<Chunk> param_chunk = param_chunks.at(callee);

    std::vector<std::shared_ptr<Code>> assign_to_tmps;

    auto arg = arguments.begin();
    auto tmp1 = tmp_vars.begin();
    while (arg != arguments.end() && tmp1 != tmp_vars.end()) {
        assign_to_tmps.push_back(assign(*tmp1, *arg));
        arg++;
        tmp1++;
    }

    std::vector<std::shared_ptr<Code>> tmps_to_chunk;

    auto tmp2 = tmp_vars.begin();
    auto param = param_chunk->variables.begin();
    while (tmp2 != tmp_vars.end() && param != param_chunk->variables.end()) {
        tmps_to_chunk.push_back(make_block(
            {make_read(Reg::Scratch2, *tmp2),
             param_chunk->store(Reg::Result, *param, Reg::Scratch2)}
        ));
        tmp2++;
        param++;
    }

    return make_scope(
        tmp_vars,
        {make_add(Reg::Zero, Reg::Zero, Reg::Zero),
         make_block(assign_to_tmps),
         make_add(Reg::Zero, Reg::Zero, Reg::Zero),
         stack::allocate(param_chunk),
         make_add(Reg::Zero, Reg::Zero, Reg::Zero),
         make_block(tmps_to_chunk),
         make_add(Reg::Zero, Reg::Zero, Reg::Zero),
         make_lis(Reg::TargetPC),
         make_use(callee->label),
         make_jalr(Reg::TargetPC)}
    );
}
