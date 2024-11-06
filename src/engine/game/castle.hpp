#pragma once
#include "common.hpp"

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