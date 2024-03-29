#pragma once
#include "game/board.hpp"

namespace Chessmate {
    string createGame();
    string joinGame(int32 gameid, Player player);
    string doMove(int32 gameid, int32 playerid, string movestring);
    string getGameState(int32 gameid, int32 depth = 0);
}