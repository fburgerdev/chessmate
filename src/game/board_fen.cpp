#include "board.hpp"
#include "parse.hpp"

namespace Chessmate {
    // split
    static List<string> split(const string& source, const string& delimiter) {
        address start = 0;
        address end = string::npos;
        List<string> splitted;
        while ((end = source.find(delimiter, start)) != std::string::npos) {
            if (end == start) {
                start += 1;
            }
            else {
                splitted.push_back(source.substr(start, end - start));
                start = end + delimiter.length();
            }
        }
        splitted.push_back(source.substr(start));
        return splitted;
    }

    // fromFEN
    void Board::fromFEN(const string& fen) {
        // TODO: Error Handling
        // words
        List<string> words = split(fen, " ");

        // squares
        int32 square = 0;
        for (char ch : words[0]) {
            if (std::isalpha(ch)) {
                set(square++, Parse::toPiece(ch));
            }
            else if (std::isdigit(ch)) {
                square += ch - '0';
            }
        }
        
        // active
        active = (words[1] == "w" ? Player::White : Player::Black);
        
        // castle
        castle = CastleState(
            words[2].find('K') != string::npos,
            words[2].find('Q') != string::npos,
            words[2].find('k') != string::npos,
            words[2].find('q') != string::npos
        );
        
        // enPassant
        if (words[3] == "-") {
            enPassant = NoSquare;
        }
        else {
            enPassant = Parse::toSquare(words[3]);
        }
        
        // halfMoves
        halfMoves = uint32(std::stoul(words[4]));

        // fullMoves
        fullMoves = uint32(std::stoul(words[5]));
    }
    // toFEN
    string Board::toFEN() const {
        string fen;

        // squares
        int32 emptyCount = 0;
        for (Square square = 0; square < 64; ++square) {
            if (get(square).isEmpty()) {
                emptyCount += 1;
            }
            else {
                if (emptyCount) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += Parse::fromPiece(get(square));
            }
            if (getFile(square) == 7) {
                if (emptyCount) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                if (getRank(square) != 7) {
                    fen += '/';
                }
            }
        }
        fen += ' '; // advance

        // active
        fen += (active == Player::White ? 'w' : 'b');
        fen += ' ';
        
        // castle
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
        if (fen.back() == ' ') {
            fen += '-';
        }
        fen += ' '; // advance

        // enPassant
        if (enPassant == NoSquare) {
            fen += '-';
        }
        else {
            fen += Parse::fromSquare(enPassant);
        }
        fen += ' '; // advance

        // halfMoves
        fen += std::to_string(halfMoves);
        fen += ' '; // advance

        // fullMoves
        fen += std::to_string(fullMoves);

        return fen;
    }
}