#include "board_canadian.hpp"

using namespace draughts;

BoardCanadian::BoardCanadian(): BoardInternational(BOARD_SIZE)
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

std::shared_ptr<Position> BoardCanadian::MakeCopy() const
{
    return std::make_shared<BoardCanadian>(*this);
}

int BoardCanadian::GetBoardSize() const
{
    return BOARD_SIZE;
}

int BoardCanadian::GetPieceRows() const
{
    return NUM_PIECE_ROWS;
}

Board::Notation BoardCanadian::GetNotation() const
{
    return Notation::NUMERIC;
}
