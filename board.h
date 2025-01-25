#pragma once

#include "notation.h"
#include "piece.h"
#include "tile.h"
#include "position.h"
#include "move.h"
#include "gamestatus.h"
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
        explicit Board(std::shared_ptr<Rules>, size_t N);
        explicit Board(std::shared_ptr<Rules>, size_t W, size_t H);
    public:
        ////// Interface Position begin //////

        virtual const Tile& GetTile(int row, int col) const override;
        virtual Alliance GetTurn() const override;
        virtual bool MakeMove(const Move& move) override;
        virtual bool UndoLastMove() override;
        virtual void LegalMoves(Alliance alliance, std::vector<Move>& lolm) const override;
        virtual bool HasAvailableTile(int row, int col) const override;
        virtual const std::vector<Move>& GetMoveLog() const override;
        virtual std::shared_ptr<Rules> GetRules() const override;
        const std::map<int, Piece*>& GetPieces(Alliance alliance) const override;
        virtual int GetBoardWidth() const override;
        virtual int GetBoardHeight() const override;
        virtual int GetBoardSize() const override;
        virtual bool IsCoronationTile(int row, int col, Alliance alliance) const override;

        ////// Interface Position _end //////

        virtual void SetupInitialPosition();
        virtual const std::vector<std::tuple<int,int>>& GetCoronationTiles(Alliance alliance) const override;
        virtual int GetPieceRows() const = 0;
        virtual Notation GetNotation() const = 0;
        virtual std::string TileToNotation(const Tile& tile) const = 0;
        virtual std::string MoveToNotation(const Move& move) const override;
        virtual unsigned int GetHash() const override;
        virtual std::string ToString() const override;

        virtual int GetTotalPieces() const override;
        bool HasValidTile(int row, int col) const;
        bool IsTileEmpty(int row, int col) const;
        bool SetPiece(int row, int col, Alliance alliance, bool isKing = false);
        virtual bool IsEndgameScenario() const override;
        virtual GameStatus GetGameStatus() const;
        Tile &GetTile(int row, int col);

    protected:
        enum PieceType
        {
            DARK_PIECE = 0,
            DARK_KING = 1,
            LIGHT_PIECE = 2,
            LIGHT_KING = 3,
        };
    protected:
        virtual void Clear();
        virtual void Reset();
        void SetTile(const Tile& tile, int row, int col);
        void SetTile(const Tile& tile, int index);
        bool CheckCrown(const Piece &piece) const;
        int IndexByCoords(int row, int col) const;
        std::pair<int,int> CoordsByIndex(int index) const;
        bool IsValidIndex(int index) const;
        bool IsValidCoords(int row, int col) const;
        void CalcPieceCount(int aCount[4]) const;
    protected:
        //mutable bool _bValidLegalMoves;
        //mutable std::vector<Move> _legalMoves;
        mutable bool _bValidHash;
        mutable unsigned int _hash;
        mutable bool _bGameStatusChanged;
        GameStatus _gameStatus;
        bool _log = true;
        size_t _width;
        std::vector<Tile> _grid;
        std::shared_ptr<Rules> _rules;
        std::vector<Move> _moveLog;
        std::map<Alliance,std::map<int, Piece*>> _mapOfPieces;

    };
}
