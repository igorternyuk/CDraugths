#include "board.hpp"
#include "rules.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

Board::Board(std::shared_ptr<Rules> rules, int N):
    _rules(rules)
{
    _grid.reserve(N * N);
    for(int r = 0; r < N; ++r)
        for(int c = 0; c < N; ++c)
            _grid.push_back(Tile(r, c));
    _moveLog.clear();
}

void Board::Clear()
{
    for(size_t i = 0; i < _grid.size(); ++i)
        _grid[i].RemovePiece();
}

Tile &Board::GetTile(int row, int col)
{
    int index = IndexByCoords(row, col);
    return IsValidIndex(index) ? _grid[index] : Tile::NULL_TILE;
}

const std::map<int, Piece *> &Board::GetPieces(Alliance alliance) const
{
    return _mapOfPieces.at(alliance);
}

void Board::SetTile(const Tile &tile, int row, int col)
{
    int index = IndexByCoords(row, col);
    if(IsValidIndex(index))
        _grid[index] = tile;
}

void Board::SetTile(const Tile &tile, int index)
{
    if(IsValidIndex(index))
        _grid[index] = tile;
}

int Board::IndexByCoords(int row, int col) const
{
    int index = -1;
    const int N = GetBoardSize();
    if(row >= 0 && row < N && col >= 0 && col < N)
        index = row * N + col;
    return index;
}

std::pair<int, int> Board::CoordsByIndex(int index) const
{
    if(IsValidIndex(index))
    {
        const int N = GetBoardSize();
        int row = index / N;
        int col = index % N;
        return {row, col};
    }
    else
        return {-1,-1};
}

bool Board::IsValidIndex(int index) const
{
    return index >= 0 && index < _grid.size();
}

bool Board::IsValidCoords(int row, int col) const
{
    int index = IndexByCoords(row, col);
    return IsValidIndex(index);
}

void Board::CalcPieceCount(int &count_red_pieces, int &count_red_kings, int &count_blue_pieces, int &count_blue_kings) const
{
    count_red_pieces = 0;
    count_red_kings = 0;
    count_blue_pieces = 0;
    count_blue_kings = 0;

    const int BOARD_SIZE = GetBoardSize();
    for(int y = 0; y < BOARD_SIZE; ++y)
    {
        for(int x = 0; x < BOARD_SIZE; ++x)
        {
            const Tile& tile = GetTile(y, x);
            if(tile.HasPiece())
            {
                const Piece& piece = tile.GetPiece();
                Alliance alliance = piece.GetAlliance();
                bool isKing = piece.IsKing();
                if(alliance == Alliance::RED)
                {
                    if(isKing)
                        count_red_kings++;
                    else
                        count_red_pieces++;
                }
                else if(alliance == Alliance::BLUE)
                {
                    if(isKing)
                        count_blue_kings++;
                    else
                        count_blue_pieces++;
                }
            }
         }
    }
}

bool Board::IsEndGameScenario() const
{
    GameStatus gamestatus = _rules->GetGameStatus(*this);
    return gamestatus != GameStatus::PLAY;
}

GameStatus Board::GetGameStatus() const
{
    Alliance turn = GetTurn();
    if(turn == Alliance::RED)
    {
        std::vector<Move> redMoves;
        _rules->CalcLegalMoves(*this, Alliance::RED, redMoves);
        if(redMoves.empty())
            return GameStatus::BLUE_WON;
    }
    else if(turn == Alliance::BLUE)
    {
        std::vector<Move> blueMoves;
        _rules->CalcLegalMoves(*this, Alliance::BLUE, blueMoves);
        if(blueMoves.empty())
            return GameStatus::RED_WON;
    }

    return GameStatus::PLAY;
}

bool Board::HasValidTile(int row, int col) const
{
    return IsValidCoords(row, col);
}

bool Board::IsTileEmpty(int row, int col) const
{
    int index = IndexByCoords(row, col);
    return IsValidIndex(index) && _grid[index].IsEmpty();
}

const Tile& Board::GetTile(int row, int col) const
{
    int index = IndexByCoords(row, col);
    return IsValidIndex(index) ? _grid[index] : Tile::NULL_TILE;
}

Alliance Board::GetTurn() const
{
    if(_moveLog.empty())
        return _rules->FirstMoveAlliance();
    Alliance alliance = _moveLog.back().GetFirstStep().GetStart().GetPiece().GetAlliance();
    return OpponentAlliance(alliance);
}

bool Board::SetPiece(int row, int col, Alliance alliance, bool isKing)
{
    if(IsValidCoords(row,col))
    {
        Piece piece(row, col, alliance, isKing);
        int index = IndexByCoords(row, col);
        _grid[index].SetPiece(piece);
        _mapOfPieces[alliance][index] = &_grid[index].GetPiece();
        return true;
    }
    return false;
}

