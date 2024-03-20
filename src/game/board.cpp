#include "board.hpp"
#include "string.hpp"
#include <iostream> // Debug

namespace Chessmate {
    // Constants
    // Constants :: PawnDirection / PawnStartingPos
    static const PlayerValue<int32> pawndirection = { -1, 1 };
    static const PlayerValue<int32> pawnstartingpos = { 6, 1 };
    // Constants :: Direction
    static const List<List<Direction>> knightdirections = {
        { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 },
        { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
    };
    static const List<List<Direction>> bishopdirections = { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };
    static const List<List<Direction>> rookdirections = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
    static const List<List<Direction>> queendirections = {
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 },
        { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 },
    };
    static const List<List<Direction>> kingdirections = {
        { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 },
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 },
     };
    static const Map<PieceType, List<List<Direction>>> directionmap = {
        { PieceType::Knight, knightdirections },
        { PieceType::Bishop, bishopdirections },
        { PieceType::Rook, rookdirections },
        { PieceType::Queen, queendirections },
    };
    // Board
    // Board :: Constructor
    Board::Board(const string& fen) {
        fromFEN(fen);
    }
    Board::Board(const Board& oldboard, const Move& move)
        : Board(oldboard) {
        // CastleState
        if (type(move.origin) == PieceType::King) {
            if (active == Player::White) {
                castle[Castle::WhiteKingside] = false;
                castle[Castle::WhiteQueenside] = false;
            }
            else {
                castle[Castle::BlackKingside] = false;
                castle[Castle::BlackQueenside] = false;
            }
        }
        else if (move.target == toSquare("h1")) {
            castle[Castle::WhiteKingside] = false;
        }
        else if (move.target == toSquare("a1")) {
            castle[Castle::WhiteQueenside] = false;
        }
        else if (move.target == toSquare("h8")) {
            castle[Castle::BlackKingside] = false;
        }
        else if (move.target == toSquare("a8")) {
            castle[Castle::BlackQueenside] = false;
        }
        // Halfmove
        if (type(move.origin) == PieceType::Pawn || isEnemy(move.target)) {
            halfmove = 0;
        }
        else {
            halfmove += 1;
        }
        // Piece
        if (move.flag == MoveFlag::PromoteN) {
            set(move.target, Piece(PieceType::Knight, active));
        }
        if (move.flag == MoveFlag::PromoteB) {
            set(move.target, Piece(PieceType::Bishop, active));
        }
        if (move.flag == MoveFlag::PromoteR) {
            set(move.target, Piece(PieceType::Rook, active));
        }
        if (move.flag == MoveFlag::PromoteQ) {
            set(move.target, Piece(PieceType::Queen, active));
        }
        else {
            set(move.target, get(move.origin));
        }
        if (move.flag == MoveFlag::CastleStateK) {
            set((active == Player::White) ? toSquare("f1") : toSquare("f8"), Piece(PieceType::Rook, active));
            set((active == Player::White) ? toSquare("h1") : toSquare("h8"), Piece());
        }
        else if (move.flag == MoveFlag::CastleStateQ) {
            set((active == Player::White) ? toSquare("d1") : toSquare("d8"), Piece(PieceType::Rook, active));
            set((active == Player::White) ? toSquare("a1") : toSquare("a8"), Piece());
        }
        set(move.origin, Piece());
        // EnPassant
        if (move.flag == MoveFlag::EnPassant) {
            set(enpassant, Piece());
        }
        // DoubleAdvance
        if (move.flag == MoveFlag::DoubleAdvance) {
            enpassant = addSquare(move.origin, getFile(move.target - move.origin), 0);
        }
        else {
            enpassant = InvalidSquare;
        }
        // Fullmove
        if (active == Player::Black) {
            fullmove += 1;
        }
        // Active
        active = getEnemy(active);
    }
    // Board :: FEN
    void Board::fromFEN(const string& fen) {
        // TODO: Error Handling
        List<string> splitted = split(fen, " ");
        // Board
        int32 square = 0;
        for (char ch : splitted[0]) {
            if (std::isalpha(ch)) {
                set(square++, toPiece(ch));
            }
            else if (std::isdigit(ch)) {
                square += ch - '0';
            }
        }
        // Active
        active = (splitted[1] == "w" ? Player::White : (splitted[1] == "b" ? Player::Black : Player::None));
        // CastleState
        castle = CastleState(contains(splitted[2], 'K'), contains(splitted[2], 'Q'), contains(splitted[2], 'k'), contains(splitted[2], 'q'));
        // EnPassant
        if (splitted[3] == "-") {
            enpassant = InvalidSquare;
        }
        else {
            enpassant = toSquare(splitted[3]);
        }
        // Halfmove
        halfmove = std::stoul(splitted[4]);
        // Fullmove
        fullmove = std::stoul(splitted[5]);
    }
    string Board::toFEN() const {
        string fen;
        // Board
        int32 emptycount = 0;
        for (Square square = 0; square < 64; ++square) {
            if (get(square).isEmpty()) {
                emptycount += 1;
            }
            else {
                if (0 < emptycount) {
                    fen += std::to_string(emptycount);
                    emptycount = 0;
                }
                fen += fromPiece(get(square));
            }
            if (getFile(square) == 7) {
                if (0 < emptycount) {
                    fen += std::to_string(emptycount);
                    emptycount = 0;
                }
                fen += '/';
            }
        }
        fen.pop_back();
        fen += ' ';
        // Active
        fen += (active == Player::White ? 'w' : 'b');
        fen += ' ';
        // CastleState
        if (castle[Castle::WhiteKingside]) {
            fen += 'K';
        }
        if (castle[Castle::WhiteQueenside]) {
            fen += 'Q';
        }
        if (castle[Castle::BlackKingside]) {
            fen += 'k';
        }
        if (castle[Castle::BlackQueenside]) {
            fen += 'q';
        }
        fen += ' ';
        // EnPassant
        if (enpassant == InvalidSquare) {
            fen += '-';
        }
        else {
            fen += fromSquare(enpassant);
        }
        fen += ' ';
        // Halfmove
        fen += std::to_string(halfmove);
        fen += ' ';
        // Fullmove
        fen += std::to_string(fullmove);
        return fen;
    }
    // Board :: Square
    // Board :: Square :: Get / Set
    const Piece& Board::get(Square square) const {
        return squares.at(square);
    }
    const Piece& Board::get(Square square, Direction filedir, Direction rankdir) const {
        return get(addSquare(square, filedir, rankdir));
    }
    void Board::set(Square square, Piece piece) {
        squares.at(square) = piece;
    }
    // Board :: Square :: Player / Type
    Player Board::player(Square square) const {
        return get(square).player;
    }
    PieceType Board::type(Square square) const {
        return get(square).type;
    }
    // Board :: Square :: IsEmpty
    bool Board::isEmpty(Square square) const {
        return get(square).isEmpty();
    }
    bool Board::isEmpty(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).isEmpty();
    }
    bool Board::inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isEmpty(square, filedir, rankdir);
    }
    // Board :: Square :: IsFriendly
    bool Board::isFriendly(Square square) const {
        return get(square).player == active;
    }
    bool Board::isFriendly(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).player == active;
    }
    bool Board::inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isFriendly(square, filedir, rankdir);
    }
    // Board :: Square :: IsEnemy
    bool Board::isEnemy(Square square) const {
        return get(square).player == (active == Player::White ? Player::Black : Player::White);
    }
    bool Board::isEnemy(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).player == getEnemy(active);
    }
    bool Board::inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isEnemy(square, filedir, rankdir);
    }
    // Board :: Square :: InBoundsAndEquals
    bool Board::inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const {
        return inBounds(square, filedir, rankdir) && get(square, filedir, rankdir) == piece;
    }
    // Board :: GetMoveset
    List<Move> Board::getMoveset() const {
        List<Move> legalmoves;
        for (Square square = 0; square < 64; ++square) {
            if (isFriendly(square)) {
                // Pawn
                if (type(square) == PieceType::Pawn) {
                    // Advance
                    if (inBoundsAndEmpty(square, 0, pawndirection[active])) {
                        legalmoves.emplace_back(square, 0, pawndirection[active]);
                        // DoubleAdvance
                        if (getRank(square) == pawnstartingpos[active] && isEmpty(square, 0, pawndirection[active] * 2)) {
                            legalmoves.emplace_back(square, 0, pawndirection[active] * 2);
                        }
                    }
                    // Capture
                    if (inBoundsAndEnemy(square, 1, pawndirection[active])) {
                        legalmoves.emplace_back(square, 1, pawndirection[active]);
                    }
                    if (inBoundsAndEnemy(square, -1, pawndirection[active])) {
                        legalmoves.emplace_back(square, -1, pawndirection[active]);
                    }
                    // EnPassant
                    if (inBounds(square, 1, pawndirection[active]) && addSquare(square, 1, pawndirection[active]) == enpassant) {
                        legalmoves.emplace_back(square, enpassant);
                    }
                    else if (inBounds(square, 1, pawndirection[active]) && addSquare(square, -1, pawndirection[active]) == enpassant) {
                        legalmoves.emplace_back(square, enpassant);
                    }
                }
                // Knight
                else if (type(square) == PieceType::Knight) {
                    for (const auto& direction : knightdirections) {
                        if (inBounds(square, direction[0], direction[1]) && !isFriendly(square, direction[0], direction[1])) {
                            legalmoves.emplace_back(square, direction[0], direction[1]);
                        }
                    }
                }
                // Bishop / Rook / Queen
                else if (type(square) == PieceType::Bishop || type(square) == PieceType::Rook || type(square) == PieceType::Queen) {
                    const auto& directions = (type(square) == PieceType::Bishop ? bishopdirections : (type(square) == PieceType::Rook ? rookdirections : queendirections));
                    for (const auto& direction : directions) {
                        for (int32 step = 1; step < 8; ++step) {
                            if (inBounds(square, direction[0] * step, direction[1] * step)) {
                                if (isEmpty(square, direction[0] * step, direction[1] * step)) {
                                    legalmoves.emplace_back(square, direction[0] * step, direction[1] * step);
                                }
                                else if (isEnemy(square, direction[0] * step, direction[1] * step)) {
                                    legalmoves.emplace_back(square, direction[0] * step, direction[1] * step);
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
                // King
                else if (type(square) == PieceType::King) {
                    // Normal
                    for (const auto& direction : kingdirections) {
                        if (inBounds(square, direction[0], direction[1]) && !isFriendly(square, direction[0], direction[1])) {
                            legalmoves.emplace_back(square, direction[0], direction[1]);
                        }
                    }
                    // CastleState
                    // CastleState :: White
                    if (active == Player::White) {
                        // Kingside
                        if (castle[Castle::WhiteKingside] && isEmpty(toSquare("f1")) && isEmpty(toSquare("g1"))) {
                            legalmoves.emplace_back(square, toSquare("g1"), MoveFlag::CastleStateK);
                        }
                        // Queenside
                        if (castle[Castle::WhiteQueenside] && isEmpty(toSquare("d1")) && isEmpty(toSquare("c1"))  && isEmpty(toSquare("b1"))) {
                            legalmoves.emplace_back(square, toSquare("c1"), MoveFlag::CastleStateQ);
                        }
                    }
                    // CastleState :: Black
                    else {
                        // Kingside
                        if (castle[Castle::BlackKingside] && isEmpty(toSquare("f8")) && isEmpty(toSquare("g8"))) {
                            legalmoves.emplace_back(square, toSquare("g8"), MoveFlag::CastleStateK);
                        }
                        // Queenside
                        if (castle[Castle::BlackQueenside] && isEmpty(toSquare("d8")) && isEmpty(toSquare("c8"))  && isEmpty(toSquare("b8"))) {
                            legalmoves.emplace_back(square, toSquare("c8"), MoveFlag::CastleStateQ);
                        }
                    }
                }
            }
        }
        return legalmoves;
    }
    // Board :: FindPiece
    Square Board::Board::findPiece(Piece piece) const {
        for (Square square = 0; square < 64; ++square) {
            if (get(square) == piece) {    
                return square;
            }
        }
        return InvalidSquare;
    }
    // Board :: IsSquareAttackedBy
    bool Board::isSquareAttackedBy(Square square, Player player) const {
        // Pawn
        if (inBoundsAndEquals(square, 1, -pawndirection[player], Piece(PieceType::Pawn, player))) {
            return true;
        }
        if (inBoundsAndEquals(square, -1, -pawndirection[player], Piece(PieceType::Pawn, player))) {
            return true;
        }
        // Knight
        for (const auto& direction : knightdirections) {
            if (inBoundsAndEquals(square, direction[0], direction[1], Piece(PieceType::Knight, player))) {
                return true;
            }
        }
        // Bishop / Rook / Queen
        for (const auto& [type, directions] : directionmap) {
            for (const auto& direction : directions) {
                for (int32 step = 1; step < 8; ++step) {
                    if (inBounds(square, direction[0] * step, direction[1] * step)) {
                        if (isEmpty(square, direction[0] * step, direction[1] * step)) {
                            continue;
                        }
                        else {
                            if (get(square, direction[0] * step, direction[1] * step) == Piece(type, player)) {
                                return true;
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
        // King
        for (const auto& direction : kingdirections) {
            if (inBoundsAndEquals(square, direction[0], direction[1], Piece(PieceType::King, player))) {
                return true;
            }
        }
        return false;
    }
    // Board :: LegalMoves
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
    // Board :: CanCaptureKing / IsKingAttacked
    bool Board::canCaptureKing() const {
        return isSquareAttackedBy(findPiece(Piece(PieceType::King, getEnemy(active))), active);
    }
    bool Board::isKingAttacked() const {
        return isSquareAttackedBy(findPiece(Piece(PieceType::King, active)), getEnemy(active));
    }
    // InCheck / InCheckmate
    bool Board::inCheck() const {
        return isKingAttacked();
    }
    bool Board::inCheckmate() const {
        return inCheck() && getLegalMoves().size() == 0;
    }
    // Board :: ToString
    string Board::toString() const {
        // Board
        List<string> lines;
        for (int32 rank = 0; rank < 8; ++rank) {
            string line = "| ";
            for (int32 file = 0; file < 8; ++file) {
                line += fromPiece(get((8 * rank + file)));
                line += ' ';
            }
            if (rank == 0) {
                line += " Gamestate";
            }
            else if (rank == 2) {
                line += " Active: ";
                line += (active == Player::White ? "White" : (active == Player::Black ? "Black" : "None"));
            }
            else if (rank == 3) {
                line += " CastleState: ";
                if (castle[Castle::WhiteKingside]) {
                    line += 'K';
                }
                if (castle[Castle::WhiteQueenside]) {
                    line += 'Q';
                }
                if (castle[Castle::BlackKingside]) {
                    line += 'k';
                }
                if (castle[Castle::BlackQueenside]) {
                    line += 'q';
                }
            }
            else if (rank == 4) {
                line += " Halfmove: ";
                line += std::to_string(halfmove); 
            }
            else if (rank == 5) {
                line += " Fullmove: ";
                line += std::to_string(fullmove); 
            }
            lines.push_back(line);
        }
        // MaxLength
        address maxlength = 0;
        for (const string& line : lines) {
            maxlength = std::max(line.length(), maxlength);
        }
        // Out
        string out;
        for (int32 i = 0; i < maxlength + 2; ++i) {
            out += '-';
        }
        out += '\n';
        for (const string& line : lines) {
            out += line;
            for (int32 i = line.length(); i < maxlength; ++i) {
                out += ' ';
            }
            out += " |\n";
        }
        for (int32 i = 0; i < maxlength + 2; ++i) {
            out += '-';
        }
        out += '\n';
        return out;
    }
    // Board :: HasSamePositions (According to FIDE)
    bool Board::hasSamePositions(const Board& other) const {
        // SamePlayer
        if (active != other.active) {
            return false;
        }
        // SamePieces
        for (Square square = 0; square < 64; ++square) {
            if (get(square) != other.get(square)) {
                return false;
            }
        }
        // SameMoves (NOTE: move order is stable)
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
    // Board :: CanClaimDraw
    bool Board::canClaimDraw() const {
        return 50 <= halfmove;
    }
    // Board :: Iterate
    auto Board::begin() {
        return squares.begin();
    }
    const auto Board::begin() const {
        return squares.begin();
    }
    auto Board::end() {
        return squares.end();
    }
    const auto Board::end() const {
        return squares.end();
    }
}