#include "parse.hpp"

namespace Chessmate {
    // fromSquare
    string Parse::fromSquare(Square square) {
        string str;
        str += string("abcdefgh")[square % 8];
        str += string("87654321")[square / 8];
        return str;
    }
    // toSquare
    Square Parse::toSquare(const string& str) {
        if (str.length() == 2) {
            if ('a' <= str[0] <= 'h' && '1' <= str[1] <= '8') {
                return 8 * ('8' - str[1]) + (str[0] - 'a');
            }
        }
        return NoSquare;
    }

    // fromPlayer
    string Parse::fromPlayer(Player player) {
        switch (player) {
        case Player::White:
            return "w";
        case Player::Black:
            return "b";
        case Player::None:
            return "none";
        default:
            return string();
        }
    }
    // toPlayer
    Player Parse::toPlayer(const string& str) {
        if (str == "w") {
            return Player::White;
        }
        else if (str == "b") {
            return Player::Black;
        }
        return Player::None;
    }

    // toPieceType
    PieceType Parse::toPieceType(char ch) {
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
    // toPiece
    Piece Parse::toPiece(char ch) {
        return Piece(toPieceType(ch), std::isalpha(ch) ? (std::isupper(ch) ? Player::White : Player::Black) : Player::None);
    }
    // fromPiece
    char Parse::fromPiece(Piece piece) {
        char ch;
        // type
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
        // player
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