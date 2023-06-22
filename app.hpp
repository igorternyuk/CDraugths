#pragma once
#include "drawhelper.hpp"
#include "step.hpp"

class App
{
public:
    App();
public:
    void Run(int argc, char *argv[]);
private:
    enum
    {
        TILE_SIZE_PX = 100
    };
    static int GetWindowWidth();
    static int GetWindowHeight();
    //Handle user's input
    static void MouseFunc(int button, int state, int x, int y);
    static void KeyboardFunc(unsigned char key, int x, int y);
    //Update game state
    static void Timer(int);
    static void Update();
    //Drawing functions
    static void DisplayFunc();
    static void HighlightLastMove();
    static void HighlightLegalMoves();
    static void DrawGameStatus();
    static void DrawBoard();
    static void DrawPieces();
    static void DrawMoveStep(const Step& step, DrawHelper::Color color);
    static void DrawHumanPlayerSelection();
    static void DrawPiece(const Piece& piece, DrawHelper::Color color, float pieceSizeRatio = 0.37f);
    static void DrawCrown(const Piece& piece, DrawHelper::Color color);
private:
};
