#pragma once
#include "move.hpp"
#include "castle.hpp"

namespace Chessmate {
    // Board
    class Board {
    public:
        // constructor
        Board(const string& fen);
        Board(const Board& oldboard, const Move& move);

        // static
        static Direction PawnDirection(Player player);
        static Direction PawnStartingPosition(Player player);
        
        // fromFEN / toFEN
        void fromFEN(const string& fen);
        string toFEN() const;

        // Algebraic Notation (UCI Standard)
        Move fromAlgebraicNotation(string notation) const;
        
        // square
        // :: get / set
        const Piece& get(Square square) const;
        const Piece& get(Square square, Direction filedir, Direction rankdir) const;
        void set(Square square, Piece piece);
        // :: player / type
        Player player(Square square) const;
        PieceType type(Square square) const;
        // :: isEmpty
        bool isEmpty(Square square) const;
        bool isEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const;
        // :: isFriendly
        bool isFriendly(Square square) const;
        bool isFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const;
        // :: isEnemy
        bool isEnemy(Square square) const;
        bool isEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const;
        // :: inBoundsAndEquals
        bool inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const;

        // moves
        // :: getMoveSet
        List<Move> getMoveset() const;
        // :: getLegalMoves
        List<Move> getLegalMoves() const;
    
        // check
        // :: findPiece
        Square findPiece(Piece piece) const;
        // :: getAttackingSquares / isSquareAttackedBy
        List<Square> getAttackingSquares(Square square, Player player) const;
        bool isSquareAttackedBy(Square square, Player player) const;
        // :: canCaptureKing / isKingAttacked
        bool canCaptureKing() const;
        bool isKingAttacked() const;
        // :: inCheck / inCheckmate
        bool inCheck() const;
        bool inCheckmate() const;

        // draw
        // :: isDraw
        bool isDraw() const;
        // :: hasSamePositions (According to FIDE)
        bool hasSamePositions(const Board& other) const;

        // String
        string toString() const;

        // iterate
        auto begin();
        const auto begin() const;
        auto end();
        const auto end() const;

        // member
        Player active;
        CastleState castle;
        Square enPassant;
        uint32 halfMoves, fullMoves;
        Array<Piece, 64> squares;
    };
}