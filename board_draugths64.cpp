#include "board_draugths64.hpp"
#include "rules_draughts64.hpp"
#include "rules.hpp"
#include <sstream>

using namespace draughts;

static std::map<int, std::string> algebraicNotaionFileMap
{
    { 0, "a"},
    { 1, "b"},
    { 2, "c"},
    { 3, "d"},
    { 4, "e"},
    { 5, "f"},
    { 6, "g"},
    { 7, "h"}
};

BoardDraugths64::BoardDraugths64(): Board(std::make_shared<RulesDraughts64>(), BOARD_SIZE)
{
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
        {
            if((c + r) % 2 != 0)
            {
                Tile& tile = GetTile(r, c);
                tile.SetDark();
            }
        }
    SetupInitialPosition();
}

std::shared_ptr<Position> BoardDraugths64::MakeCopy() const
{
    return std::make_shared<BoardDraugths64>(*this);
}

int BoardDraugths64::GetBoardSize() const
{
    return BOARD_SIZE;
}

bool BoardDraugths64::IsEndgameScenario() const
{
    Alliance turn = GetTurn();
    if(turn == Alliance::RED)
    {
        std::vector<Move> lolm_red;
        LegalMoves(Alliance::RED, lolm_red);
        return lolm_red.empty();
    }
    else if(turn == Alliance::BLUE)
    {
        std::vector<Move> lolm_blue;
        LegalMoves(Alliance::BLUE, lolm_blue);
        return lolm_blue.empty();
    }

    return false;
}

bool BoardDraugths64::MakeMove(const Move &move)
{
    if(Board::MakeMove(move))
    {
        _hash = GetHash();
        _mapRep[_hash]++;
        /////////////////////////////////////////////////////////////////////
        int count_red_pieces = 0, count_red_kings = 0, count_blue_pieces = 0, count_blue_kings = 0;
        CalcPieceCount(count_red_pieces, count_red_kings, count_blue_pieces, count_blue_kings);

        if((count_red_pieces == 0 && count_red_kings >= 3 && count_blue_kings == 1)
                || (count_blue_pieces == 0 && count_blue_kings >= 3 && count_blue_pieces == 0))
        {
            ++count15;
        }

        int reds_total = count_red_pieces + count_red_kings;
        int blues_total = count_blue_pieces + count_blue_kings;
        int total = reds_total + blues_total;

        int balance = (count_red_pieces + 3 * count_red_kings) - (count_blue_pieces + 3 * count_blue_kings);
        if(total == 2 || total == 3)
        {
            count45 = 0;
            count67 = 0;
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count23 = 0;
            }
            else
            {
                ++count23;
            }
        }
        else if(total == 4 || total == 6)
        {
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count45 = 0;
            }
            else
            {
                ++count45;
            }
        }
        else if(total == 6 || total == 7)
        {
            count67 = 0;
            if(balance != oldBalance)
            {
                oldBalance = balance;
                count67 = 0;
            }
            else
            {
                ++count67;
            }
        }

         return true;
    }

    return false;
}

void BoardDraugths64::SetupInitialPosition()
{
    Reset();
    Clear();
    for(int y = 0; y < NUM_CHECKER_ROW_FOR_ONE_SIDE; ++y)
        for(int x = (y + 1) % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::BLUE);


    for(int y = BOARD_SIZE - NUM_CHECKER_ROW_FOR_ONE_SIDE; y < BOARD_SIZE; ++y)
        for(int x = (y + 1) % 2; x < BOARD_SIZE; x += 2)
            SetPiece(y,x,Alliance::RED);
}

GameStatus BoardDraugths64::GetGameStatus() const
{
    if(!_mapRep.empty() && _mapRep.find(_hash) != _mapRep.end() )
    {
        if(_mapRep.at(_hash) >= 3)
        //std::cout << "Draw by threefold repetition!\n";
        return GameStatus::DRAW;
    }

    if(count15 >= 2 * 15)
    {
        //std::cout << "No king capture within 15 moves!\n";
        return GameStatus::DRAW;
    }

    if(count23 >= 2 * 5)
    {
        //std::cout << "No captures or coronations in the last 5 moves for 2-3 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(count45 >= 2 * 30)
    {
        //std::cout << "No captures or coronations in the last 30 moves for 4-5 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(count67 >= 2 * 60)
    {
        //std::cout << "No captures or coronations in the last 60 moves for 6-7 piece ending!\n";
        return GameStatus::DRAW;
    }

    if(_moveLog.size() > 30)
    {
        int count = 0;
        for(auto it = _moveLog.rbegin(); it != _moveLog.rend(); ++it)
        {
            const Move& move = *it;
            for(size_t i = 0; i < move.StepCount(); ++i)
            {
                if(move.GetStep(i).GetStart().GetPiece().IsKing())
                    count++;
            }
        }

        if(count >= 30)
            return GameStatus::DRAW;
    }

    return Board::GetGameStatus();
}

std::string BoardDraugths64::TileToAlgebraicNotation(const Tile &tile) const
{
    int row = tile.GetRow();
    int col = tile.GetCol();
    return algebraicNotaionFileMap[col] + std::to_string(BOARD_SIZE - row);
}

std::string BoardDraugths64::MoveToAlgebraicNotation(const Move &move) const
{
    if(move.IsEmpty()) return "";
    const Step& step = move.GetStep(0);
    const Tile& tileStart = step.GetStart();
    std::string notation = TileToAlgebraicNotation(tileStart);
    const int stepCount = move.StepCount();
    if(step.isJump())
    {
        for(int i = 0; i < stepCount; ++i)
        {
            notation += ":" + TileToAlgebraicNotation(step.GetEnd());
        }
    }
    else
    {
        notation +=  "-" + TileToAlgebraicNotation(step.GetEnd());
    }
    return notation;
}

std::string BoardDraugths64::ToString() const
{
    std::stringstream ss;
    for(int y = 0; y < BOARD_SIZE; ++y)
    {
        std::string line;
        for(int x = 0; x < BOARD_SIZE; ++x)
        {
            const Tile& tile = GetTile(y,x);
            if(tile.HasPiece())
            {
                Piece p = tile.GetPiece();
                if(p.GetAlliance() == Alliance::RED)
                {
                    line += p.IsKing() ? "[R]" : "[r]";
                }
                else if(p.GetAlliance() == Alliance::BLUE)
                {
                    line += p.IsKing() ? "[B]" : "[b]";
                }
            }
            else
            {
                line += tile.IsDark() ?  "[.]" : "[ ]";
            }
        }
        line += "\n";
        ss << line;
    }
    return ss.str();
}

int BoardDraugths64::GetTotalPieces() const
{
    int count_red_pieces = 0, count_red_kings = 0, count_blue_pieces = 0, count_blue_kings = 0;
    CalcPieceCount(count_red_pieces, count_red_kings, count_blue_pieces, count_blue_kings);
    return count_red_pieces + count_red_kings + count_blue_pieces + count_blue_kings;
}
