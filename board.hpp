#pragma once

#include "piece.hpp"
#include "tile.hpp"
#include "position.hpp"
#include "move.hpp"
#include "gamestatus.hpp"
#include <memory>
#include <vector>
#include <map>
#include <map>
#include <string>
#include <cmath>

class Rules;

namespace draughts
{
    //TODO add vectors of pieces
    class Board : public Position
    {
    public:
        explicit Board(std::shared_ptr<Rules>, int N);
    public:
        ////// Interface Posiiton begin //////

        virtual const Tile& GetTile(int row, int col) const override;
        virtual Alliance GetTurn() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual void LegalMoves(Alliance alliance, std::vector<Move>& lolm) const override;
        virtual bool HasAvailableTile(int row, int col) const override;
        virtual const std::vector<Move>& GetMoveLog() const override;
        virtual std::shared_ptr<Rules> GetRules() const override;
        const std::map<int, Piece*>& GetPieces(Alliance alliance) const override;

        ////// Interface Posiiton end //////

        virtual void SetupInitialPosition() = 0;
        virtual std::string TileToAlgebraicNotation(const Tile& tile) const = 0;
        virtual unsigned int GetHash() const override;

        bool HasValidTile(int row, int col) const;
        bool IsTileEmpty(int row, int col) const;
        bool SetPiece(int row, int col, Alliance alliance, bool isKing = false);
        bool IsEndGameScenario() const;
        virtual GameStatus GetGameStatus() const;
        Tile &GetTile(int row, int col);


    protected:
        void Clear();
        void Reset();
        void SetTile(const Tile& tile, int row, int col);
        void SetTile(const Tile& tile, int index);
        bool CheckCrown(const Piece &piece) const;
        int IndexByCoords(int row, int col) const;
        std::pair<int,int> CoordsByIndex(int index) const;
        bool IsValidIndex(int index) const;
        bool IsValidCoords(int row, int col) const;
        void CalcPieceCount(int &count_red_pieces, int &count_red_kings, int &count_blue_pieces, int &count_blue_kings) const;
    protected:
        std::vector<Tile> _grid;
        std::shared_ptr<Rules> _rules;
        std::vector<Move> _moveLog;
        std::map<Alliance,std::map<int, Piece*>> _mapOfPieces;
        mutable bool _bValidLegalMoves;
        mutable std::vector<Move> _legalMoves;
        mutable bool _bValidHash;
        mutable unsigned int _hash;
    };
}
