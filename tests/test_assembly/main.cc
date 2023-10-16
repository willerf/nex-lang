
#include "../../src/program_representation/code_structures/block.h"
#include "../../src/program_representation/assembly.h"
#include "../../src/program_representation/assembly.h"

#include "../../src/transformations/print.h"

using namespace std;

int main() {

    Print print_v;

    shared_ptr<Code> code = make_shared<Block>(
        vector<shared_ptr<Code>>{
            make_add(Reg::Result, Reg::Input1, Reg::Input2),
        }
    );

    code->accept(print_v);

    return 0;
}
