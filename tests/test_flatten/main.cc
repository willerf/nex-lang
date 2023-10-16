
#include "../../src/program_representation/code_structures/block.h"
#include "../../src/program_representation/code_structures/word.h"

#include "../../src/transformations/flatten.h"
#include "../../src/transformations/print.h"

using namespace std;

int main() {

    Flatten flatten;
    Print print_v;

    shared_ptr<Code> code = make_shared<Block>(
        vector<shared_ptr<Code>>{
            make_shared<Word>(1),
            make_shared<Word>(2),
            make_shared<Block>(
                vector<shared_ptr<Code>>{
                    make_shared<Word>(3),
                    make_shared<Block>(
                        vector<shared_ptr<Code>>{
                            make_shared<Word>(4),
                            make_shared<Word>(5),
                            make_shared<Word>(6),
                        }
                    ),
                    make_shared<Word>(7)
                }
            ),
            make_shared<Word>(8),
            make_shared<Block>(
                vector<shared_ptr<Code>>{
                    make_shared<Block>(
                        vector<shared_ptr<Code>>{}
                    ),
                    make_shared<Word>(9)
                }
            )
        }
    );

    code->accept(print_v);
    code->accept(flatten);
    code = make_shared<Block>(flatten.get());
    code->accept(print_v);
    
    return 0;
}
