#include "gameserver.hpp"
#include "game/parse.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>

namespace Chessmate {
    // createRandomID
    static uint32 createRandomID() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1000,9999);
        return dist(rng);
    }

    // getCurrentDateAndTime
    static string getCurrentDateAndTime() {
        std::time_t now = time(0);
        std::tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }

    // filepath
    static string s_Directory = "./";
    // :: getFilepath
    string getFilepath(int32 gameid) {
        return s_Directory + std::to_string(gameid) + ".game";
    }

    // openGame
    static string openGame(int32 gameID, function<string(nlohmann::json)> callback) {
        std::ifstream stream(getFilepath(gameID));
        if (stream.is_open()) {
            return callback(nlohmann::json::parse(stream));
        }
        nlohmann::json response;
        response["success"] = false;
        response["error"] = "game doesn't exist";
        return nlohmann::to_string(response); 
    }

    // createGame
    string createGame() {
        int32 gameID = createRandomID();
        nlohmann::json json;
        json["creationDate"] = getCurrentDateAndTime();
        json["playerWhite"] = 0;
        json["playerBlack"] = 0;
        json["gameState"]["initialFEN"] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        json["gameState"]["currentFEN"] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        json["gameState"]["moves"] = nlohmann::json::array();
        std::ofstream(getFilepath(gameID)) << std::setw(4) << json;
        nlohmann::json response;
        response["success"] = true;
        response["gameID"] = gameID;
        return nlohmann::to_string(response);
    }

    // joinGame
    string joinGame(int32 gameID, Player player) {
        return openGame(gameID, [&](nlohmann::json json) {
            nlohmann::json response;
            if (player == Player::None) {
                if (json["playerWhite"] == 0) {
                    return joinGame(gameID, Player::White);
                }
                else if (json["playerBlack"] == 0) {
                    return joinGame(gameID, Player::Black);
                }
                else {
                    response["success"] = false;
                    response["error"] = "game is full"; 
                }
            }
            else {
                string playerKey = (player == Player::White ? "playerWhite" : "playerBlack");
                if (json[playerKey] == 0) {
                    int32 playerID = createRandomID();
                    json[playerKey] = playerID;
                    std::ofstream(getFilepath(gameID)) << std::setw(4) << json;
                    response["success"] = true;
                    response["playerID"] = playerID;
                    response["player"] = (player == Player::White ? "white" : "black");
                }
                else {
                    response["success"] = false;
                    response["error"] = "player already joined";
                }
            }
            return nlohmann::to_string(response);
        });
    }

    // doMove
    string doMove(int32 gameID, int32 playerID, string moveString) {
        return openGame(gameID, [&](nlohmann::json json) {
            nlohmann::json response;
            Board board(json["gameState"]["currentFEN"]);
            if (0 < json["playerWhite"] && 0 < json["playerBlack"]) {
                string playerKey = (board.active == Player::White ? "playerWhite" : "playerBlack");
                if (json[playerKey] == playerID) {
                    Move move = board.fromAlgebraicNotation(moveString);
                    if (move.isValid()) {
                        json["gameState"]["currentFEN"] = Board(board, move).toFEN();
                        json["gameState"]["moves"].push_back(move.toAlgebraicNotation());
                        std::ofstream(getFilepath(gameID)) << std::setw(4) << json;
                        response["success"] = true;
                    }
                    else {
                        response["success"] = false;
                        response["error"] = "invalid move";
                    }
                }
                else {
                    response["success"] = false;
                    response["error"] = "wrong playerid provided";
                }
            }
            else {
                response["success"] = false;
                response["error"] = "game not full";
            }
            return nlohmann::to_string(response);
        });
    }

    // getGameState
    string getGameState(int32 gameID, int32 depth) {
        return openGame(gameID, [&](nlohmann::json json) {
            nlohmann::json response;
            Board board(json["gameState"]["currentFEN"]);
            
            // success
            response["success"] = true;
            
            // FEN
            response["gameState"]["FEN"] = board.toFEN();
            
            // active
            response["gameState"]["active"] = Parse::fromPlayer(board.active);

            // castle
            response["gameState"]["castle"] = nlohmann::json::array();
            if (board.castle[Castle::WhiteKingside]) {
                response["gameState"]["castle"].push_back("K");
            }
            if (board.castle[Castle::WhiteQueenside]) {
                response["gameState"]["castle"].push_back("Q");
            }
            if (board.castle[Castle::BlackKingside]) {
                response["gameState"]["castle"].push_back("k");
            }
            if (board.castle[Castle::BlackQueenside]) {
                response["gameState"]["castle"].push_back("q");
            }
            
            // halfMoves
            response["gameState"]["halfMoves"] = board.halfMoves;
            // fullMoves
            response["gameState"]["halfMoves"] = board.fullMoves;
            
            // inCheck
            response["gameState"]["inCheck"] = board.isKingAttacked();
            // state
            if (json["playerWhite"] == 0 || json["playerBlack"] == 0) {
                response["gameState"]["status"] = "prepare";
            }
            else {
                if (board.getLegalMoves().size()) {
                    response["gameState"]["status"] =  "inGame";
                }
                else {
                    if (board.inCheck()) {
                        response["gameState"]["status"] = "checkmate";
                    }
                    else {
                        response["gameState"]["status"] = "draw";
                    }
                }
            }

            // moveList
            response["gameState"]["moveList"] = nlohmann::json::array();
            for (const string& move : json["gameState"]["moves"]) {
                response["gameState"]["moveList"].emplace_back(move);
            }
            // legalMoves
            response["gameState"]["legalMoves"] = nlohmann::json::array();
            for (const Move& move : board.getLegalMoves()) {
                response["gameState"]["legalMoves"].push_back(move.toAlgebraicNotation());
            }

            return nlohmann::to_string(response);
        });
    }
}