#include "board.hpp"
#include "rules.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace draughts;

Board::Board(std::shared_ptr<Rules> rules, int N):
    _rules(rules)
{
    _grid.reserve(N * N);
    for(int r = 0; r < N; ++r)
        for(int c = 0; c < N; ++c)
            _grid.push_back(Tile(r, c));
    _moveLog.clear();
    _gameStatus = GameStatus::PLAY;
    _bGameStatusChanged = true;

}

void Board::Clear()
{
    for(size_t i = 0; i < _grid.size(); ++i)
        _grid[i].RemovePiece();
}

void Board::Reset()
{
    _mapOfPieces.clear();
    _moveLog.clear();
    _bValidLegalMoves = false;
    _legalMoves.clear();
    _bValidHash = false;
    _hash = 0;
    _bGameStatusChanged = true;
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

void Board::SetupInitialPosition()
{
    const int BOARD_SIZE = GetBoardSize();
    const int NUM_PIECES_ROWS = GetPieceRows();
    Reset();
    Clear();
    for(int y = 0; y < NUM_PIECES_ROWS; ++y)
        for(int x = (y + 1) % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::DARK);


    for(int y = BOARD_SIZE - NUM_PIECES_ROWS; y < BOARD_SIZE; ++y)
        for(int x = (y + 1) % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::LIGHT);
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

void Board::CalcPieceCount(int aCount[]) const
{
    aCount[RED_PIECE] = 0;
    aCount[RED_KING] = 0;
    aCount[BLUE_PIECE] = 0;
    aCount[BLUE_KING] = 0;

    auto redPieces = GetPieces(Alliance::DARK);
    auto bluePieces = GetPieces(Alliance::LIGHT);

    for(const auto& [k,p] : redPieces)
    {
        const Piece& piece = *p;
        if(piece.IsKing())
            aCount[RED_KING]++;
        else
            aCount[RED_PIECE]++;
    }

    for(const auto& [k,p] : bluePieces)
    {
        const Piece& piece = *p;
        if(piece.IsKing())
            aCount[BLUE_KING]++;
        else
            aCount[BLUE_PIECE]++;
    }
}

std::string Board::MoveToNotation(const Move &move) const
{
    if(move.IsEmpty()) return "";
    const Step& step = move.GetStep(0);
    const Tile& tileStart = step.GetStart();
    std::string notation = TileToNotation(tileStart);
    if(step.isJump())
    {
        const int stepCount = move.StepCount();
        for(int i = 0; i < stepCount; ++i)
        {
            const Step& step = move.GetStep(i);
            notation += ":" + TileToNotation(step.GetEnd());
        }
    }
    else
    {        
        notation +=  "-" + TileToNotation(step.GetEnd());
    }
    return notation;
}


bool Board::IsEndgameScenario() const
{
   return GetGameStatus() != GameStatus::PLAY;
}

GameStatus Board::GetGameStatus() const
{
    if(_bGameStatusChanged)
    {
        Alliance turn = GetTurn();

        if(turn == Alliance::LIGHT)
        {
            std::vector<Move> blueMoves;
            _rules->CalcLegalMoves(*this, Alliance::LIGHT, blueMoves);
            if(blueMoves.empty())
                return GameStatus::RED_WON;
        }
        else if(turn == Alliance::DARK)
        {
           std::vector<Move> redMoves;
           _rules->CalcLegalMoves(*this, Alliance::DARK, redMoves);
           if(redMoves.empty())
               return GameStatus::BLUE_WON;
        }

        _bGameStatusChanged = false;
        return GameStatus::PLAY;
    }

    return _gameStatus;
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
    _bGameStatusChanged = true;
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
    _bGameStatusChanged = true;
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

std::string Board::ToString() const
{
    const int BOARD_SIZE = GetBoardSize();
    std::stringstream ss;
    for(int y = 0; y < BOARD_SIZE; ++y)
    {
        std::string line;
        for(int x = 0; x < BOARD_SIZE; ++x)
        {
            const Tile& tile = GetTile(y,x);
            if(tile.HasPiece())
            {
                Piece p = tile.GetPiece();
                if(p.GetAlliance() == Alliance::DARK)
                {
                    line += p.IsKing() ? "[R]" : "[r]";
                }
                else if(p.GetAlliance() == Alliance::LIGHT)
                {
                    line += p.IsKing() ? "[B]" : "[b]";
                }
            }
            else
            {
                line += tile.IsDark() ?  "[.]" : "[ ]";
            }
        }
        line += "\n";
        ss << line;
    }
    return ss.str();
}

int Board::GetTotalPieces() const
{
    auto redPieces = GetPieces(Alliance::DARK);
    auto bluePieces = GetPieces(Alliance::LIGHT);
    return redPieces.size() + bluePieces.size();
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

std::shared_ptr<draughts::Rules> Board::GetRules() const
{
    return _rules;
}
