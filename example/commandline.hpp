#pragma once
#include "common.hpp"
#include <iostream> // std::cerr

namespace Chessmate {
    enum CLOptionType {
        CLFlag, CLArgument, CLOptArgument
    };
    struct CLArgs {
        List<string> positional;
        Map<char, string> options;
        bool isvalid;
    };
    CLArgs parseCommandLine(int32 argc, const char* argv[], const Map<char, CLOptionType>& options) {
        // Args
        List<string> args;
        for (int32 i = 1; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }
        // Options
        Map<char, string> parsedoptions;
        for (address i = 0; i < args.size(); ++i) {
            // --
            if (args[i] == "--") {
                return { List<string>(args.begin() + i + 1, args.end()), parsedoptions, true };
            }
            // optional
            else if (args[i].at(0) == '-') {
                if (args[i].length() == 1) {
                    std::cerr << "error: \"-\" is not a valid positional or option." << std::endl;
                    return { {}, {}, false };
                }
                address index = 1;
                while (index < args[i].length()) {
                    char alpha = std::tolower(args[i].at(index));
                    if (options.find(alpha) == options.end()) {
                        std::cerr << "error: \"" << alpha << "\" is not a valid option." << std::endl;
                        return { {}, {}, false };
                    }
                    if (options.at(alpha) == CLFlag) {
                        parsedoptions.emplace(alpha, string());
                        index += 1;
                    }
                    else if (options.at(alpha) == CLArgument) {
                        if (index + 1 == args[i].length()) {
                            if (i + 1 < args.size()) {
                                parsedoptions.emplace(alpha, args[i + 1]);
                                i += 1;
                                break;
                            }
                            else {
                                std::cerr << "error: \"" << alpha << "\" expects to have an argument provided." << std::endl;
                                return { {}, {}, false };
                            }
                        }
                        else {
                            parsedoptions.emplace(alpha, args[i].substr(index + 1));
                            break;
                        }
                    }
                    else if (options.at(alpha) == CLOptArgument) {
                        if (index + 1 < args[i].length()) {
                            parsedoptions.emplace(alpha, args[i].substr(index + 1));
                            break;
                        }
                        else {
                            parsedoptions.emplace(alpha, string());
                            break;
                        }
                    }
                }
            }
            // positional
            else {
                return { List<string>(args.begin() + i, args.end()), parsedoptions, true };
            }
        }
        return { {}, parsedoptions, true };
    }
    template<typename T>
    bool matchesList(const List<T>& list1, const List<T>& list2) {
        if (list1.size() == list2.size()) {
            for (address i = 0; i < list1.size(); ++i) {
                if (list1[i] != list2[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
}