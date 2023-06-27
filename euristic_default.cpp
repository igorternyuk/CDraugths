#include "euristic_default.hpp"

using namespace draughts;

EuristicDefault::EuristicDefault()
{

}

int EuristicDefault::Evaluate(const Position &position, const Rules& rules)
{
    int score = 0;

    std::vector<Move> lightLegalMoves;
    position.LegalMoves(Alliance::LIGHT, lightLegalMoves);
    std::vector<Move> darkLegalMoves;
    position.LegalMoves(Alliance::DARK, lightLegalMoves);

    if(lightLegalMoves.empty())
        return -100000000;
    if(lightLegalMoves.empty())
        return 100000000;

    score += 10*lightLegalMoves.size();
    score -= 10*darkLegalMoves.size();

    const int BOARD_SIZE = position.GetBoardSize();

    int num_dark_kings = 0;
    int num_light_kings = 0;
    for(int y = 0; y < BOARD_SIZE; ++y)
    {
        for(int x = 0; x < BOARD_SIZE; ++x)
        {
            Tile currTile = position.GetTile(y,x);
            if(currTile.HasPiece())
            {
                const Piece& piece = currTile.GetPiece();
                const int pieceValue = rules.GetPieceValue(piece);

                if(currTile.GetPiece().GetAlliance() == Alliance::LIGHT)
                {
                    if(piece.IsKing())
                        num_light_kings++;
                    score += pieceValue;
                    score += (BOARD_SIZE - y);
                    if(x == 0 || x == BOARD_SIZE)
                        score -= pieceValue / 2;

                    //retrasadas
                    if(x == 0 && y == BOARD_SIZE - 1)
                        score -= pieceValue / 2;
                    else if(x == BOARD_SIZE -1 && y == BOARD_SIZE - 2)
                        score -= pieceValue / 2;

                    //golden piece
                    if(x == BOARD_SIZE / 2 && y == BOARD_SIZE - 1)
                        score += 3 * pieceValue;
                }
                else if(currTile.GetPiece().GetAlliance() == Alliance::DARK)
                {
                    if(piece.IsKing())
                        num_dark_kings--;
                    score -= pieceValue;
                    score -= (y + 1);
                    if(x == 0 || x == BOARD_SIZE)
                        score += pieceValue / 2;

                    //retrasadas
                    if(x == BOARD_SIZE - 1 && y == 0)
                        score += pieceValue / 2;
                    else if(x == 0 && y == 1)
                        score += pieceValue / 2;

                    //golden piece
                    if(x == BOARD_SIZE / 2 - 1 && y == 0)
                        score -= 3 * pieceValue;
                }
            }
        }
    }

    score += 10 * (num_light_kings - num_dark_kings);
    return score;
}
