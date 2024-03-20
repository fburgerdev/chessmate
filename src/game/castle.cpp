#include "castle.hpp"

namespace Chessmate {
    // CastleState
    // CastleState :: Constructor
    CastleState::CastleState()
        : CastleState(false, false, false, false) {}
    CastleState::CastleState(bool wk, bool wq, bool bk, bool bq)
        : m_wk(wk), m_wq(wq), m_bk(bk), m_bq(bq) {}
    // CastleState :: Operator[]
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
    // CastleState :: Compare
    bool CastleState::operator==(const CastleState& other) const {
        return m_wk == other.m_wk && m_wq == other.m_wq && m_bk == other.m_bk && m_bq == other.m_bq;
    }
    bool CastleState::operator!=(const CastleState& other) const {
        return m_wk != other.m_wk || m_wq != other.m_wq || m_bk != other.m_bk || m_bq != other.m_bq;
    }
}