
#include <stdint.h>

#include <bitset>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <vector>

#include "block.h"
#include "catch2/matchers/catch_matchers.hpp"
#include "chunk.h"
#include "elim_vars.h"
#include "flatten.h"
#include "reg.h"
#include "utils.h"
#include "var_access.h"
#include "variable.h"

TEST_CASE("Simple test read write variable", "[vars]") {
    auto var1 = std::make_shared<Variable>("var1");
    auto var2 = std::make_shared<Variable>("var2");

    auto r1 = Reg::Scratch;
    auto r2 = Reg::Result;
    auto program1 = make_block({
        make_read(r1, var1),
        make_write(var2, r2),
    });

    std::vector<std::shared_ptr<Variable>> all_vars = {var1, var2};
    std::shared_ptr<Chunk> frame = std::make_shared<Chunk>(all_vars);
    ElimVars elim_vars {frame};
    auto program2 = program1->accept(elim_vars);

    Flatten flatten;
    program2->accept(flatten);
    auto program3 = flatten.get();

    auto program4 = word_to_uint(program3);

    auto instr1 = std::bitset<32>(
        "100011" + std::bitset<5>((uint32_t)Reg::FramePtr).to_string()
        + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<16>(8).to_string()
    );

    auto instr2 = std::bitset<32>(
        "101011" + std::bitset<5>((uint32_t)Reg::FramePtr).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(12).to_string()
    );

    REQUIRE_THAT(
        program4,
        Catch::Matchers::Equals(std::vector<uint32_t> {
            (uint32_t)instr1.to_ulong(),
            (uint32_t)instr2.to_ulong()})
    );
}
