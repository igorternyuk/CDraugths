#include "board_canadian.h"

using namespace draughts;

static std::map<int, std::string> _mapNotation = FillNotationMap(BoardCanadian::BOARD_SIZE,BoardCanadian::BOARD_SIZE);

BoardCanadian::BoardCanadian(): BoardInternational(BOARD_SIZE)
{
    BoardInternational::SetupBoard();
    BoardInternational::SetupInitialPosition();
}

std::shared_ptr<Position> BoardCanadian::MakeCopy() const
{
    return std::make_shared<BoardCanadian>(*this);
}

int BoardCanadian::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardCanadian::GetNotation() const
{
    return Notation::NUMERIC;
}

std::string BoardCanadian::TileToNotation(const Tile &tile) const
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
