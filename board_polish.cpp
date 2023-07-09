#include "board_polish.h"

using namespace draughts;

static std::map<int, std::string> _mapNotation = FillNotationMap(BoardPolish::BOARD_SIZE,BoardPolish::BOARD_SIZE);

BoardPolish::BoardPolish(): BoardInternational(BOARD_SIZE)
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
    BoardInternational::SetupInitialPosition();
}

std::shared_ptr<Position> BoardPolish::MakeCopy() const
{
    return std::make_shared<BoardPolish>(*this);
}

int BoardPolish::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardPolish::GetNotation() const
{
    return Notation::NUMERIC;
}

std::string BoardPolish::TileToNotation(const Tile &tile) const {
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
