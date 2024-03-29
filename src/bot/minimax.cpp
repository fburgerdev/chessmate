#include "minimax.hpp"
#include "evaluate.hpp"

namespace Chessmate {
    // MinimaxResult
    // MinimaxResult :: Constructor
    MinimaxResult::MinimaxResult(Move move, float eval)
        : move(move), eval(eval) {}
    // MinimaxResult :: Compare
    bool MinimaxResult::operator<(const MinimaxResult& other) const {
        return eval < other.eval;
    }
    bool MinimaxResult::operator<=(const MinimaxResult& other) const {
        return eval <= other.eval;
    }
    bool MinimaxResult::operator>(const MinimaxResult& other) const {
        return eval > other.eval;
    }
    bool MinimaxResult::operator>=(const MinimaxResult& other) const {
        return eval >= other.eval;
    }
    // Minimize / Maximize
    MinimaxResult minimize(const Board& board, uint32 depth, float alpha, float beta) {
        MinimaxResult result = { Move(), +1e30 };
        if (board.inCheckmate()) {
            return result;
        }
        if (depth == 1) {
            for (Move move : board.getLegalMoves()) {
                result = std::min(MinimaxResult(move, evaluateBoard(Board(board, move))), result);
            }
        }
        else {
            for (Move move : board.getLegalMoves()) {
                result = std::min(maximize(Board(board, move), depth - 1, alpha, beta), result);
                if (result.eval < alpha) {
                    return result;
                }
                beta = std::min(result.eval, beta);
            }
        }
        return result;
    }
    MinimaxResult maximize(const Board& board, uint32 depth, float alpha, float beta) {
        MinimaxResult result = { Move(), -1e30 };
        if (board.inCheckmate()) {
            return result;
        }
        if (depth == 1) {
            for (Move move : board.getLegalMoves()) {
                result = std::max(MinimaxResult(move, -evaluateBoard(Board(board, move))), result);
            }
        }
        else {
            for (Move move : board.getLegalMoves()) {
                result = std::max(minimize(Board(board, move), depth - 1, alpha, beta), result);
                if (beta < result.eval) {
                    return result;
                }
                alpha = std::max(result.eval, alpha);
            }
        }
        return result;
    }
    // Minimax
    MinimaxResult minimax(const Board& board, uint32 depth) {
        return maximize(board, depth, -1e30, 1e30);
    }
}