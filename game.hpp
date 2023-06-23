﻿#pragma once
#include "board.hpp"
#include "gamestatus.hpp"

class Player;
class Game
{    
public:
    static Game* GetInstance();
    enum class Mode
    {
        HUMAN_CPU,
        CPU_HUMAN,
        CPU_CPU,
        HUMAN_HUMAN
    };

    enum class Type
    {
        DRAUGHTS64,
        POLISH,
        BRAZILIAN,
        PORTUGUESE,
        CHECKRERS,
        TURKISH,
    };

private:
    explicit Game();
    Game(const Game& game) = delete;
    Game(Game&& game) = delete;
    Game& operator=(const Game& game) = delete;
    Game& operator=(Game&& game) = delete;

public:
    void SetupNewGame(Type type, Mode mode);
    void Play();
    Board &GetBoard();
    GameStatus GetStatus();
    void Update();
    void UndoLastMove();
    Alliance GetTurn() const;
    const Move &GetLastMove() const;
    const std::shared_ptr<Player> &GetPlayer(Alliance alliance) const;
    Move::Status MakeMove(const Move& move);
    Alliance getHumanPlayer() const;
    Alliance getAiPlayer() const;
    Alliance getCurrentPlayer() const;
    void PrintBoard();
    std::vector<std::pair<int,int>> GetSelected() const; //TODO return vector of Tiles here
private:
    void UpdateGameStatus();
    void SwitchTurn();
    void Reset();
private:
    std::shared_ptr<Board> _board;
    std::shared_ptr<Player> _playerRed;
    std::shared_ptr<Player> _playerBlue;
    Mode _mode;
    Type _type;
    Alliance _turn = Alliance::RED;
    GameStatus _status = GameStatus::PLAY;
    Move _lastMove;
};
