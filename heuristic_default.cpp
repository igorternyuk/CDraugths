#include "heuristic_default.h"

using namespace draughts;

static constexpr int INF = 100000000;

HeuristicDefault::HeuristicDefault()
{

}

int HeuristicDefault::Evaluate(const Position &position, const Rules& rules)
{
    int score = 0;

    std::vector<Move> lightLegalMoves;
    position.LegalMoves(Alliance::LIGHT, lightLegalMoves);
    std::vector<Move> darkLegalMoves;
    position.LegalMoves(Alliance::DARK, darkLegalMoves);

    if(lightLegalMoves.empty())
        return -INF;
    if(darkLegalMoves.empty())
        return INF;

    score += 10*lightLegalMoves.size();
    score -= 10*darkLegalMoves.size();

    const int W = position.GetBoardWidth();
    const int H = position.GetBoardHeight();

    int num_dark_kings = 0;
    int num_light_kings = 0;

    const auto& piecesLight = position.GetPieces(Alliance::LIGHT);
    const auto& piecesDark = position.GetPieces(Alliance::DARK);

    for(const auto& [k, p]: piecesLight)
    {
        const Piece& piece = *p;
        const int x = piece.GetCol();
        const int y = piece.GetRow();
        const int pieceValue = rules.GetPieceValue(piece);

        if(piece.IsKing())
            num_light_kings++;
        score += pieceValue;
        score += (H - y);
        if(x == 0 || x == W - 1)
            score -= pieceValue / 2;

        if((x == 0 || x == W - 1) && y == H - 1)
            score -= pieceValue / 2;
        else if((x == 0 || x == W - 1) && y == H - 2)
            score -= pieceValue / 2;

        //golden piece
        if((x == W / 2 || x == W / 2 - 1) && y == H - 1)
            score += 3 * pieceValue;
    }

    for(const auto& [k, p]: piecesDark)
    {
        const Piece& piece = *p;
        const int x = piece.GetCol();
        const int y = piece.GetRow();
        const int pieceValue = rules.GetPieceValue(piece);

        if(piece.IsKing())
            num_dark_kings--;
        score -= pieceValue;
        score -= (y + 1);
        if(x == 0 || x == W - 1)
            score += pieceValue / 2;

        if((x == 0 || x == W - 1) && y == 0)
            score += pieceValue / 2;
        else if((x == 0 || x == W - 1) && y == 1)
            score += pieceValue / 2;

        //golden piece
        if((x == W / 2 || x == W / 2 - 1) && y == 0)
            score -= 3 * pieceValue;
    }

    score += 10 * (num_light_kings - num_dark_kings);
    return score;
}
