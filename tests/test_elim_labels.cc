
#include <stdint.h>

#include <bitset>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <string>
#include <vector>

#include "beq_label.h"
#include "bne_label.h"
#include "catch2/matchers/catch_matchers.hpp"
#include "define_label.h"
#include "elim_labels.h"
#include "label.h"
#include "reg.h"
#include "use_label.h"
#include "utils.h"
#include "word.h"

struct Code;

TEST_CASE("Simple test use label", "[labels]") {
    auto label = std::make_shared<Label>("label1");

    std::vector<std::shared_ptr<Code>> program1 = {
        make_word(1),  // address 0
        make_word(2),  // 4
        make_define(label),  // 8
        make_word(3),  // 8
        make_word(4),  // 12
        make_use(label),  // 16
        make_word(5),  // 20
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(std::vector<uint32_t> {1, 2, 3, 4, 8, 5})
    );
}

TEST_CASE("Multiple use label", "[labels]") {
    auto label1 = std::make_shared<Label>("label1");
    auto label2 = std::make_shared<Label>("label2");
    auto label3 = std::make_shared<Label>("label3");

    std::vector<std::shared_ptr<Code>> program1 = {
        make_word(1),  // address 0
        make_use(label3),  // 4
        make_use(label2),  // 8
        make_use(label1),  // 12
        make_word(2),  // 16
        make_word(3),  // 20
        make_define(label1),  // 24
        make_define(label2),  // 24
        make_define(label3),  // 24
        make_word(4),  // 24
        make_use(label1),  // 28
        make_use(label3),  // 32
        make_use(label2),  // 36
        make_word(5),  // 40
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(
            std::vector<uint32_t> {1, 24, 24, 24, 2, 3, 4, 24, 24, 24, 5}
        )
    );
}

TEST_CASE("Simple test positive beq label", "[labels]") {
    auto label = std::make_shared<Label>("label1");

    Reg r1 = Reg::Scratch;
    Reg r2 = Reg::Result;
    std::vector<std::shared_ptr<Code>> program1 = {
        make_word(1),  // address 0
        make_beq(r1, r2, label),  // 4
        make_word(2),  // 8
        make_word(3),  // 12
        make_word(4),  // 16
        make_define(label),  // 20
        make_word(5),  // 20
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    auto instr = std::bitset<32>(
        "000100" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(3).to_string()
    );

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(
            std::vector<uint32_t> {1, (uint32_t)instr.to_ulong(), 2, 3, 4, 5}
        )
    );
}

TEST_CASE("Simple test positive bne label", "[labels]") {
    auto label = std::make_shared<Label>("label1");

    Reg r1 = Reg::Input1;
    Reg r2 = Reg::Input2;
    std::vector<std::shared_ptr<Code>> program1 = {
        make_bne(r1, r2, label),  // address 0
        make_word(1),  // 4
        make_word(2),  // 8
        make_word(3),  // 12
        make_word(4),  // 16
        make_word(5),  // 20
        make_define(label),  // 24
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    auto instr = std::bitset<32>(
        "000101" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(5).to_string()
    );

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(
            std::vector<uint32_t> {(uint32_t)instr.to_ulong(), 1, 2, 3, 4, 5}
        )
    );
}

TEST_CASE("Simple test negative beq label", "[labels]") {
    auto label = std::make_shared<Label>("label1");

    Reg r1 = Reg::Scratch;
    Reg r2 = Reg::Result;
    std::vector<std::shared_ptr<Code>> program1 = {
        make_word(1),  // address 0
        make_word(2),  // 4
        make_define(label),  // 8
        make_word(3),  // 8
        make_word(4),  // 12
        make_beq(r1, r2, label),  // 16
        make_word(5),  // 20
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    auto instr = std::bitset<32>(
        "000100" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(-3).to_string()
    );

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(
            std::vector<uint32_t> {1, 2, 3, 4, (uint32_t)instr.to_ulong(), 5}
        )
    );
}

TEST_CASE("Simple test negative bne label", "[labels]") {
    auto label = std::make_shared<Label>("label1");

    Reg r1 = Reg::Input1;
    Reg r2 = Reg::Input2;
    std::vector<std::shared_ptr<Code>> program1 = {
        make_define(label),  // address 0
        make_word(1),  // 0
        make_word(2),  // 4
        make_word(3),  // 8
        make_word(4),  // 12
        make_bne(r1, r2, label),  // 16
        make_word(5),  // 20
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    auto instr = std::bitset<32>(
        "000101" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(-5).to_string()
    );

    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(
            std::vector<uint32_t> {1, 2, 3, 4, (uint32_t)instr.to_ulong(), 5}
        )
    );
}

TEST_CASE("Simple test bne/beq adjacent", "[labels]") {
    auto label1 = std::make_shared<Label>("label1");
    auto label2 = std::make_shared<Label>("label2");

    Reg r1 = Reg::Input1;
    Reg r2 = Reg::Input2;
    std::vector<std::shared_ptr<Code>> program1 = {
        make_word(1),  // address 0
        make_word(2),  // 4
        make_bne(r1, r2, label1),  // 8
        make_define(label1),  // 12
        make_beq(r1, r2, label1),  // 12
        make_word(3),  // 16
        make_word(4),  // 20
        make_beq(r1, r2, label2),  // 24
        make_define(label2),  // 28
        make_bne(r1, r2, label2),  // 28
        make_word(5),  // 32
    };

    auto program2 = elim_labels(program1);
    auto program3 = word_to_uint(program2);

    auto instr1 = std::bitset<32>(
        "000101" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(0).to_string()
    );
    auto instr2 = std::bitset<32>(
        "000100" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(-1).to_string()
    );
    auto instr3 = std::bitset<32>(
        "000100" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(0).to_string()
    );
    auto instr4 = std::bitset<32>(
        "000101" + std::bitset<5>((uint32_t)r1).to_string()
        + std::bitset<5>((uint32_t)r2).to_string()
        + std::bitset<16>(-1).to_string()
    );
    REQUIRE_THAT(
        program3,
        Catch::Matchers::Equals(std::vector<uint32_t> {
            1,
            2,
            (uint32_t)instr1.to_ulong(),
            (uint32_t)instr2.to_ulong(),
            3,
            4,
            (uint32_t)instr3.to_ulong(),
            (uint32_t)instr4.to_ulong(),
            5})
    );
}
