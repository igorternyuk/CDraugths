#include "board_brazilian.h"

using namespace draughts;


BoardBrazilian::BoardBrazilian():BoardInternational(BOARD_SIZE)
{
    for(int r = 0; r < BOARD_HEIGHT; ++r)
        for(int c = 0; c < BOARD_WIDTH; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
            }
        }
    BoardInternational::SetupInitialPosition();
}

std::shared_ptr<Position> BoardBrazilian::MakeCopy() const
{
    return std::make_shared<BoardBrazilian>(*this);
}

int BoardBrazilian::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Notation BoardBrazilian::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

std::string BoardBrazilian::TileToNotation(const Tile &tile) const
{
    const int row = tile.GetRow();
    const int col = tile.GetCol();
    return _mapAlgebraicNotaion[col] + std::to_string(BOARD_SIZE - row);
}
