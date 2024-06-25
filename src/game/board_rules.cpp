#include "board.hpp"
#include "parse.hpp"

namespace Chessmate {
    // directions
    // :: knight
    static const List<List<Direction>> KnightDirections = {
        { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 },
        { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }
    };
    // :: bishop
    static const List<List<Direction>> BishopDirections = { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };
    // :: rook
    static const List<List<Direction>> RookDirections = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
    // :: queen
    static const List<List<Direction>> QueenDirections = {
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 },
        { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 },
    };
    // :: king
    static const List<List<Direction>> KingDirections = {
        { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 },
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 },
    };
    // :: direction map
    static const Map<PieceType, List<List<Direction>>> DirectionMap = {
        { PieceType::Bishop, BishopDirections },
        { PieceType::Rook, RookDirections },
        { PieceType::Queen, QueenDirections },
    };

    // getMoveset
    List<Move> Board::getMoveset() const {
        List<Move> moveset;
        for (Square square = 0; square < 64; ++square) {
            // is player
            if (!isFriendly(square)) {
                continue;
            }

            // pawn
            if (type(square) == PieceType::Pawn) {
                // advance
                if (inBoundsAndEmpty(square, 0, PawnDirection(active))) {
                    // promote
                    if (getRank(square) == PawnStartingPosition(getEnemy(active))) {
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active), MoveFlag::PromoteQ);
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active), MoveFlag::PromoteR);
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active), MoveFlag::PromoteB);
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active), MoveFlag::PromoteN);
                    }
                    // normal
                    else {
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active));
                    }
                    // double advanec
                    if (getRank(square) == PawnStartingPosition(active) && isEmpty(square, 0, PawnDirection(active) * 2)) {
                        moveset.emplace_back(get(square), square, 0, PawnDirection(active) * 2, MoveFlag::DoubleAdvance);
                    }
                }
                // capture
                // :: right
                if (inBoundsAndEnemy(square, 1, PawnDirection(active))) {
                    // Promote
                    if (getRank(square) == PawnStartingPosition(getEnemy(active))) {
                        moveset.emplace_back(get(square), square, 1, PawnDirection(active), MoveFlag::PromoteQ);
                        moveset.emplace_back(get(square), square, 1, PawnDirection(active), MoveFlag::PromoteR);
                        moveset.emplace_back(get(square), square, 1, PawnDirection(active), MoveFlag::PromoteB);
                        moveset.emplace_back(get(square), square, 1, PawnDirection(active), MoveFlag::PromoteN);
                    }
                    // normal
                    else {
                        moveset.emplace_back(get(square), square, 1, PawnDirection(active));
                    }
                }
                // :: left
                if (inBoundsAndEnemy(square, -1, PawnDirection(active))) {
                    // promote
                    if (getRank(square) == PawnStartingPosition(getEnemy(active))) {
                        moveset.emplace_back(get(square), square, -1, PawnDirection(active), MoveFlag::PromoteQ);
                        moveset.emplace_back(get(square), square, -1, PawnDirection(active), MoveFlag::PromoteR);
                        moveset.emplace_back(get(square), square, -1, PawnDirection(active), MoveFlag::PromoteB);
                        moveset.emplace_back(get(square), square, -1, PawnDirection(active), MoveFlag::PromoteN);
                    }
                    // normal
                    else {
                        moveset.emplace_back(get(square), square, -1, PawnDirection(active));
                    }
                }
                // enPassant
                // :: right
                if (inBounds(square, 1, PawnDirection(active)) && addSquare(square, 1, PawnDirection(active)) == enPassant) {
                    moveset.emplace_back(get(square), square, enPassant);
                }
                // :: left
                else if (inBounds(square, -1, PawnDirection(active)) && addSquare(square, -1, PawnDirection(active)) == enPassant) {
                    moveset.emplace_back(get(square), square, enPassant);
                }
            }

            // knight
            else if (type(square) == PieceType::Knight) {
                for (const auto& direction : KnightDirections) {
                    if (inBounds(square, direction[0], direction[1]) && !isFriendly(square, direction[0], direction[1])) {
                        moveset.emplace_back(get(square), square, direction[0], direction[1]);
                    }
                }
            }
            
            // bishop / rook / queen
            else if (type(square) == PieceType::Bishop || type(square) == PieceType::Rook || type(square) == PieceType::Queen) {
                for (const auto& direction : DirectionMap.at(type(square))) {
                    for (int32 step = 1; step < 8; ++step) {
                        if (inBounds(square, direction[0] * step, direction[1] * step)) {
                            if (isEmpty(square, direction[0] * step, direction[1] * step)) {
                                moveset.emplace_back(get(square), square, direction[0] * step, direction[1] * step);
                            }
                            else if (isEnemy(square, direction[0] * step, direction[1] * step)) {
                                moveset.emplace_back(get(square), square, direction[0] * step, direction[1] * step);
                                break;
                            }
                            else {
                                break;
                            }
                        }
                        else {
                            break;
                        }
                    }
                }
            }

            // king
            else if (type(square) == PieceType::King) {
                // normal
                for (const auto& direction : KingDirections) {
                    if (inBounds(square, direction[0], direction[1]) && !isFriendly(square, direction[0], direction[1])) {
                        moveset.emplace_back(get(square), square, direction[0], direction[1]);
                    }
                }

                // castle
                if (!inCheck()) {
                    // white
                    if (active == Player::White) {
                        // kingside
                        if (castle[Castle::WhiteKingside] && isEmpty(Parse::toSquare("f1")) && isEmpty(Parse::toSquare("g1"))) {
                            if (!isSquareAttackedBy(Parse::toSquare("f1"), getEnemy(active))) {
                                moveset.emplace_back(get(square), square, Parse::toSquare("g1"), MoveFlag::CastleK);
                            }
                        }
                        // queenside
                        if (castle[Castle::WhiteQueenside] && isEmpty(Parse::toSquare("d1")) && isEmpty(Parse::toSquare("c1"))  && isEmpty(Parse::toSquare("b1"))) {
                            if (!isSquareAttackedBy(Parse::toSquare("d1"), getEnemy(active))) {
                                moveset.emplace_back(get(square), square, Parse::toSquare("c1"), MoveFlag::CastleQ);
                            }
                        }
                    }
                    // black
                    else {
                        // kingside
                        if (castle[Castle::BlackKingside] && isEmpty(Parse::toSquare("f8")) && isEmpty(Parse::toSquare("g8"))) {
                            if (!isSquareAttackedBy(Parse::toSquare("f8"), getEnemy(active))) {
                                moveset.emplace_back(get(square), square, Parse::toSquare("g8"), MoveFlag::CastleK);
                            }
                        }
                        // queenside
                        if (castle[Castle::BlackQueenside] && isEmpty(Parse::toSquare("d8")) && isEmpty(Parse::toSquare("c8"))  && isEmpty(Parse::toSquare("b8"))) {
                            if (!isSquareAttackedBy(Parse::toSquare("d8"), getEnemy(active))) {
                                moveset.emplace_back(get(square), square, Parse::toSquare("c8"), MoveFlag::CastleQ);
                            }
                        }
                    }
                }
            }
        }
        return moveset;
    }
    // getLegalMoves
    List<Move> Board::getLegalMoves() const {
        List<Move> legalmoves;
        for (const Move& move : getMoveset()) {
            Board newboard(*this, move);
            if (!newboard.canCaptureKing()) {
                legalmoves.emplace_back(move);
            }
        }
        return legalmoves;
    }

    // check
    // :: findPiece
    Square Board::Board::findPiece(Piece piece) const {
        for (Square square = 0; square < 64; ++square) {
            if (get(square) == piece) {    
                return square;
            }
        }
        return NoSquare;
    }
    // :: getAttackingSquares
    List<Square> Board::getAttackingSquares(Square square, Player attackingPlayer) const {
        List<Square> squares;

        // pawn
        // :: normal
        if (inBoundsAndEquals(square, 1, -PawnDirection(attackingPlayer), Piece(PieceType::Pawn, attackingPlayer))) {
            squares.push_back(addSquare(square, 1, -PawnDirection(attackingPlayer)));
        }
        if (inBoundsAndEquals(square, -1, -PawnDirection(attackingPlayer), Piece(PieceType::Pawn, attackingPlayer))) {
            squares.push_back(addSquare(square, -1, -PawnDirection(attackingPlayer)));
        }
        // :: enPassant
        if (attackingPlayer == active && square == addSquare(enPassant, 0, PawnDirection(getEnemy(attackingPlayer)))) {
            if (inBoundsAndEquals(square, 1, 0, Piece(PieceType::Pawn, attackingPlayer))) {
                squares.push_back(addSquare(square, 1, 0));
            }
            if (inBoundsAndEquals(square, -1, 0, Piece(PieceType::Pawn, attackingPlayer))) {
                squares.push_back(addSquare(square, -1, 0));
            }
        }

        // knight
        for (const auto& direction : KnightDirections) {
            if (inBoundsAndEquals(square, direction[0], direction[1], Piece(PieceType::Knight, attackingPlayer))) {
                squares.push_back(addSquare(square, direction[0], direction[1]));
            }
        }

        // bishop / rook / queen
        for (const auto& [type, directions] : DirectionMap) {
            for (const auto& direction : directions) {
                for (int32 step = 1; step < 8; ++step) {
                    if (inBounds(square, direction[0] * step, direction[1] * step)) {
                        if (isEmpty(square, direction[0] * step, direction[1] * step)) {
                            continue;
                        }
                        else {
                            if (get(square, direction[0] * step, direction[1] * step) == Piece(type, attackingPlayer)) {
                                squares.push_back(addSquare(square, direction[0] * step, direction[1] * step));
                            }
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }

        // king
        for (const auto& direction : KingDirections) {
            if (inBoundsAndEquals(square, direction[0], direction[1], Piece(PieceType::King, attackingPlayer))) {
                squares.push_back(addSquare(square, direction[0], direction[1]));
            }
        }

        return squares;
    }
    // :: isSquareAttackedBy
    bool Board::isSquareAttackedBy(Square square, Player attackingPlayer) const {
        return getAttackingSquares(square, attackingPlayer).size();
    }
    // :: canCaptureKing / isKingAttacked
    bool Board::canCaptureKing() const {
        return isSquareAttackedBy(findPiece(Piece(PieceType::King, getEnemy(active))), active);
    }
    bool Board::isKingAttacked() const {
        return isSquareAttackedBy(findPiece(Piece(PieceType::King, active)), getEnemy(active));
    }
    // :: inCheck / inCheckmate
    bool Board::inCheck() const {
        return isKingAttacked();
    }
    bool Board::inCheckmate() const {
        return inCheck() && getLegalMoves().size() == 0;
    }

    // draw
    // :: isDraw
    bool Board::isDraw() const {
        return !inCheck() && getLegalMoves().size() == 0;
    } 
    // :: hasSamePositions (According to FIDE)
    bool Board::hasSamePositions(const Board& other) const {
        // same player
        if (active != other.active) {
            return false;
        }

        // same pieces
        for (Square square = 0; square < 64; ++square) {
            if (get(square) != other.get(square)) {
                return false;
            }
        }

        // same moves (NOTE: move order is stable)
        List<Move> legalmoves = getLegalMoves();
        List<Move> otherlegalmoves = other.getLegalMoves();
        if (legalmoves.size() != otherlegalmoves.size()) {
            return false;
        }
        for (address i = 0; i < legalmoves.size(); ++i) {
            if (legalmoves[i] != otherlegalmoves[i]) {
                return false;
            }
        }
        
        return true;
    }
}