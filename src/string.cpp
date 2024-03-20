#include "string.hpp"

namespace Chessmate {
    // Contains
    bool contains(const string& source, char ch) {
        return source.find(ch) != string::npos;
    }
    // Split
    List<string> split(const string& source, const string& delimiter) {
        address start = 0;
        address end = string::npos;
        List<string> splitted;
        while ((end = source.find(delimiter, start)) != std::string::npos) {
            splitted.push_back(source.substr(start, end - start));
            start = end + delimiter.length();
        }
        splitted.push_back(source.substr(start));
        return splitted;
    }
}