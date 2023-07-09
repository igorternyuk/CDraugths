#include "board_diagonal100.h"

using namespace draughts;

BoardDiagonal100::BoardDiagonal100()
{

}

void BoardDiagonal100::SetupInitialPosition()
{
    const int HEIGHT = GetBoardHeight();
    const int WIDTH = GetBoardWidth();
    const int N = std::min(HEIGHT, WIDTH);
    Reset();
    Clear();
    for(int y = 0; y < HEIGHT; ++y)
        for(int x = 0; x < WIDTH; ++x)
        {
            if(x + y < N - 1 && GetTile(y,x).IsDark())
                SetPiece(y,x,Alliance::DARK);
            else if(x + y > N - 1 && GetTile(y,x).IsDark())
                SetPiece(y,x,Alliance::LIGHT);
        }
}

bool BoardDiagonal100::IsCoronationTile(int row, int col, Alliance alliance) const
{
    const int HEIGHT = GetBoardHeight();
    const int WIDTH = GetBoardWidth();
    const int N = std::min(HEIGHT, WIDTH);
    bool isInLeftTopCorner = col >= 0 && col <= N / 2 && row >= 0 && row <= N / 2;
    bool isInRightBottomCorner = col >= N / 2 - 1 && col < N  && row >= N / 2 - 1 && row < N;
    bool isUpperOfMainDiagonal = row + col < N - 1;
    bool isLowerOfMainDiagonal = row + col > N - 1;

    if(alliance == Alliance::LIGHT)
        return isInLeftTopCorner && isUpperOfMainDiagonal && (row == 0 || col == 0);
    else if(alliance == Alliance::DARK)
        return isInRightBottomCorner && isLowerOfMainDiagonal && (row == N - 1  || col == N - 1 );

    return false;

}
