
#include "elim_labels.h"

#include <map>

#include "assembly.h"
#include "beq_label.h"
#include "bne_label.h"
#include "define_label.h"
#include "label.h"
#include "use_label.h"
#include "word.h"

template<uint32_t N>
uint32_t signed_sub(uint32_t a, uint32_t b) {
    return (uint32_t)((int32_t)a - (int32_t)b) & ((1 << N) - 1);
}

std::vector<std::shared_ptr<Code>>
elim_labels(std::vector<std::shared_ptr<Code>> program) {
    std::map<std::shared_ptr<Label>, uint32_t> symbol_table;

    uint32_t address = 0;
    for (auto& code : program) {
        if (std::shared_ptr<DefineLabel> define_label =
                std::dynamic_pointer_cast<DefineLabel>(code)) {
            if (symbol_table.count(define_label->label)) {
                std::cerr << "Duplicate label error!" << std::endl;
            } else {
                symbol_table.insert({define_label->label, address});
            }
        } else {
            address += 4;
        }
    }

    std::vector<std::shared_ptr<Code>> result;
    uint32_t location = 0;
    for (auto& code : program) {
        if (std::dynamic_pointer_cast<DefineLabel>(code)) {
            continue;
        }

        std::shared_ptr<UseLabel> debuglabel =
            std::dynamic_pointer_cast<UseLabel>(code);

        location += 4;
        if (std::shared_ptr<Word> word =
                std::dynamic_pointer_cast<Word>(code)) {
            result.push_back(word);
        } else if (std::shared_ptr<UseLabel> use_label = std::dynamic_pointer_cast<UseLabel>(code)) {
            if (symbol_table.find(use_label->label) != symbol_table.end()) {
                result.push_back(make_word(symbol_table.at(use_label->label)));
            } else {
                std::cerr << "Undefined label error!" << std::endl;
                exit(1);
            }
        } else if (std::shared_ptr<BeqLabel> beq_label = std::dynamic_pointer_cast<BeqLabel>(code)) {
            if (symbol_table.count(beq_label->label)) {
                result.push_back(make_beq(
                    beq_label->s,
                    beq_label->t,
                    signed_sub<16>(
                        symbol_table.at(beq_label->label) / 4,
                        location / 4
                    )
                ));
            } else {
                std::cerr << "Undefined label error!" << std::endl;
                exit(1);
            }
        } else if (std::shared_ptr<BneLabel> bne_label = std::dynamic_pointer_cast<BneLabel>(code)) {
            if (symbol_table.count(bne_label->label)) {
                result.push_back(make_bne(
                    bne_label->s,
                    bne_label->t,
                    signed_sub<16>(
                        symbol_table.at(bne_label->label) / 4,
                        location / 4
                    )
                ));
            } else {
                std::cerr << "Undefined label error!" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Invalid code structure!" << std::endl;
            exit(1);
        }
    }

    return result;
}
