#include "gameserver.hpp"
#include "commandline.hpp"
#include "string.hpp"
#include <iostream> // std::cout, std::cerr

using namespace Chessmate;
int32 main(int32 argc, const char* argv[]) {
    CLArgs commandline = parseCommandLine(argc, argv, { { 'p', CLArgument } });
    if (commandline.isvalid) {
        const List<string>& positional = commandline.positional;
        if (positional.size()) {
            string action = positional[0];
            if (action != "joingame" && commandline.options.find('p') != commandline.options.end()) {
                std::cerr << "error: \"p\" is not a valid option for action \"" << action << "\"." << std::endl;
            }
            else if (action == "creategame") {
                if (positional.size() == 1) {
                    std::cout << createGame() << std::endl;
                    return EXIT_SUCCESS;
                }
                else {
                    std::cerr << "error: expects no additional arguments for action \"creategame\"." << std::endl;
                }
            }
            else if (action == "joingame") {
                if (positional.size() == 2) {
                    string gameid = positional[1];
                    if (isDigitSeq(gameid)) {
                        Player player = Player::None;
                        if (commandline.options.find('p') != commandline.options.end()) {
                            if (commandline.options.at('p') == "white") {
                                player = Player::White;
                            }
                            else if (commandline.options.at('p') == "black") {
                                player = Player::Black;
                            }
                            else {
                                std::cerr << "error: option \"p\" has no valid value." << std::endl;
                                return EXIT_FAILURE;
                            }
                        }
                        std::cout << joinGame(std::stoul(gameid), player) << std::endl;
                        return EXIT_SUCCESS;
                    }
                    else {
                        std::cerr << "error: \"gameid\" is not in a valid format." << std::endl;
                    }
                }
                else {
                    std::cerr << "error: expects 1 additional argument (gameid) for action \"joingame\"." << std::endl;
                }
            }
            else if (action == "gamestate") {
                if (positional.size() == 2) {
                    string gameid = positional[1];
                    if (isDigitSeq(gameid)) {
                        std::cout << getGameState(std::stoul(gameid, 0)) << std::endl;
                        return EXIT_SUCCESS;
                    }
                    else {
                        std::cerr << "error: additional argument 1 \"gameid\" is not in a valid format." << std::endl;
                    }
                }
                else {
                    std::cerr << "error: expects 1 additional argument (gameid) for action \"gamestate\"." << std::endl;
                }
            }
            else if (action == "domove") {
                if (positional.size() == 4) {
                    string gameid = positional[1];
                    if (isDigitSeq(gameid)) {
                        string playerid = positional[2];
                        if (isDigitSeq(playerid)) {
                            string move = positional[3];
                            std::cout << doMove(std::stoul(gameid), std::stoul(playerid), move) << std::endl;
                            return EXIT_SUCCESS;
                        }
                        else {
                            std::cerr << "error: additional argument 2 \"playerid\" is not in a valid format." << std::endl;
                        }
                    }
                    else {
                        std::cerr << "error: additional argument 1 \"gameid\" is not in a valid format" << std::endl;
                    }
                }
                else {
                    std::cerr << "error: expects 3 additional arguments (gameid, playerid, move(UCI Notation)) for action \"domove\"." << std::endl;
                }
            }
            else {
                std::cerr << "error: \"" << action << "\" is not a valid action." << std::endl;
            }
        }
        else {
            std::cerr << "error: no action specified." << std::endl;
        }
    }
    return EXIT_FAILURE;
}