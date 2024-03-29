#pragma once
#include "common.hpp"

namespace Chessmate {
    // Contains
    inline bool contains(const string& source, char ch) {
        return source.find(ch) != string::npos;
    }
    // Split
    inline List<string> split(const string& source, const string& delimiter) {
        address start = 0;
        address end = string::npos;
        List<string> splitted;
        while ((end = source.find(delimiter, start)) != std::string::npos) {
            if (end == start) {
                start += 1;
            }
            else {
                splitted.push_back(source.substr(start, end - start));
                start = end + delimiter.length();
            }
        }
        splitted.push_back(source.substr(start));
        return splitted;
    }
    // IsDigitSeq
    inline bool isDigitSeq(const string &str) {
        if (str.empty()) {
            return false;
        }
        else {
            for (char ch : str) {
                if (!std::isdigit(ch)) {
                    return false;
                }
            }
            return true;
        }
    }
}