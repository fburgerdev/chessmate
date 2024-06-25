#include "castle.hpp"

namespace Chessmate {
    // constructor
    CastleState::CastleState()
        : CastleState(false, false, false, false) {}
    CastleState::CastleState(bool wk, bool wq, bool bk, bool bq)
        : m_wk(wk), m_wq(wq), m_bk(bk), m_bq(bq) {}
    
    // operator[]
    bool& CastleState::operator[](Castle type) {
        switch (type) {
        case Castle::WhiteKingside:
            return m_wk;
        case Castle::WhiteQueenside:
            return m_wq;
        case Castle::BlackKingside:
            return m_bk;
        case Castle::BlackQueenside:
            return m_bq;
        default:
            return *(bool*)0;
        }
    }
    bool CastleState::operator[](Castle type) const {
        switch (type) {
        case Castle::WhiteKingside:
            return m_wk;
        case Castle::WhiteQueenside:
            return m_wq;
        case Castle::BlackKingside:
            return m_bk;
        case Castle::BlackQueenside:
            return m_bq;
        default:
            return false;
        }
    }
}