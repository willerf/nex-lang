
#pragma once

#include <functional>

#include "assembly.h"
#include "block.h"
#include "code.h"
#include "label.h"

std::shared_ptr<Code> make_while(
    std::shared_ptr<Code> e1,
    std::function<std::shared_ptr<Code>(std::shared_ptr<Label>)> comp,
    std::shared_ptr<Code> e2,
    std::shared_ptr<Code> body
);


