
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "flatten.h"
#include "word.h"
#include "block.h"

TEST_CASE( "Flatten simple nested structure", "[flatten]" ) {
    
    std::shared_ptr<Word> word1 = std::make_shared<Word>(1);
    std::shared_ptr<Word> word2 = std::make_shared<Word>(2);
    std::shared_ptr<Word> word3 = std::make_shared<Word>(3);
    std::shared_ptr<Word> word4 = std::make_shared<Word>(4);

    std::shared_ptr<Block> block = std::make_shared<Block>(std::vector<std::shared_ptr<Code>>{
        word1,
        std::make_shared<Block>(std::vector<std::shared_ptr<Code>>{
            word2,
            word3
        }),
        word4
    });

    Flatten flatten;
    block->accept(flatten);

    REQUIRE_THAT(flatten.get(), Catch::Matchers::Equals(std::vector<std::shared_ptr<Code>>{ word1, word2, word3, word4 }));
}
     
