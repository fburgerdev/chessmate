#pragma once
#include "move.hpp"
#include "castle.hpp"

namespace Chessmate {
    // Board
    class Board {
    public:
        // Constructor
        Board(const string& fen);
        Board(const Board& oldboard, const Move& move);
        // FEN
        void fromFEN(const string& fen);
        string toFEN() const;
        // Square
        // Square :: Get / Set
        const Piece& get(Square square) const;
        const Piece& get(Square square, Direction filedir, Direction rankdir) const;
        void set(Square square, Piece piece);
        // Square :: Player / Type
        Player player(Square square) const;
        PieceType type(Square square) const;
        // Square :: IsEmpty
        bool isEmpty(Square square) const;
        bool isEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsFriendly
        bool isFriendly(Square square) const;
        bool isFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsEnemy
        bool isEnemy(Square square) const;
        bool isEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const;
        // Square :: InBoundsAndEquals
        bool inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const;
        // GetMoveset
        List<Move> getMoveset() const;
        // FindPiece
        Square findPiece(Piece piece) const;
        // IsSquareAttackedBy
        bool isSquareAttackedBy(Square square, Player player) const;
        // LegalMoves
        List<Move> getLegalMoves() const;
        // CanCaptureKing
        bool canCaptureKing() const;
        // IsKingAttacked
        bool isKingAttacked() const;
        // InCheck / InCheckmate
        bool inCheck() const;
        bool inCheckmate() const;
        // ToString
        string toString() const;
        // HasSamePositions (According to FIDE)
        bool hasSamePositions(const Board& other) const;
        // CanClaimDraw
        bool canClaimDraw() const;
        // Iterate
        auto begin();
        const auto begin() const;
        auto end();
        const auto end() const;
        // Member
        Player active;
        CastleState castle;
        Square enpassant;
        uint32 halfmove, fullmove;
        Array<Piece, 64> squares;
    };
}