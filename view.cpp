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

void View::FlippedCoordinates(int x, int y, int &fx, int &fy)
{
    const Board& board = _game->GetBoard();
    const int boardSize = board.GetBoardSize();
    fx = boardSize - 1 - x;
    fy = boardSize - 1 - y;
}

void View::FlipBoard()
{
    _bRotateBoard = !_bRotateBoard;
}

View* View::GetInstance()
{
    static View view;
    return &view;
}

void View::OnMouseEvent(int button, int state, int x, int y)
{
    if(_viewMode == ViewMode::eBoard)
    {
        if(state == GLUT_DOWN)
        {
            const Board& board = _game->GetBoard();
            Alliance turn = _game->GetTurn();
            std::shared_ptr<Player> player = _game->GetPlayer(turn);

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
                        if(_bRotateBoard)
                        {
                            int fx, fy;
                            FlippedCoordinates(mx, my, fx, fy);
                            ptrPlayerHuman->Select(board, fx, fy);
                        }
                        else
                        {
                            ptrPlayerHuman->Select(board, mx, my);
                        }
                    }
                    else if(button == GLUT_RIGHT_BUTTON)
                    {
                        ptrPlayerHuman->Unselect();
                    }
                }
            }
        }
    }
    else if(_viewMode == ViewMode::eMenuGameType)
    {
        if(state == GLUT_DOWN)
        {
            if(button == GLUT_LEFT_BUTTON)
            {
                _viewMode = ViewMode::eBoard;
                _game->SetupNewGame((Game::Type)_selectedGameMode, Game::Mode::HUMAN_CPU, 6);

                const int width = GetWindowWidth();
                const int height = GetWindowHeight();

                glViewport(0, 0, width, height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0,width,height,0,-1,1);
                glutReshapeWindow(width, height);
                glutPostRedisplay();
            }
        }
    }

}

void View::OnMouseMotion(int x, int y)
{
    _selectedGameMode = std::min(3, ((y - 80) / 25));
}

void View::DrawGameTypeMenu()
{
    DrawHelper::DrawWord("SELECT GAME TYPE:", 70, 20, 20, _colorMenuTitle);
    const char* menuItems[] { "DRAUGTHS64", "POLISH", "BRAZILIAN", "CANADIAN" };

    for(int i = 0; i < 4; ++i)
    {
        DrawHelper::DrawWord(menuItems[i], SCREEN_WIDTH / 4, 80 + i * 40, 20, i == _selectedGameMode ? _colorMenuItemSelected : _colorMenuItem);
    }
}

void View::DrawGameModeMenu()
{
    DrawHelper::DrawWord("RED PLAYER WON!", 5, 20, 20, {255,0,0});
}

void View::OnKeyboardEvent(unsigned char key, int x, int y)
{
    bool bNeedToResizeWindow = false;
    bool bFlipChanged = false;
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(key == KEY_RETURN)//enter
    {
        _game->Play();
    }
    else if(key == ' ')
    {
        std::cout << "Last move undo" << std::endl;
        _game->UndoLastMove();
    }
    else if(key == '1')
    {
        _game->SetupNewGame(Game::Type::DRAUGHTS64, Game::Mode::CPU_HUMAN, 6);
        bNeedToResizeWindow = true;
    }
    else if(key == '2')
    {
        _game->SetupNewGame(Game::Type::POLISH, Game::Mode::HUMAN_CPU, 6);
        _bRotateBoard = false;
        bNeedToResizeWindow = true;
    }
    else if(key == '3')
    {
        _game->SetupNewGame(Game::Type::POLISH, Game::Mode::CPU_HUMAN, 6);
        _bRotateBoard = true;
        bNeedToResizeWindow = true;
    }
    else if(key == 'r')
    {
        FlipBoard();
        bFlipChanged = true;
    }
    else if(key == 'm')
    {
        _viewMode = ViewMode::eMenuGameType;
        width = GetDefaultWindowWidth();
        height = GetDefaultWindowHeight();
        bNeedToResizeWindow = true;
    }
    if(bNeedToResizeWindow)
    {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,width,height,0,-1,1);
        glutReshapeWindow(width, height);
    }

    if(bFlipChanged)
    {
        glutPostRedisplay();
    }
}

void View::Update()
{
    _game->Update();
}