bool Board::MakeMove(const Move& move)
{
    if(move.IsEmpty()) return false;
    const Tile& tileStart = move.GetFirstStep().GetStart();
    const Tile& tileEnd = move.GetLastStep().GetEnd();
    int startRow = tileStart.GetRow();
    int startCol = tileStart.GetCol();

    const Piece& movedPiece = tileStart.GetPiece();
    int endRow = tileEnd.GetRow();
    int endCol = tileEnd.GetCol();

    //TODO make this check in rules
    if(!(HasValidTile(startRow, startCol) && _rules->IsTileValid(*this, tileStart)
            && HasValidTile(endRow, endCol) && _rules->IsTileValid(*this, tileEnd)))
        return false;

    GetTile(startRow, startCol).RemovePiece();
    GetTile(endRow, endCol).SetPiece(movedPiece);

    Alliance turn = GetTurn();
    int startIndex = IndexByCoords(startRow, startCol);
    int endIndex = IndexByCoords(endRow, endCol);
    //size_t sz_ = _mapOfPieces[turn].size();
    _mapOfPieces[turn].erase(startIndex);
   // size_t sz = _mapOfPieces[turn].size();
    _mapOfPieces[turn][endIndex] = &GetTile(endRow, endCol).GetPiece();

    Alliance opponent = OpponentAlliance(turn);
    const int stepCount = move.StepCount();
    if(move.IsJump())
    {
        for(int i = 0; i < stepCount; ++i)
        {
            const Step& step = move.GetStep(i);
            int cx = step.GetCaptured().GetCol();
            int cy = step.GetCaptured().GetRow();
            GetTile(cy, cx).RemovePiece();
            int index = IndexByCoords(cy, cx);
            _mapOfPieces[opponent].erase(index);
        }
    }

    if(move.IsCoronation())
    {
        GetTile(endRow, endCol).GetPiece().Crown();
    }

    _moveLog.push_back(move);
    _bValidLegalMoves = false;
    _bValidHash = false;

    return true;
}

bool Board::UndoLastMove()
{
    if(_moveLog.empty()) return false;
    const Move& move = _moveLog.back();
    const Tile& tileFirst = move.GetFirstStep().GetStart();
    const Tile& tileLast = move.GetLastStep().GetEnd();
    int startCol = tileFirst.GetCol();
    int startRow = tileFirst.GetRow();
    const Piece& movedPiece = tileFirst.GetPiece();
    int endCol = tileLast.GetCol();
    int endRow = tileLast.GetRow();

    Alliance turn = GetTurn();
    int startIndex = IndexByCoords(startRow, startCol);
    int endIndex = IndexByCoords(endRow, endCol);
    Alliance opponent = OpponentAlliance(turn);
     GetTile(startRow, startCol).SetPiece(movedPiece);
    if(move.IsCoronation())
        GetTile(startRow, startCol).GetPiece().Uncrown();
     GetTile(endRow, endCol).RemovePiece();
    _mapOfPieces[opponent][startIndex] = &GetTile(startRow, startCol).GetPiece();
    _mapOfPieces[opponent].erase(endIndex);

    const int stepCount = move.StepCount();
    if(move.IsJump())
    {
        for(int i = 0; i < stepCount; ++i)
        {
            const Step& step = move.GetStep(i);
            const Piece& capturedPiece = step.GetCaptured();
            int cr = capturedPiece.GetRow();
            int cc = capturedPiece.GetCol();
            GetTile(cr, cc).SetPiece(capturedPiece);
            int index = IndexByCoords(cr, cc);
            _mapOfPieces[turn][index] = &GetTile(cr, cc).GetPiece();
        }
    }

    _moveLog.pop_back();
    _moveLog.shrink_to_fit();
    _bValidLegalMoves = false;
    _bValidHash = false;

    return true;
}

unsigned int Board::GetHash() const
{
    if(!_bValidHash)
    {
        std::string str = ToString();
        _hash = Utils::hash_str(str.c_str());
        _bValidHash = true;
    }
    return _hash;
}

void Board::LegalMoves(Alliance alliance, std::vector<Move> &moves) const
{
    if(!_bValidLegalMoves && _rules)
    {
        _rules->CalcLegalMoves(*this, alliance, moves);
        _legalMoves.clear();
        std::copy(moves.begin(), moves.end(), std::back_inserter(_legalMoves));
        _bValidLegalMoves = true;
    }
    else
    {
       std::copy(_legalMoves.begin(), _legalMoves.end(), std::back_inserter(moves));
    }
}

bool Board::HasAvailableTile(int row, int col) const
{
    bool rc = false;
    if(HasValidTile(row, col))
    {
        const Tile& tile = GetTile(row, col);
        if(_rules && _rules->IsTileValid(*this, tile))
            rc = true;
    }
    return rc;
}

const std::vector<Move> &Board::GetMoveLog() const
{
    return _moveLog;
}

std::shared_ptr<Rules> Board::GetRules() const
{
    return _rules;
}
