
#include "nl_lib.h"

#include <map>
#include <span>

#include "ast_node.h"
#include "compile_error.h"
#include "extract_symbols.h"
#include "module_table.h"
#include "nex_lang_parsing.h"
#include "nex_lang_scanning.h"

static const std::string print_module(
#include "print_module.nl"
);

static const std::string math_module(
#include "math_module.nl"
);

static const std::string list_module(
#include "list_module.nl"
);

static std::map<std::string, std::string> nl_lib {
    {"print", print_module},
    {"math", math_module},
    {"list", list_module}};

void nl_lib_import(
    std::string import_name,
    std::vector<std::string>& import_list,
    ModuleTable& module_table,
    std::vector<std::pair<std::string, ASTNode>>& modules
) {
    if (nl_lib.contains(import_name) && !module_table.contains(import_name)) {
        std::string input = nl_lib.at(import_name);
        std::string input_file_path = "nl_lib/" + import_name;
        try {
            auto tokens = scan(input);
            auto ast_node = parse(tokens);
            auto result_list = extract_symbols(ast_node, module_table);
            import_list.insert(
                import_list.end(),
                result_list.begin(),
                result_list.end()
            );
            modules.push_back({input_file_path, ast_node});
        } catch (CompileError& compile_error) {
            compile_error.input_file_path = input_file_path;
            throw;
        }
    }
}
