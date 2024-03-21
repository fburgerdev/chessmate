#include "commandline.hpp"
#include "formatresults.hpp"
#include <iostream>

using namespace Chessmate;
void gameloop() {
    static Game game;
    std::cout << game.getBoard().toString() << std::endl;
    std::cout << "LegalMoves:" << std::endl;
    List<Move> legalmoves = game.getBoard().getLegalMoves();
    for (address i = 0; i < legalmoves.size(); ++i) {
        std::cout << i << ": " << legalmoves[i].toAlgebraicNotation() << std::endl;
    }
    int moveindex;
    std::cin >> moveindex;
    game.doMove(legalmoves[moveindex]);
    gameloop();
}
int32 main(int32 argc, const char* argv[]) {
    // Game game = fromPGN("../chessgames/game1.pgn");
    // for (const Board& board : game.getBoardList()) {
    //     std::cout << board.toString() << std::endl;
    //     int32 input;
    //     std::cin >> input;
    // }
    CLArgs commandline = parseCommandLine(argc, argv, { { 'd', CLArgument } });
    if (commandline.isvalid) {
        if (commandline.positional.size() == 0) {
            std::cerr << "error: provide board position as FEN string" << std::endl;
            return EXIT_FAILURE;
        }
        else if (1 < commandline.positional.size()) {
            std::cerr << "error: only 1 positional argument expected" << std::endl;
            return EXIT_FAILURE;
        }
        string fen = commandline.positional[0];
        uint32 depth = 0;
        if (commandline.options.find('d') != commandline.options.end()) {
            depth = std::stoul(commandline.options.at('d'));
        }
        Game game(fen);
        std::cout << formatResults(game, depth) << std::endl;
    }
    else {
        return EXIT_FAILURE;
    }
    // gameloop();
    return EXIT_SUCCESS;
}