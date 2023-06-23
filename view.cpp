#include "view.hpp"
#include "player_human.hpp"
#include <GL/glut.h>
#include <iostream>

#define KEY_RETURN 13

using namespace view;
using namespace draughts;

View::View():_game(Game::GetInstance())
{

}

View* View::GetInstance()
{
    static View view;
    return &view;
}

void View::OnMouseEvent(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        Game* game = Game::GetInstance();
        const Board& board = game->GetBoard();
        Alliance turn = game->GetTurn();
        std::shared_ptr<Player> player = game->GetPlayer(turn);

        if(player->IsHuman())
        {
            auto ptrPlayerHuman = std::dynamic_pointer_cast<PlayerHuman>(player);
            if (ptrPlayerHuman)
            {
                int mx = x / TILE_SIZE_PX;
                int my = y / TILE_SIZE_PX;
                std::cout << "mx = " << mx << " my = " << my << std::endl;
                if(button == GLUT_LEFT_BUTTON)
                {
                    ptrPlayerHuman->Select(board, mx, my);
                }
                else if(button == GLUT_RIGHT_BUTTON)
                {
                    ptrPlayerHuman->Unselect();
                }
            }
        }
    }
}

void View::OnKeyboardEvent(unsigned char key, int x, int y)
{
    if(key == KEY_RETURN)//enter
    {
        _game->Play();
        //Resize window
        //glViewport(0, 0, width, height)
        //glScissor(0, 0, width, height)
        //glOrtho(0,WINDOW_WIDTH,WINDOW_HEIGHT,0,-1,1);
    }
    else if(key == ' ')
    {
        std::cout << "Last move undo" << std::endl;
        _game->UndoLastMove();
    }
    else if(key == 'a')
    {
        _game->SetupNewGame(Game::Type::DRAUGHTS64, Game::Mode::HUMAN_CPU);
        _game->Play();
    }
}

void View::Update()
{
    _game->Update();
}

void View::Render()
{
    DrawBoard();
    DrawHumanPlayerSelection();
    DrawPieces();
    GameStatus status = _game->GetStatus();
    if(status == GameStatus::PLAY)
        HighlightLegalMoves();
    HighlightLastMove();
    DrawGameStatus();
}

Game *View::GetGame() const
{
    return _game;
}

int View::GetWindowWidth()
{
    View* view = View::GetInstance();
    Game* game = view->GetGame();
    const Board& board = game->GetBoard();
    int sz = board.GetBoardSize();
    return sz * TILE_SIZE_PX;
}

int View::GetWindowHeight()
{
    View* view = View::GetInstance();
    Game* game = view->GetGame();
    const Board& board = game->GetBoard();
    int sz = board.GetBoardSize();
    return sz * TILE_SIZE_PX;
}

void View::HighlightLastMove()
{
    Move lastMove = _game->GetLastMove();
    const int stepCount = lastMove.StepCount();
    DrawHelper::Color color = {211,204,0};
    for(int i = 0; i < stepCount; ++i)
    {
         const Step& step = lastMove.GetStep(i);
        DrawMoveStep(step, color);
    }
}

void View::HighlightLegalMoves()
{
    GameStatus status = _game->GetStatus();
    if(status == GameStatus::PLAY)
    {
        std::vector<Move> lolm;
        Alliance turn = _game->GetTurn();
        Game::GetInstance()->GetBoard().LegalMoves(turn, lolm);
        DrawHelper::Color color = {0,127,0};
        for(auto &m: lolm)
        {
            const int stepCount = m.StepCount();
            for(int i = 0; i < stepCount; ++i)
            {
                const Step& step = m.GetStep(i);
                DrawMoveStep(step, color);
            }
        }
    }
}

void View::DrawGameStatus()
{
    GameStatus status = _game->GetStatus();
    if(status == GameStatus::PLAY)
    {
        Alliance turn = _game->GetTurn();
        if(turn == Alliance::RED)
            DrawHelper::DrawWord("REDS TO PLAY", 5, 20, 20, {255,0,0});
        else if(turn == Alliance::BLUE)
            DrawHelper::DrawWord("BLUES TO PLAY", 5, 20, 20, {0,0,255});
    }
    else if(status == GameStatus::RED_WON)
    {
        DrawHelper::DrawWord("RED PLAYER WON!", 5, 20, 20, {255,0,0});
    }
    else if(status ==GameStatus::BLUE_WON)
    {
        DrawHelper::DrawWord("BLUE PLAYER WON!", 5, 20, 20, {0,0,255});
    }
    else if(status == GameStatus::DRAW)
    {
        DrawHelper::DrawWord("DRAW!", 5, 20, 20, {0,255,0});
    }
}

