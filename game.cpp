#include "game.h"
#include "board_ru.h"
#include "board_polish.h"
#include "board_brazilian.h"
#include "board_canadian.h"
#include "board_checkers.h"
#include "board_italian.h"
#include "board_portuguese.h"
#include "board_frisian.h"
#include "board_turkish.h"
#include "board_armenian_tamas.h"
#include "board_diagonal64.h"
#include "board_diagonal100.h"
#include "player_human.h"
#include "player_alphabeta.h"
#include "heuristic_diagonal.h"
#include "rules.h"
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <iostream>

using namespace draughts;

Game *Game::GetInstance()
{
    static Game game;
    return &game;
}

Game::Game():_mode(Mode::HUMAN_CPU), _type(Type::POLISH)
{
    //SetupNewGame(_type, _mode, Level::eEASY);
}

Board &Game::GetBoard()
{
    return *_board;
}

GameStatus Game::GetStatus()
{
    return _board->GetGameStatus();
}

void Game::Update()
{
    if(_status == GameStatus::PLAY)
    {
        Alliance turn = GetTurn();
        std::shared_ptr<Player> player = GetPlayer(turn);
        Move move = player->MakeMove(*_board);
        if(move.GetStatus() != Move::Status::ILLEGAL_MOVE)
        {
            if(_board->MakeMove(move))
            {
                SwitchTurn();
                UpdateGameStatus();
                _lastMove = move;
            }
        }
    }
}

void Game::UndoLastMove()
{
    auto log = _board->GetMoveLog();
    if(!log.empty())
    {
        _board->UndoLastMove();
        _lastMove = log.back();
        SwitchTurn();
    }
}

void Game::UpdateGameStatus()
{
    _status = _board->GetGameStatus();
}

void Game::SwitchTurn()
{
    this->_turn = draughts::OpponentAlliance(_turn);
}

void Game::Play()
{
    SetupNewGame(_type, _mode, _searchDepth);
}

void Game::SetupNewGame(Type type, Mode mode, int searchDepth)
{
    //TODO add all remaining game types
    if(type == Type::DRAUGHTS64)
    {
        _board = std::make_shared<BoardRu>();
    }
    else if(type == Type::POLISH)
    {
        _board = std::make_shared<BoardPolish>();
    }
    else if(type == Type::BRAZILIAN)
    {
        _board = std::make_shared<BoardBrazilian>();
    }
    else if(type == Type::CANADIAN)
    {
        _board = std::make_shared<BoardCanadian>();
    }
    else if(type == Type::CHECKERS)
    {
        _board = std::make_shared<BoardCheckers>();
    }
    else if(type == Type::ITALIAN)
    {
        _board = std::make_shared<BoardItalian>();
    }
    else if(type == Type::PORTUGUESE)
    {
        _board = std::make_shared<BoardPortuguese>();
    }
    else if(type == Type::FRISIAN)
    {
        _board = std::make_shared<BoardFrisian>();
    }
    else if(type == Type::TURKISH)
    {
        _board = std::make_shared<BoardTurkish>();
    }
    else if(type == Type::ARMENIAN)
    {
        _board = std::make_shared<BoardArmenianTamas>();
    }
    else if(type == Type::DRAUGHTS80)
    {
        _board = std::make_shared<BoardRu>(10,8);
    }
    else if(type == Type::DIAGONAL64)
    {
        _board = std::make_shared<BoardDiagonal64>();
    }
    else if(type == Type::DIAGONAL100)
    {
        _board = std::make_shared<BoardDiagonal100>();
    }

    if(mode == Mode::HUMAN_CPU)
    {
        _playerLight = std::make_shared<PlayerHuman>(Alliance::LIGHT);
        if(type == Type::DIAGONAL64 == type == Type::DIAGONAL100)
            _playerDark = std::make_shared<PlayerAlphaBeta>(Alliance::DARK, searchDepth, std::make_shared<HeuristicDiagonal>());
        else
            _playerDark = std::make_shared<PlayerAlphaBeta>(Alliance::DARK, searchDepth);
    }
    else if(mode == Mode::CPU_HUMAN)
    {
        if(type == Type::DIAGONAL64 == type == Type::DIAGONAL100)
            _playerLight = std::make_shared<PlayerAlphaBeta>(Alliance::LIGHT, searchDepth,std::make_shared<HeuristicDiagonal>());
        else
            _playerLight = std::make_shared<PlayerAlphaBeta>(Alliance::LIGHT, searchDepth);
        _playerDark = std::make_shared<PlayerHuman>(Alliance::DARK);
    }

    _mode = mode;
    _type = type;
    _searchDepth = searchDepth;
    _turn = _board->GetRules()->FirstMoveAlliance();
    _status = GameStatus::PLAY;
    Reset();
}

void Game::PrintBoard()
{
    std::cout << "//////////// BOARD ////////////" << std::endl;
    std::cout << _board->ToString() << std::endl;
    std::cout << "///////////////////////////////" << std::endl;
}

std::vector<std::pair<int,int>> Game::GetSelected() const
{
    std::vector<std::pair<int,int>> selection;
    Alliance turn = GetTurn();
    std::shared_ptr<Player> player = GetPlayer(turn);
    if(player->IsHuman())
    {
        auto ptrPlayerHuman = std::dynamic_pointer_cast<PlayerHuman>(player);
        if (ptrPlayerHuman)
        {
            selection = ptrPlayerHuman->GetSelection();
        }
    }
    return selection;
}

int Game::MapLevelToSearchDepth(Level level)
{
    int searchDepth = 5;
    switch(level)
    {
        case Level::eBEGINNER:
            searchDepth = 4;
            break;
        case Level::eEASY:
            searchDepth = 5;
            break;
        case Level::eMEDIUM:
            searchDepth = 6;
            break;
        case Level::eHARD:
            searchDepth = 7;
            break;
        case Level::eVERY_HARD:
            searchDepth = 8;
            break;
        case Level::eHARDEST:
            searchDepth = 9;
            break;
            break;
        default:
            searchDepth = 5;
    }

    return searchDepth;
}
void Game::Reset()
{
    _board->SetupInitialPosition();
    _turn = _board->GetRules()->FirstMoveAlliance();
    _status = GameStatus::PLAY;
    _lastMove.Clear();
}

Alliance Game::GetTurn() const
{
    return _turn;
}

const Move &Game::GetLastMove() const
{
    return _lastMove;
}

const std::shared_ptr<Player> &Game::GetPlayer(Alliance alliance) const
{
    return alliance == Alliance::DARK ? _playerDark : _playerLight;
}

Game::Type Game::GetType() const {
    return _type;
}

