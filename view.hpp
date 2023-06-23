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
            TILE_SIZE_PX = 100
        };
    public:

        void OnMouseEvent(int button, int state, int x, int y);
        void OnKeyboardEvent(unsigned char key, int x, int y);
        void Update();
        void Render();
    public:
        static int GetWindowWidth();
        static int GetWindowHeight();
        draughts::Game *GetGame() const;

    private:
        explicit View();
        View(const View& game) = delete;
        View(View&& game) = delete;
        View& operator=(const View& game) = delete;
        View& operator=(View&& game) = delete;

    private:
        void HighlightLastMove();
        void HighlightLegalMoves();
        void DrawGameStatus();
        void DrawBoard();
        void DrawPieces();
        void DrawMoveStep(const draughts::Step& step, DrawHelper::Color color);
        void DrawHumanPlayerSelection();
        void DrawPiece(const draughts::Piece& piece, DrawHelper::Color color, float pieceSizeRatio = 0.37f);
        void DrawCrown(const draughts::Piece& piece, DrawHelper::Color color);

    private:
        draughts::Game * _game;
    };
}

