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
            SCREEN_WIDTH = 480,
            SCREEN_HEIGHT = 480,
        };
    public:

        void OnMouseEvent(int button, int state, int x, int y);
        void OnMouseMotion(int x, int y);
        void OnKeyboardEvent(unsigned char key, int x, int y);
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
            eMenuOpponentStrength,
            eBoard,
        };
    private:
        void FlippedCoordinates(int x, int y, int& fx, int& fy);
        void FlipBoard();
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
    private:
        draughts::Game * _game;
        bool _bShowNotation = true;
        bool _bRotateBoard = false;
        ViewMode _viewMode = ViewMode::eMenuGameType;
        int _selectedGameMode = 0;
        DrawHelper::Color _colorNotation = {9, 65, 50};
        //const DrawHelper::Color colorLight = {255, 255, 136};
        const DrawHelper::Color _colorTileLight = {245, 245, 245};
        const DrawHelper::Color _colorTileDark = {93, 162, 45};
        const DrawHelper::Color _colorMenuTitle = {255, 0, 0};
        const DrawHelper::Color _colorMenuItem = {38, 144, 205};
        const DrawHelper::Color _colorMenuItemSelected = {255, 255, 0};
    };
}

