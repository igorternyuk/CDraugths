#pragma once
#include "alliance.hpp"
#include "move.hpp"
#include <map>
#include <memory>

namespace draughts
{
    class Rules;
    class Position
    {
    public:
        virtual ~Position() {};
    public:
        virtual std::shared_ptr<Position> MakeCopy() const = 0;
        virtual bool MakeMove(const Move& move) = 0;
        virtual bool UndoLastMove() = 0;
        virtual bool IsEndgameScenario() const = 0;
        virtual Alliance GetTurn() const = 0;
        virtual int GetBoardSize() const = 0;
        virtual const Tile& GetTile(int row, int col) const = 0;
        virtual unsigned int GetHash() const = 0;
        virtual void LegalMoves(Alliance alliance, std::vector<Move>& lolm) const = 0;
        virtual bool HasAvailableTile(int row, int col) const = 0;
        virtual std::shared_ptr<Rules> GetRules() const = 0;
        virtual std::string MoveToNotation(const Move& move) const = 0;
        virtual const std::vector<Move>& GetMoveLog() const = 0;
        virtual std::string ToString() const = 0;
        virtual int GetTotalPieces() const = 0;
        virtual const std::map<int, Piece*>& GetPieces(Alliance alliance) const = 0;
    };
}

