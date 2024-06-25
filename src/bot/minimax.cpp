#include "minimax.hpp"
#include "evaluate.hpp"

namespace Chessmate {
    // MinimaxResult
    // :: constructor
    MinimaxResult::MinimaxResult(Move move, float eval)
        : move(move), eval(eval) {}

    // :: compare
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

    // minimize
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
    // maximize
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
    // minimax
    MinimaxResult minimax(const Board& board, uint32 depth) {
        return maximize(board, depth, -1e30, +1e30);
    }
}