
#include <map>
#include <memory>
#include <vector>

#include "chunk.h"
#include "elim_calls.h"
#include "elim_if_stmts.h"
#include "elim_ret_stmts.h"
#include "elim_scopes.h"
#include "elim_vars_proc.h"
#include "entry_exit.h"

struct Variable;
struct Procedure;

void compile_procedure(
    std::shared_ptr<Procedure> proc,
    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks
) {
    ElimCalls elim_calls {proc, param_chunks};
    proc->code = proc->code->accept(elim_calls);

    ElimIfStmts elim_if_stmts;
    proc->code = proc->code->accept(elim_if_stmts);

    ElimRetStmts elim_ret_stmts {proc->end_label};
    proc->code = proc->code->accept(elim_ret_stmts);

    ElimScopes elim_scopes;
    proc->code = proc->code->accept(elim_scopes);
    auto local_vars = elim_scopes.get();

    std::vector<std::shared_ptr<Variable>> all_local_vars = {
        proc->param_ptr,
        proc->dynamic_link,
        proc->saved_pc};
    all_local_vars
        .insert(all_local_vars.end(), local_vars.begin(), local_vars.end());
    std::shared_ptr<Chunk> local_vars_chunk =
        std::make_shared<Chunk>(all_local_vars);

    proc->code = add_entry_exit(proc, local_vars_chunk);

    auto param_chunk = std::make_shared<Chunk>(proc->parameters);
    ElimVarsProc elim_vars_proc(local_vars_chunk, param_chunk, proc->param_ptr);
    proc->code = proc->code->accept(elim_vars_proc);
}
