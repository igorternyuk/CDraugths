#include "board_brazilian.hpp"

using namespace draughts;


BoardBrazilian::BoardBrazilian():BoardInternational(BOARD_SIZE)
{
    //int k = 0;
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
                //++k;
                //int index = IndexByCoords(r, c);
               // _mapNotation[index] = std::to_string(k);
            }
        }
    BoardInternational::SetupInitialPosition();
}

std::shared_ptr<Position> BoardBrazilian::MakeCopy() const
{
    return std::make_shared<BoardBrazilian>(*this);
}

int BoardBrazilian::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardBrazilian::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Board::Notation BoardBrazilian::GetNotation() const
{
    return Notation::ALGEBRAIC;
}

std::string BoardBrazilian::TileToNotation(const Tile &tile) const
{
    const int row = tile.GetRow();
    const int col = tile.GetCol();
    return _mapAlgebraicNotaion[col] + std::to_string(BOARD_SIZE - row);
}
