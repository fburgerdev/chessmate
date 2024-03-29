#include "gameserver.hpp"
#include <fstream> // std::ifstream, std::ofstream
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <functional>

namespace Chessmate {
    // RandomID
    static uint32 createRandomID() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1000,9999);
        return dist(rng);
    }
    // DateAndTime
    static string getCurrentDateAndTime() {
        std::time_t now = time(0);
        std::tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }
    // Filepath
    static string s_Directory = "./";
    string getFilepath(int32 gameid) {
        return s_Directory + std::to_string(gameid) + ".game";
    }
    string createGame() {
        int32 gameid = createRandomID();
        nlohmann::json json;
        json["creation-date"] = getCurrentDateAndTime();
        json["player-white"] = 0;
        json["player-black"] = 0;
        json["gamestate"]["initial-fen"] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        json["gamestate"]["current-fen"] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        json["gamestate"]["moves"] = nlohmann::json::array();
        std::ofstream(getFilepath(gameid)) << std::setw(4) << json;
        nlohmann::json response;
        response["success"] = true;
        response["gameid"] = gameid;
        return nlohmann::to_string(response);
    }
    string openGame(int32 gameid, std::function<string(nlohmann::json)> callback) {
        std::ifstream stream(getFilepath(gameid));
        if (stream.is_open()) {
            return callback(nlohmann::json::parse(stream));
        }
        nlohmann::json response;
        response["success"] = false;
        response["error"] = "game doesn't exist";
        return nlohmann::to_string(response); 
    }
    string joinGame(int32 gameid, Player player) {
        return openGame(gameid, [&](nlohmann::json json) {
            nlohmann::json response;
            if (player == Player::None) {
                if (json["player-white"] == 0) {
                    return joinGame(gameid, Player::White);
                }
                else if (json["player-black"] == 0) {
                    return joinGame(gameid, Player::Black);
                }
                else {
                    response["success"] = false;
                    response["error"] = "game is full"; 
                }
            }
            else {
                string playerkey = (player == Player::White ? "player-white" : "player-black");
                if (json[playerkey] == 0) {
                    int32 playerid = createRandomID();
                    json[playerkey] = playerid;
                    std::ofstream(getFilepath(gameid)) << std::setw(4) << json;
                    response["success"] = true;
                    response["playerid"] = playerid;
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
    string doMove(int32 gameid, int32 playerid, string movestring) {
        return openGame(gameid, [&](nlohmann::json json) {
            nlohmann::json response;
            Board board(json["gamestate"]["current-fen"]);
            if (0 < json["player-white"] && 0 <json["player-black"]) {
                string playerkey = (board.active == Player::White ? "player-white" : "player-black");
                if (json[playerkey] == playerid) {
                    Move move = board.fromAlgebraicNotation(movestring);
                    if (move.isValid()) {
                        json["gamestate"]["current-fen"] = Board(board, move).toFEN();
                        json["gamestate"]["moves"].push_back(move.toAlgebraicNotation());
                        std::ofstream(getFilepath(gameid)) << std::setw(4) << json;
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
    string getGameState(int32 gameid, int32 depth) {
        return openGame(gameid, [&](nlohmann::json json) {
            nlohmann::json response;
            Board board(json["gamestate"]["current-fen"]);
            // Success
            response["success"] = true;
            // FEN
            response["gamestate"]["fen"] = board.toFEN();
            // Active
            response["gamestate"]["active"] = fromPlayer(board.active);
            // Castle
            response["gamestate"]["castle"] = nlohmann::json::array();
            if (board.castle[Castle::WhiteKingside]) {
                response["gamestate"]["castle"].push_back("K");
            }
            if (board.castle[Castle::WhiteQueenside]) {
                response["gamestate"]["castle"].push_back("Q");
            }
            if (board.castle[Castle::BlackKingside]) {
                response["gamestate"]["castle"].push_back("k");
            }
            if (board.castle[Castle::BlackQueenside]) {
                response["gamestate"]["castle"].push_back("q");
            }
            // Halfmove
            response["gamestate"]["halfmove"] = board.halfmove;
            // Fullmove
            response["gamestate"]["halfmove"] = board.fullmove;
            // InCheck
            response["gamestate"]["incheck"] = board.isKingAttacked();
            // State
            if (json["player-white"] == 0 || json["player-black"] == 0) {
                response["gamestate"]["status"] = "prepare";
            }
            else {
                if (board.getLegalMoves().size()) {
                    response["gamestate"]["status"] =  "ingame";
                }
                else {
                    if (board.inCheck()) {
                        response["gamestate"]["status"] = "checkmate";
                    }
                    else {
                        response["gamestate"]["status"] = "draw";
                    }
                }
            }
            // Movelist
            response["gamestate"]["movelist"] = nlohmann::json::array();
            for (const string& move : json["gamestate"]["moves"]) {
                response["gamestate"]["movelist"].emplace_back(move);
            }
            // Legalmoves
            response["gamestate"]["legalmoves"] = nlohmann::json::array();
            for (const Move& move : board.getLegalMoves()) {
                response["gamestate"]["legalmoves"].push_back(move.toAlgebraicNotation());
            }
            return nlohmann::to_string(response);
        });
    }
}