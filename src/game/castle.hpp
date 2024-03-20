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
        // Constructor
        CastleState();
        CastleState(bool wk, bool wq, bool bk, bool bq);
        // Operator[]
        bool operator[](Castle type) const;
        bool& operator[](Castle type);
        // Compare
        bool operator==(const CastleState& other) const;
        bool operator!=(const CastleState& other) const;
    private:
        // Member
        bool m_wk, m_wq, m_bk, m_bq;
    };
}