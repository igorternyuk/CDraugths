﻿#pragma once
#include "board.hpp"
#include "gamestatus.hpp"

namespace draughts
{
    class Player;
    class Game
    {
    public:
        static Game* GetInstance();
        enum class Mode
        {
            CPU_HUMAN,
            HUMAN_CPU,           
        };

        enum Type
        {
            DRAUGHTS64,
            POLISH,
            BRAZILIAN,
            CANADIAN,
            CHECKRERS,
            PORTUGUESE,
            FRISIAN,            
            TURKISH,
            NUM_OF_GAME_TYPES,
        };

        enum class Level
        {
            eBEGINNER,
            eVERY_EASY,
            eEASY,
            eMEDIUM,
            eHARD,
            eVERY_HARD,
            eHARDEST,
        };

    private:
        explicit Game();
        Game(const Game& game) = delete;
        Game(Game&& game) = delete;
        Game& operator=(const Game& game) = delete;
        Game& operator=(Game&& game) = delete;

    public:
        void SetupNewGame(Type type, Mode mode, int searchDepth = 8);
        void Play();
        int MapLevelToSearchDepth(Level level);
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
        int _searchDepth;
        Alliance _turn = Alliance::LIGHT;
        GameStatus _status = GameStatus::UNKNOWN;
        Move _lastMove;
    };
}


