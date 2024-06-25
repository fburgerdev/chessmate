#include "gameserver.hpp"
#include "clparse.hpp"

using namespace Chessmate;
int32 main(int32 argc, const char* argv[]) {
    // parse command line arguments
    CLArgs commandline = parseCommandLine(argc, argv, { { 'p', CLArgument } });
    if (commandline.isValid) {
        const List<string>& positional = commandline.positional;
        if (positional.size()) {
            string action = positional[0];
            if (action != "joingame" && commandline.options.find('p') != commandline.options.end()) {
                std::cerr << "error: \"p\" is not a valid option for action \"" << action << "\"." << endl;
            }
            // creategame
            else if (action == "creategame") {
                if (positional.size() == 1) {
                    cout << createGame() << endl;
                    return EXIT_SUCCESS;
                }
                else {
                    std::cerr << "error: expects no additional arguments for action \"creategame\"." << endl;
                }
            }
            // joingame
            else if (action == "joingame") {
                if (positional.size() == 2) {
                    string gameid = positional[1];
                    try {
                        Player player = Player::None;
                        if (commandline.options.find('p') != commandline.options.end()) {
                            if (commandline.options.at('p') == "white") {
                                player = Player::White;
                            }
                            else if (commandline.options.at('p') == "black") {
                                player = Player::Black;
                            }
                            else {
                                std::cerr << "error: option \"p\" has no valid value." << endl;
                                return EXIT_FAILURE;
                            }
                        }
                        cout << joinGame(std::stoul(gameid), player) << endl;
                        return EXIT_SUCCESS;
                    }
                    catch (std::invalid_argument e) {
                        std::cerr << "error: \"gameid\" is not in a valid format." << endl;
                    }
                }
                else {
                    std::cerr << "error: expects 1 additional argument (gameid) for action \"joingame\"." << endl;
                }
            }
            // gamestate
            else if (action == "gamestate") {
                if (positional.size() == 2) {
                    try {
                        int32 gameID = std::stoul(positional[1]);
                        cout << getGameState(gameID, 0) << endl;
                        return EXIT_SUCCESS;
                    }
                    catch (std::invalid_argument e) {
                        std::cerr << "error: additional argument 1 \"gameid\" is not in a valid format." << endl;
                    }
                }
                else {
                    std::cerr << "error: expects 1 additional argument (gameid) for action \"gamestate\"." << endl;
                }
            }
            // domove
            else if (action == "domove") {
                if (positional.size() == 4) {
                    try {
                        int32 gameID = std::stoul(positional[1]);
                        try {
                            int32 playerID = std::stoul(positional[2]);
                            string move = positional[3];
                            cout << doMove(gameID, playerID, move) << endl;
                            return EXIT_SUCCESS;
                        }
                        catch (std::invalid_argument e) {
                            std::cerr << "error: additional argument 2 \"playerid\" is not in a valid format." << endl;
                        }
                    }
                    catch (std::invalid_argument e) {
                        std::cerr << "error: additional argument 1 \"gameid\" is not in a valid format" << endl;
                    }
                }
                else {
                    std::cerr << "error: expects 3 additional arguments (gameid, playerid, move(UCI Notation)) for action \"domove\"." << endl;
                }
            }
            else {
                std::cerr << "error: \"" << action << "\" is not a valid action." << endl;
            }
        }
        else {
            std::cerr << "error: no action specified." << endl;
        }
    }
    return EXIT_FAILURE;
}