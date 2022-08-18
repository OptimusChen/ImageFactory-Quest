#include "Utils/StringUtils.hpp" 


#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

namespace StringUtils {

    std::string removeTrailingZeros(int i) {
        std::string s = std::to_string(i);

        /*
            for some reason it just doesn't add zeros in this? idk man
            ¯\_(ツ)_/¯
        */

        return s;
    }

    std::vector<std::string> split(std::string s, char c) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(s);
        while (getline(ss, token, c)){
            tokens.push_back(token);
        }
    
        return tokens;
    }

}