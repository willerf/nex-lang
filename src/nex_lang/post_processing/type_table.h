
#pragma once

#include <map>
#include <memory>
#include <string>

#include "nl_type.h"

using TypeTable = std::map<std::string, std::shared_ptr<NLType>>;
