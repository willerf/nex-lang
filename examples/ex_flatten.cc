
#include <memory>
#include <vector>

#include "block.h"
#include "flatten.h"
#include "print.h"
#include "word.h"

struct Code;

int main() {
    Flatten flatten;
    Print print_v;

    std::shared_ptr<Code> code =
        std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {
            std::make_shared<Word>(1),
            std::make_shared<Word>(2),
            std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {
                std::make_shared<Word>(3),
                std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {
                    std::make_shared<Word>(4),
                    std::make_shared<Word>(5),
                    std::make_shared<Word>(6),
                }),
                std::make_shared<Word>(7)
            }),
            std::make_shared<Word>(8),
            std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {
                std::make_shared<Block>(std::vector<std::shared_ptr<Code>> {}),
                std::make_shared<Word>(9)
            })
        });

    code->accept(print_v);
    code->accept(flatten);
    code = std::make_shared<Block>(flatten.get());
    code->accept(print_v);

    return 0;
}
