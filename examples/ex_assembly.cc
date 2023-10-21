
#include "assembly.h"
#include "block.h"
#include "print.h"

int main() {
    Print print_v;

    std::shared_ptr<Code> code =
        std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {
            make_add(Reg::Result, Reg::Input1, Reg::Input2),
        });

    code->accept(print_v);

    return 0;
}
