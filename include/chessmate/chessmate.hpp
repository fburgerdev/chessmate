#pragma once
// #include <...> (HPPMERGE)
#include <ranges>
#include <memory>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <unordered_set>
#include <map>
#include <set>
#include <unordered_map>
#include <functional>
#include <optional>
#include <iostream>
// #include "chessmate.hpp" (HPPMERGE)
// game


// #include "common.hpp" (HPPMERGE)
namespace Chessmate {
    // ranges
    namespace stdr = std::ranges;

    // types
    // types :: address
    using address = std::size_t;
    // types :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // types :: uint
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // types :: string
    using string = std::string;
    using string_view = std::string_view;

    // container
    // container :: sequential
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Deque = std::deque<T>;
    // container :: tree
    template<typename T>
    using Set = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;
    // container :: hash
    template<typename T>
    using HashSet = std::unordered_set<T>;
    template<typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    // move semantics
    using std::forward;
    using std::move;

    // smart pointers
    using std::make_unique;
    using std::make_shared;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;

    // function
    using std::function;

    // templates
    template<typename T>
    using Opt = std::optional<T>;

    // debug
    using std::cout;
    using std::endl;
}

// #include "player.hpp" (HPPMERGE)
namespace Chessmate {
    // Player
    enum class Player {
        None = 0, White, Black
    };
    // getEnemy
    Player getEnemy(Player player);
}

// #include "piece.hpp" (HPPMERGE)
namespace Chessmate {
    // PieceType
    enum class PieceType {
        None = 0, Pawn, Knight, Bishop, Rook, Queen, King
    };
    // Piece
    struct Piece {
        // construtor
        Piece();
        Piece(PieceType type, Player player);

        // isEmpty
        bool isEmpty() const;
        
        // operator<=>
        auto operator<=>(const Piece& other) const = default;

        // type / player
        PieceType type;
        Player player;
    };
}

// #include "square.hpp" (HPPMERGE)
namespace Chessmate {
    // Square / Direction
    using Square = int32;
    using Direction = int32;
    
    // NoSquare
    static inline constexpr Square NoSquare = -1;

    // inBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir);
    
    // getFile / getRank
    int32 getFile(Square square);
    int32 getRank(Square square);

    // addSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir);
}

// #include "move.hpp" (HPPMERGE)
namespace Chessmate {
    // MoveFlag
    enum class MoveFlag {
        None = 0, Normal, DoubleAdvance, EnPassant, CastleK, CastleQ, PromoteN, PromoteB, PromoteR, PromoteQ
    };
    // Move
    struct Move {
        // constructor
        Move();
        Move(MoveFlag flag);
        Move(Piece piece, Square origin, Square target);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir);
        Move(Piece piece, Square origin, Square target, MoveFlag flag);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir, MoveFlag flag);

        // isValid
        bool isValid() const;
        
        // compare
        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;
        
        // toAlgebraicNotation (Algebraic Notation - UCI Standard)
        string toAlgebraicNotation() const;
        
        // member
        Piece piece;
        Square origin, target;
        MoveFlag flag;
    };
}

// #include "castle.hpp" (HPPMERGE)
namespace Chessmate {
    // Castle
    enum class Castle {
        WhiteKingside, WhiteQueenside, BlackKingside, BlackQueenside
    };
    // CastleState
    struct CastleState {
    public:
        // constructor
        CastleState();
        CastleState(bool wk, bool wq, bool bk, bool bq);

        // operator[]
        bool& operator[](Castle type);
        bool operator[](Castle type) const;
        
        // operator<=>
        auto operator<=>(const CastleState&) const = default;
    private:
        // member
        bool m_wk, m_wq, m_bk, m_bq;
    };
}

// #include "board.hpp" (HPPMERGE)
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

// #include "parse.hpp" (HPPMERGE)
namespace Chessmate {
    struct Parse {
        // fromSquare / toSquare    
        static string fromSquare(Square square);
        static Square toSquare(const string& str);

        // fromPlayer / toPlayer
        static string fromPlayer(Player player);
        static Player toPlayer(const string& str);

        // toPieceType
        static PieceType toPieceType(char ch);
        // toPiece / fromPiece
        static Piece toPiece(char ch);
        static char fromPiece(Piece piece);
    };
}
// bot

// #include "evaluate.hpp" (HPPMERGE)
namespace Chessmate {
    // evaluatePieceType
    float evaluatePieceType(PieceType type);
    // evaluatePiece
    float evaluatePiece(Piece piece, Player active);
    // evaluateSquare
    float evaluateSquare(Square square, const Board& board);
    // evaluateBoard
    float evaluateBoard(const Board& board);
}

// #include "minimax.hpp" (HPPMERGE)
namespace Chessmate {
    // MinimaxResult
    struct MinimaxResult {
        // constructor
        MinimaxResult(Move move, float eval);
        
        // compare
        bool operator<(const MinimaxResult& other) const;
        bool operator<=(const MinimaxResult& other) const;
        bool operator>(const MinimaxResult& other) const;
        bool operator>=(const MinimaxResult& other) const;

        // move / eval
        Move move;
        float eval;
    };
    
    // minimize / maximize
    MinimaxResult minimize(const Board& board, uint32 depth, float alpha, float beta);
    MinimaxResult maximize(const Board& board, uint32 depth, float alpha, float beta);
    // minimax
    MinimaxResult minimax(const Board& board, uint32 depth);
}