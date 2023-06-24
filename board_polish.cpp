#include "board_polish.hpp"
#include "rules_international.hpp"

using namespace draughts;

BoardPolish::BoardPolish(): Board(std::make_shared<RulesInternational>(), BOARD_SIZE)
{
     int k = 0;
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
                ++k;
                int index = IndexByCoords(r, c);
                _mapNotation[index] = std::to_string(k);
            }
        }
    SetupInitialPosition();
}

std::shared_ptr<Position> BoardPolish::MakeCopy() const
{
    return std::make_shared<BoardPolish>(*this);
}

int BoardPolish::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardPolish::GetNumPiecesForRow() const
{
    return NUM_PIECES_FOR_ROW;
}

Board::Notation BoardPolish::GetNotation() const
{
    return Notation::NUMERIC;
}

bool BoardPolish::MakeMove(const Move &move)
{
    return Board::MakeMove(move);
}

std::string BoardPolish::TileToNotation(const Tile &tile) const
{
    const int row = tile.GetRow();
    const int col = tile.GetCol();
    if(IsValidCoords(row, col))
    {
        const int index = IndexByCoords(row, col);
        if(_mapNotation.find(index) != _mapNotation.end())
            return _mapNotation.at(index);
    }
    return std::string("");
}

GameStatus BoardPolish::GetGameStatus() const
{
    return Board::GetGameStatus();
}

void BoardPolish::SetupInitialPosition()
{
    Board::SetupInitialPosition();
    //SetupTestPosition();
}

void BoardPolish::SetupTestPosition()
{
    Reset();
    Clear();
    SetPiece(2,7,Alliance::RED, true);
    SetPiece(6,7,Alliance::RED, false);
    SetPiece(9,2,Alliance::RED, false);
    SetPiece(9,4,Alliance::RED, false);

    SetPiece(5,4,Alliance::BLUE, false);
    SetPiece(6,5,Alliance::BLUE, false);
    SetPiece(8,5,Alliance::BLUE, false);
}
