#pragma once
#include "game.hpp"
#include "drawhelper.hpp"

namespace view
{
    class View
    {
    public:
        static View* GetInstance();
        enum
        {
            TILE_SIZE_PX = 100,
            SCREEN_WIDTH = 660,
            SCREEN_HEIGHT = 660,
        };
    public:

        void OnMouseEvent(int button, int state, int x, int y);
        void OnMouseMotion(int x, int y);
        void OnKeyboardEvent(unsigned char key, int x, int y);
        void ProcessSpecialKeys(unsigned char key, int x, int y);
        void Update();
        void Render();
    public:
        static int GetWindowWidth();
        static int GetWindowHeight();
        static int GetDefaultWindowWidth();
        static int GetDefaultWindowHeight();
        draughts::Game *GetGame() const;

    private:
        explicit View();
        View(const View& game) = delete;
        View(View&& game) = delete;
        View& operator=(const View& game) = delete;
        View& operator=(View&& game) = delete;

    private:
        enum class ViewMode
        {
            eMenuGameType,
            eMenuGameMode,
            eMenuLevel,
            eBoard,
        };
    private:
        void FlippedCoordinates(int x, int y, int& fx, int& fy);
        void FlipBoard();
        int  SearchDepthByLevel(int level);
        void HighlightLastMove();
        void HighlightLegalMoves();
        void DrawGameStatus();
        void DrawBoard();
        void DrawPieces();
        void DrawMoveStep(const draughts::Step& step, DrawHelper::Color color);
        void DrawHumanPlayerSelection();
        void DrawPiece(const draughts::Piece& piece, DrawHelper::Color color, DrawHelper::Color color2, float pieceSizeRatio = 0.37f);
        void DrawCrown(const draughts::Piece& piece, DrawHelper::Color color);
        void DrawGameTypeMenu();
        void DrawGameModeMenu();
        void DrawLevelMenu();
    private:
        draughts::Game * _game;
        bool _bShowNotation = true;
        bool _bRotateBoard = false;
        ViewMode _viewMode = ViewMode::eMenuGameType;
        int _selectedGameType = (int)draughts::Game::Type::POLISH;
        int _selectedGameMode = (int)draughts::Game::Mode::HUMAN_CPU;
        int _selectedLevel = (int)draughts::Game::Level::eEASY;
        const std::string _sMenuTitleGameType = "WELLCOME TO THE GAME OF DRAUGHTS!";
        const std::string _sMenuTitleGameType2 = "SELECT GAME TYPE:";
        const std::string _menuItemsGameType[draughts::Game::NUM_OF_GAME_TYPES] { "DRAUGTHS64", "POLISH", "BRAZILIAN", "CANADIAN" };
        const std::string _sMenuTitleGameMode = "SELECT GAME MODE:";
        const std::string _menuItemsGameMode[2] { "PLAY WITH RED PIECES", "PLAY WITH BLUE PIECES",  };
        const std::string _sMenuTitleLevel= "SELECT OPPONENT STRENGTH:";
        const std::string _menuItemsLevel[4] { "VERY EASY", "EASY", "MEDIUM", "HARD" };

        DrawHelper::Color _colorNotation = {9, 65, 50};
        //const DrawHelper::Color colorLight = {255, 255, 136};
        const DrawHelper::Color _colorTileLight = {245, 245, 245};
        const DrawHelper::Color _colorTileDark = {93, 162, 45};
        const DrawHelper::Color _colorMenuTitle = {255, 0, 0};
        const DrawHelper::Color _colorMenuItem = {38, 144, 205};
        const DrawHelper::Color _colorMenuItemSelected = {255, 255, 0};
    };
}

