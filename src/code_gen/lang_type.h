/*
#pragma once

struct LangType {};

struct I32Type : LangType {};
struct FnType : LangType {
    std::vector<std::shared_ptr<LangType>> param_types;
    std::shared_ptr<LangType> ret_type;
};

struct LangSymbol {};

struct TypedVariable : LangSymbol {
    std::shared_ptr<Variable> variable;
    std::shared_ptr<LangType> lang_type;
};

struct ProcedureScope : LangSymbol {
    std::string name;
    std::vector<std::shared_ptr<TypedVariable>> params;
    std::vector<std::shared_ptr<TypedVariable>> local_vars;
    std::shared_ptr<LangType> ret_type;
};
*/