void View::Render()
{
    if(_viewMode == ViewMode::eMenuGameType)
    {
        DrawGameTypeMenu();
    }
    else if(_viewMode == ViewMode::eMenuGameMode)
    {
        DrawGameModeMenu();
    }
    else
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

int View::GetDefaultWindowWidth()
{
    return SCREEN_WIDTH;
}

int View::GetDefaultWindowHeight()
{
    return SCREEN_HEIGHT;
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

    Game* game = Game::GetInstance();
    Board& board = game->GetBoard();
    const int boardSize = board.GetBoardSize();
    DrawHelper::DrawFilledRect(topLeftX, topLeftY, TILE_SIZE_PX * boardSize, TILE_SIZE_PX * boardSize, _colorTileLight);

    for(int yy = 0; yy < boardSize; ++yy)
    {
        for(int xx = 0; xx < boardSize; ++xx)
        {
            int x, y;
            if(_bRotateBoard)
                FlippedCoordinates(xx, yy, x, y);
            else
            {
                x = xx;
                y = yy;
            }
            const Tile& tile = board.GetTile(yy, xx);
            if(tile.IsDark())
            {
                DrawHelper::DrawFilledRect(topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX, _colorTileDark);
            }
            //Draw notation

            if(_bShowNotation)
            {
                auto notation = board.GetNotation();
                if(notation == Board::Notation::ALGEBRAIC)
                {
                    std::string sNotation = board.TileToNotation(tile);
                    if(x == 0)
                    {
                        std::string sRank = sNotation.substr(1, 1);
                        const char* rank = sRank.c_str();
                        DrawHelper::DrawWord(rank, topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX * 3 / 5, 10, _colorNotation);
                    }
                    if(y == boardSize - 1)
                    {
                        std::string sFile = sNotation.substr(0, 1);
                        const char* file = sFile.c_str();
                        DrawHelper::DrawWord(file, topLeftX + x * TILE_SIZE_PX + TILE_SIZE_PX * 0.47, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX, 10, _colorNotation);
                    }
                }
                else if(notation == Board::Notation::NUMERIC)
                {
                    if(tile.IsDark())
                    {
                        DrawHelper::DrawWord(board.TileToNotation(tile).c_str(), topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX / 4, 10, _colorNotation);
                    }
                }
            }
        }
    }
}

void View::DrawPieces()
{
    Board& board = _game->GetBoard();

    const DrawHelper::Color colorRedPiece = {255, 0, 0};
    const DrawHelper::Color colorRedPiece2 = {177, 0, 0};
    const DrawHelper::Color colorBluePiece = {0,109,211};
    const DrawHelper::Color colorBluePiece2 = {0,0,155};
    const float pieceSizeRatio = 0.33f;

    auto redPieces = board.GetPieces(Alliance::RED);
    auto bluePieces = board.GetPieces(Alliance::BLUE);

    for(const auto& [i, p] : redPieces)
        DrawPiece(*p, colorRedPiece, colorRedPiece2, pieceSizeRatio);

    for(const auto& [i, p] : bluePieces)
        DrawPiece(*p, colorBluePiece, colorBluePiece2, pieceSizeRatio);
}

void View::DrawMoveStep(const Step &step, DrawHelper::Color color)
{
    double xx1 = step.GetStart().GetCol();
    double yy1 = step.GetStart().GetRow();
    double xx2 = step.GetEnd().GetCol();
    double yy2 = step.GetEnd().GetRow();
    int x1, y1, x2, y2;
    if(_bRotateBoard)
    {
        FlippedCoordinates(xx1, yy1, x1, y1);
        FlippedCoordinates(xx2, yy2, x2, y2);
    }
    else
    {
        x1 = xx1;
        y1 = yy1;
        x2 = xx2;
        y2 = yy2;
    }

    double x1_px = x1 * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double y1_px = y1 * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double x2_px = x2 * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    double y2_px = y2 * TILE_SIZE_PX + TILE_SIZE_PX / 2;

    DrawHelper::DrawArrow(x1_px, y1_px, x2_px, y2_px, color);
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
                int x, y;
                if(_bRotateBoard)
                    FlippedCoordinates(p.second, p.first, x, y);
                else
                {
                    x = p.second;
                    y = p.first;
                }
                DrawHelper::DrawFilledRect(x * TILE_SIZE_PX, y * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX, colorSelection);
            }
        }
    }
}

void View::DrawPiece(const Piece &piece, DrawHelper::Color color, DrawHelper::Color color2, float pieceSizeRatio)
{
    const float xx = piece.GetCol();
    const float yy = piece.GetRow();
    int x, y;
    if(_bRotateBoard)
        FlippedCoordinates(xx, yy, x, y);
    else
    {
        x = xx;
        y = yy;
    }
    const float cx = x * TILE_SIZE_PX + TILE_SIZE_PX / 2;
    const float cy = y * TILE_SIZE_PX + TILE_SIZE_PX / 2;

    const int numStrips = 7;
    const float Rmax = pieceSizeRatio * TILE_SIZE_PX;
    const float Rmin = 0.05f * Rmax;
    for(int i = 0; i < numStrips; ++i)
    {
        float t = (i - 1.0f) / (numStrips - 1.0f);
        float R = Rmax * (1.0f - t) + Rmin * t;
        DrawHelper::DrawPolygon(cx, cy, R, 36, i % 2 == 0 ? color2 : color);
    }

    if(piece.IsKing())
        DrawCrown(piece, {255,255,0});
}

void View::DrawCrown(const Piece &piece, DrawHelper::Color color)
{
    const float xx = piece.GetCol();
    const float yy = piece.GetRow();
    int x, y;
    if(_bRotateBoard)
        FlippedCoordinates(xx, yy, x, y);
    else
    {
        x = xx;
        y = yy;
    }
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

    DrawHelper::DrawPolygon(leftX, topSides, 0.05 * TILE_SIZE_PX, 36, color);
    DrawHelper::DrawPolygon(cx, topY, 0.05 * TILE_SIZE_PX, 36, color);
    DrawHelper::DrawPolygon(rightX, topSides, 0.05 * TILE_SIZE_PX, 36, color);
}