void View::DrawBoard()
{
    int topLeftX = 0;
    int topLeftY = 0;
    //const DrawHelper::Color colorLight = {255, 255, 136};
    const DrawHelper::Color colorLight = {245, 245, 245};
    const DrawHelper::Color colorDark = {93, 162, 45};
    Game* game = Game::GetInstance();
    Board& board = game->GetBoard();
    const int boardSize = board.GetBoardSize();
    DrawHelper::DrawFilledRect(topLeftX, topLeftY, TILE_SIZE_PX * boardSize, TILE_SIZE_PX * boardSize, colorLight);

    for(int y = 0; y < boardSize; ++y)
    {
        for(int x = 0; x < boardSize; ++x)
        {
            const Tile& tile = board.GetTile(y, x);
            if(tile.IsDark())
                DrawHelper::DrawFilledRect(topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX, colorDark);
        }
    }
}

void View::DrawPieces()
{
    Board& board = _game->GetBoard();

    const DrawHelper::Color colorRedPiece = {255, 0, 0};
    const DrawHelper::Color colorBluePiece = {0,109,211};
    const float pieceSizeRatio = 0.37f;

    auto redPieces = board.GetPieces(Alliance::RED);
    auto bluePieces = board.GetPieces(Alliance::BLUE);

    for(const auto& [i, p] : redPieces)
        DrawPiece(*p, colorRedPiece, pieceSizeRatio);

    for(const auto& [i, p] : bluePieces)
        DrawPiece(*p, colorBluePiece, pieceSizeRatio);
}

void View::DrawMoveStep(const Step &step, DrawHelper::Color color)
{
    double x1 = step.GetStart().GetCol() * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double y1 = step.GetStart().GetRow() * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double x2 = step.GetEnd().GetCol() * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double y2 = step.GetEnd().GetRow() * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    DrawHelper::DrawArrow(x1, y1, x2, y2, color);
}

void View::DrawHumanPlayerSelection()
{
    const DrawHelper::Color colorSelection = {255,255,0};
    Game* game = Game::GetInstance();
    Alliance turn = game->GetTurn();
    std::shared_ptr<Player> player = game->GetPlayer(turn);
    if(player->IsHuman())
    {
        auto ptrPlayerHuman = std::dynamic_pointer_cast<PlayerHuman>(player);
        if (ptrPlayerHuman)
        {
            auto selection = ptrPlayerHuman->GetSelection();
            for(const auto& p : selection)
            {
                DrawHelper::DrawFilledRect(p.second * TILE_SIZE_PX, p.first * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX, colorSelection);
            }
        }
    }
}

void View::DrawPiece(const Piece &piece, DrawHelper::Color color, float pieceSizeRatio)
{
    const float x = piece.GetCol();
    const float y = piece.GetRow();
    const float cx = x * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    const float cy = y * TILE_SIZE_PX + TILE_SIZE_PX / 2;

   DrawHelper::DrawPolygon(cx, cy, pieceSizeRatio * TILE_SIZE_PX, 36, color);
    if(piece.IsKing())
        DrawCrown(piece, {255,255,0});
}

void View::DrawCrown(const Piece &piece, DrawHelper::Color color)
{
    const float x = piece.GetCol();
    const float y = piece.GetRow();
    const float cx = x * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    const float cy = y * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    const float bottomLeftX = cx - TILE_SIZE_PX / 6;
    const float bottomRightX = cx + TILE_SIZE_PX / 6;
    const float bottomY = cy + TILE_SIZE_PX / 6;
    const float topY = cy - TILE_SIZE_PX / 4;
    const float leftX = bottomLeftX - TILE_SIZE_PX / 12;
    const float rightX = bottomRightX + TILE_SIZE_PX / 12;
    const float topSides = topY + TILE_SIZE_PX / 12;

    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    glBegin(GL_POLYGON);
    glVertex2f(bottomLeftX, bottomY);
    glVertex2f(leftX, topSides);
    glVertex2f(bottomRightX, bottomY);
    glEnd();

    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    glBegin(GL_POLYGON);
    glVertex2f(bottomLeftX, bottomY);
    glVertex2f(cx, topY);
    glVertex2f(bottomRightX, bottomY);
    glEnd();

    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    glBegin(GL_POLYGON);
    glVertex2f(bottomLeftX, bottomY);
    glVertex2f(rightX, topSides);
    glVertex2f(bottomRightX, bottomY);
    glEnd();
}
