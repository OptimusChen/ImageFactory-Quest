#pragma once

#include "beatsaber-hook/shared/utils/typedefs-string.hpp"

namespace StringUtils {
    std::string removeTrailingZeros(int s);
    std::vector<std::string> split(std::string s, char c);
}