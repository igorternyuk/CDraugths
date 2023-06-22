#pragma once
#include "rules.hpp"

/*
    virtual Alliance FirstMoveAlliance() const = 0;
    virtual int GetPieceValue(const Piece& piece) const = 0;
    virtual bool IsTileValid(const Position& position, const Tile& tile) const = 0;
    virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const = 0;
    virtual GameStatus GetGameStatus(const Position& position) = 0;
    virtual bool CheckIfCoronate(const Position& position, Move& move) const = 0;
*/

class RulesDraughts64: public Rules
{
public:
    RulesDraughts64();
public:
    virtual Alliance FirstMoveAlliance() const override;
    virtual int GetPieceValue(const Piece& piece) const override;
    virtual bool IsTileValid(const Position& position, const Tile& tile) const override;
    virtual void CalcLegalMoves(const Position& position, Alliance alliance, std::vector<Move> &moves) const override;
    virtual bool CheckIfCoronate(const Position& position, const Move& move) const override;
    virtual GameStatus GetGameStatus(const Position& position) const override;
private:
    enum PieceValues
    {
        PIECE_VALUE = 100,
        KING_VALUE = 300,
    };
    const int offsetX_[4] { +1, +1, -1, -1 };
    const int offsetY_[4] { -1, +1, +1, -1 };

private:
    void CalcAllJumps(const Position& position, const Piece& piece, Move move, std::vector<Move> &legalMoves) const;
};
