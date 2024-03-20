#include "piece.hpp"

namespace Chessmate {
    // Piece
    // Piece :: Constructor
    Piece::Piece()
        : Piece(PieceType::None, Player::None) {}
    Piece::Piece(PieceType type, Player player)
        : type(type), player(player) {}
    // Piece :: IsEmpty
    bool Piece::isEmpty() const {
        return type == PieceType::None && player == Player::None;
    }
    // Piece :: Compare
    bool Piece::operator==(const Piece& other) const {
        return type == other.type && player == other.player;
    }
    bool Piece::operator!=(const Piece& other) const {
        return type != other.type || player != other.player;
    }
    // ToPieceType
    PieceType toPieceType(char ch) {
        switch (ch) {
        case 'P':
        case 'p':
            return PieceType::Pawn;
        case 'N':
        case 'n':
            return PieceType::Knight;
        case 'B':
        case 'b':
            return PieceType::Bishop;
        case 'R':
        case 'r':
            return PieceType::Rook;
        case 'Q':
        case 'q':
            return PieceType::Queen;
        case 'K':
        case 'k':
            return PieceType::King;
        default:
            return PieceType::None;
        }
    }
    // ToPiece / FromPiece
    Piece toPiece(char ch) {
        return Piece(toPieceType(ch), std::isalpha(ch) ? (std::isupper(ch) ? Player::White : Player::Black) : Player::None);
    }
    char fromPiece(Piece piece) {
        char ch;
        // Type
        switch (piece.type) {
        case PieceType::Pawn:
            ch = 'p';
            break;
        case PieceType::Knight:
            ch = 'n';
            break;
        case PieceType::Bishop:
            ch = 'b';
            break;
        case PieceType::Rook:
            ch = 'r';
            break;
        case PieceType::Queen:
            ch = 'q';
            break;
        case PieceType::King:
            ch = 'k';
            break;
        default:
            ch = '-';
            break;
        }
        // Player
        switch (piece.player) {
        case Player::White:
            return std::toupper(ch);
        case Player::Black:
            return ch;
        default:
            return '-';
        }
    }
}