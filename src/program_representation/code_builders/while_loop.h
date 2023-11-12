
#pragma once

#include <functional>
#include <memory>

#include "assembly.h"
#include "block.h"
#include "code.h"
#include "label.h"

std::shared_ptr<Code> make_while(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> body
);
