#pragma once
#include "game/board.hpp"

namespace Chessmate {
    // createGame
    string createGame();
    // joinGame
    string joinGame(int32 gameID, Player player);
    // doMove
    string doMove(int32 gameID, int32 playerID, string moveString);
    // getGameState
    string getGameState(int32 gameID, int32 depth = 0);
}