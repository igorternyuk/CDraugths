#include "heuristic_diagonal.h"

using namespace draughts;

static constexpr int INF = 100000000;

HeuristicDiagonal::HeuristicDiagonal()
{

}

int HeuristicDiagonal::Evaluate(const Position &position, const Rules& rules)
{
    int score = 0;

    std::vector<Move> lightLegalMoves;
    position.LegalMoves(Alliance::LIGHT, lightLegalMoves);
    std::vector<Move> darkLegalMoves;
    position.LegalMoves(Alliance::DARK, lightLegalMoves);

    const std::vector<std::tuple<int,int>>& coronationTilesLight = position.GetCoronationTiles(Alliance::LIGHT);
    const std::vector<std::tuple<int,int>>& coronationTilesDark = position.GetCoronationTiles(Alliance::DARK);

    if(lightLegalMoves.empty())
        return -INF;
    if(lightLegalMoves.empty())
        return INF;

    score += 10*lightLegalMoves.size();
    score -= 10*darkLegalMoves.size();

    const int W = position.GetBoardWidth();
    const int H = position.GetBoardHeight();

    int num_dark_kings = 0;
    int num_light_kings = 0;

    const auto& piecesLight = position.GetPieces(Alliance::LIGHT);
    const auto& piecesDark = position.GetPieces(Alliance::DARK);

    enum
    {
        ROW = 0,
        COL = 1
    };
    for(const auto& [k, p]: piecesLight)
    {
        const Piece& piece = *p;
        const int px = piece.GetCol();
        const int py = piece.GetRow();
        const int pieceValue = rules.GetPieceValue(piece);

        if(piece.IsKing())
            num_light_kings++;
        score += pieceValue;

        const Tile& currTile = position.GetTile(py, px);
        for(const auto& loc: coronationTilesLight)
        {
            int r, c;
            std::tie(r,c) = loc;
            const Tile& coronationTile = position.GetTile(r, c);
            score -= currTile.DistanceTo(coronationTile);
        }
    }

    for(const auto& [k, p]: piecesDark)
    {
        const Piece& piece = *p;
        const int px = piece.GetCol();
        const int py = piece.GetRow();
        const int pieceValue = rules.GetPieceValue(piece);

        if(piece.IsKing())
            num_dark_kings--;
        score -= pieceValue;
        const Tile& currTile = position.GetTile(py, px);
        for(const auto& loc: coronationTilesLight)
        {
            int r, c;
            std::tie(r,c) = loc;
            const Tile& coronationTile = position.GetTile(r, c);
            score += currTile.DistanceTo(coronationTile);
        }
    }

    score += 10 * (num_light_kings - num_dark_kings);
    return score;
}
