#include "game.hpp"
#include "board_draugths64.hpp"
#include "player_human.hpp"
#include "player_alphabeta.hpp"
#include "rules.hpp"
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <iostream>

Game *Game::GetInstance()
{
    static Game game;
    return &game;
}

Game::Game():_mode(Mode::CPU_HUMAN), _type(Type::DRAUGHTS64)
{
    _board = std::make_shared<BoardDraugths64>();
    _playerRed = std::make_shared<PlayerHuman>(Alliance::RED);
    _playerBlue = std::make_shared<PlayerAlphaBeta>(Alliance::BLUE, 8);
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
        _lastMove = log.back();;
        SwitchTurn();
    }
}

void Game::UpdateGameStatus()
{
    _status = _board->GetGameStatus();
}

void Game::SwitchTurn()
{
    this->_turn = OpponentAlliance(_turn);
}

void Game::Play()
{
    SetupNewGame(_type, _mode);
    Reset();
}

void Game::SetupNewGame(Type type, Mode mode)
{
    //TODO add all remaining game types
    if(type == Type::DRAUGHTS64)
    {
        _board = std::make_shared<BoardDraugths64>();
    }

    if(mode == Mode::HUMAN_CPU)
    {
        _playerRed = std::make_shared<PlayerHuman>(Alliance::RED);
        _playerBlue = std::make_shared<PlayerAlphaBeta>(Alliance::BLUE, 8);
    }

    _mode = mode;
    _type = type;
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
    return alliance == Alliance::RED ? _playerRed : _playerBlue;
}

