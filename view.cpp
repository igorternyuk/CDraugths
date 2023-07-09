#include "view.h"
#include "player_human.h"
#include <GL/freeglut.h>
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
    const int W = board.GetBoardWidth();
    const int H = board.GetBoardHeight();
    fx = W - 1 - x;
    fy = H - 1 - y;
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
            if(_game->GetStatus() == GameStatus::PLAY)
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

                        //std::cout << "mx = " << mx << " my = " << my << std::endl;
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
    }
    else if(_viewMode == ViewMode::eMenuGameType)
    {
        if(state == GLUT_DOWN)
        {
            if(button == GLUT_LEFT_BUTTON)
            {
                _viewMode = ViewMode::eMenuGameMode;
            }
        }
    }
    else if(_viewMode == ViewMode::eMenuGameMode)
    {
        if(state == GLUT_DOWN)
        {
            if(button == GLUT_LEFT_BUTTON)
            {
                _viewMode = ViewMode::eMenuLevel;
            }
        }
    }
    else if(_viewMode == ViewMode::eMenuLevel)
    {
        if(state == GLUT_DOWN)
        {
            if(button == GLUT_LEFT_BUTTON)
            {
                _viewMode = ViewMode::eBoard;
                int depth = _game->MapLevelToSearchDepth(static_cast<Game::Level>(_selectedLevel));
                _game->SetupNewGame((Game::Type)_selectedGameType, (Game::Mode)_selectedGameMode, depth);

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
    if(_viewMode == ViewMode::eMenuGameType)
    {
        _selectedGameType = std::max(0, std::min((int)draughts::Game::NUM_OF_GAME_TYPES - 1, ((y - 120 + 18) / (18 + 18/2))));
    }
    else if(_viewMode == ViewMode::eMenuGameMode)
    {
        _selectedGameMode = std::max(0, std::min(1, ((y - 60) / 20)));
        if(_selectedGameMode == (int)draughts::Game::Mode::CPU_HUMAN)
            _bRotateBoard = true;
        else
            _bRotateBoard = false;

    }
    else if(_viewMode == ViewMode::eMenuLevel)
    {
        _selectedLevel = std::max(0, std::min(6, ((y - 60 + 18) / (18 + 18/2))));
    }
}

void View::DrawGameTypeMenu()
{
    drawMenuBackground();
    // glClearColor(0,0,0,0);
    int titleLeftX = (GetDefaultWindowWidth() - 20 * _sMenuTitleGameType.size()) / 2;
    DrawHelper::DrawWord24(_sMenuTitleGameType.c_str(), titleLeftX, 40, 20, _colorMenuTitle1);
    int titleLeftX2 = (GetDefaultWindowWidth() - 20 * _sMenuTitleGameType2.size()) / 2;
    DrawHelper::DrawWord24(_sMenuTitleGameType2.c_str(), titleLeftX2, 80, 20, _colorMenuTitle2);

    for(int i = 0; i < draughts::Game::NUM_OF_GAME_TYPES; ++i)
    {
        int leftX = (GetDefaultWindowWidth() - 20 * _menuItemsGameType[i].size()) / 2;
        DrawHelper::DrawWord24(_menuItemsGameType[i].c_str(), leftX, 120 + i * (18+18/2), 20, i == _selectedGameType ? _colorMenuItemSelected : _colorMenuItem);
    }
}

void View::DrawGameModeMenu()
{
    drawMenuBackground();
    //glClearColor(0,0,0,0);
    int titleLeftX = (GetDefaultWindowWidth() - 20 * _sMenuTitleGameMode.size()) / 2;
    DrawHelper::DrawWord24(_sMenuTitleGameMode.c_str(), titleLeftX, 20, 20, _colorMenuTitle1);

    for(int i = 0; i < 2; ++i)
    {
        int leftX = (GetDefaultWindowWidth() - 20 * _menuItemsGameMode[i].size()) / 2;
        DrawHelper::DrawWord24(_menuItemsGameMode[i].c_str(), leftX, 60 + i * 30, 20, i == _selectedGameMode ? _colorMenuItemSelected : _colorMenuItem);
    }
}

void View::DrawLevelMenu()
{
    drawMenuBackground();
    //glClearColor(0,0,0,0);
    int titleLeftX = (GetDefaultWindowWidth() - 20 * _sMenuTitleLevel.size()) / 2;
    DrawHelper::DrawWord24(_sMenuTitleLevel.c_str(), titleLeftX, 20, 20, _colorMenuTitle1);

    for(int i = 0; i < 7; ++i)
    {
        int leftX = (GetDefaultWindowWidth() - 20 * _menuItemsLevel[i].size()) / 2;
        DrawHelper::DrawWord24(_menuItemsLevel[i].c_str(), leftX, 60 + i * (18+18/2), 20, i == _selectedLevel ? _colorMenuItemSelected : _colorMenuItem);
    }
}

void View::drawMenuBackground()
{
    int boardSize = 6;
    float h = GetDefaultWindowHeight();
    const int tileSize = h / boardSize;
    DrawHelper::DrawFilledRect(0, 0, tileSize * boardSize, tileSize * boardSize, {130,130,130});

    for(int y = 0; y < boardSize; ++y)
    {
        for(int x = (y + 1) % 2; x < boardSize - 1; x += 2)
        {
            DrawHelper::DrawFilledRect(tileSize * x, tileSize * y, tileSize, tileSize, {90,112,120});
        }
    }

    DrawHelper::DrawPseudo3DPiece(1.5 * tileSize, 4.5 * tileSize + tileSize / 3, tileSize, 0.5 * tileSize, _colorLightPiece, _colorLightPiece2);
    //DrawHelper::DrawPseudo3DPiece(1.5 * tileSize, 3.5 * tileSize - tileSize / 3, tileSize, 0.5 * tileSize, _colorLightPiece, _colorLightPiece2);
    //DrawHelper::DrawPseudo3DPiece(3.5 * tileSize, 4 * tileSize + tileSize / 2, tileSize, 0.5 * tileSize, _colorDarkPiece, _colorDarkPiece2);
    DrawHelper::DrawPseudo3DPiece(3.5 * tileSize, 5.0 * tileSize - tileSize / 2, tileSize, 0.5 * tileSize, _colorDarkPiece, _colorDarkPiece2);
    //DrawHelper::DrawPseudo3DPiece(cx, cy, a_max, b_max, color, color2);
}

void View::OnKeyboardEvent(unsigned char key, int x, int y)
{
    bool bNeedToResizeWindow = false;
    bool bFlipChanged = false;
    int width = GetDefaultWindowWidth();
    int height = GetDefaultWindowHeight();
    if(key == KEY_RETURN)//enter
    {
        if(_viewMode == ViewMode::eBoard)
        {
            _game->Play();
        }
        else if(_viewMode == ViewMode::eMenuGameType)
        {
            _viewMode = ViewMode::eMenuGameMode;
        }
        else if(_viewMode == ViewMode::eMenuGameMode)
        {
            _viewMode = ViewMode::eMenuLevel;
            if(_selectedGameMode == (int)draughts::Game::Mode::CPU_HUMAN)
                _bRotateBoard = true;
        }
        else if(_viewMode == ViewMode::eMenuLevel)
        {
            _viewMode = ViewMode::eBoard;
            int depth = _game->MapLevelToSearchDepth(static_cast<Game::Level>(_selectedLevel));
            _game->SetupNewGame((Game::Type)_selectedGameType, (Game::Mode)_selectedGameMode, depth);

            width = GetWindowWidth();
            height = GetWindowHeight();

            glViewport(0, 0, width, height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,width,height,0,-1,1);
            glutReshapeWindow(width, height);
            glutPostRedisplay();
        }
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
    else if(key == ' ')
    {
        _bIsPseudo3DPieceStyle = !_bIsPseudo3DPieceStyle;
        bNeedToResizeWindow = false;
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

void View::ProcessSpecialKeys(unsigned char key, int x, int y)
{
    if(key == GLUT_KEY_UP)
    {
        if(_viewMode == ViewMode::eMenuGameType)
        {
            --_selectedGameType;
            if(_selectedGameType < 0)
                _selectedGameType =(int)draughts::Game::CANADIAN;
        }
        else if(_viewMode == ViewMode::eMenuGameMode)
        {
            --_selectedGameMode;
            if(_selectedGameMode < 0)
                _selectedGameMode = 1;
        }
        else if(_viewMode == ViewMode::eMenuLevel)
        {
            --_selectedLevel;
            if(_selectedLevel < 0)
                _selectedLevel = 6;
        }
    }
    else if(key == GLUT_KEY_DOWN)//enter
    {
        if(_viewMode == ViewMode::eMenuGameType)
        {
            ++_selectedGameType;
            _selectedGameType %= (int)draughts::Game::NUM_OF_GAME_TYPES;
        }
        else if(_viewMode == ViewMode::eMenuGameMode)
        {
            ++_selectedGameMode;
            _selectedGameMode %= 2;
        }
        else if(_viewMode == ViewMode::eMenuLevel)
        {
            ++_selectedLevel;
            _selectedLevel %= 4;
        }
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
    else if(_viewMode == ViewMode::eMenuLevel)
    {
        DrawLevelMenu();
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
    int W = board.GetBoardWidth();
    return W * TILE_SIZE_PX;
}

int View::GetWindowHeight()
{
    View* view = View::GetInstance();
    Game* game = view->GetGame();
    const Board& board = game->GetBoard();
    int H = board.GetBoardHeight();
    return H * TILE_SIZE_PX;
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
    bool bIsHumanPlayerRed = _game->GetPlayer(Alliance::DARK)->IsHuman();
    if(status == GameStatus::PLAY)
    {
        Alliance turn = _game->GetTurn();
        if(turn == Alliance::DARK)
            DrawHelper::DrawWord10(bIsHumanPlayerRed ? "YOUR TURN" : "OPPONENTS'S TURN", 5, 20, 10, {0,127,0});
        else if(turn == Alliance::LIGHT)
            DrawHelper::DrawWord10(bIsHumanPlayerRed ? "OPPONENTS'S TURN" : "YOUR TURN", 5, 20, 10, {0,127,0});
    }
    else if(status == GameStatus::RED_WON)
    {
        DrawHelper::DrawWord10(bIsHumanPlayerRed ? "YOU WON!" : "YOU LOST!", 5, 20, 10, {0,127,0});
    }
    else if(status ==GameStatus::BLUE_WON)
    {
        DrawHelper::DrawWord10(bIsHumanPlayerRed ? "YOU LOST!" : "YOU WON!", 5, 20, 10,{0,127,0});
    }
    else if(status == GameStatus::DRAW)
    {
        DrawHelper::DrawWord10("DRAW!", 5, 20, 10, {0,127,0});
    }
}

void View::DrawBoard()
{

    Game::Type gameType = GetGame()->GetType();
    bool bIsDiagonalDraughts = gameType == Game::Type::DIAGONAL64 || gameType == Game::Type::DIAGONAL100;
    int topLeftX = 0;
    int topLeftY = 0;

    Game* game = Game::GetInstance();
    Board& board = game->GetBoard();
    const int H = board.GetBoardHeight();
    const int W = board.GetBoardWidth();
    DrawHelper::DrawFilledRect(topLeftX, topLeftY, TILE_SIZE_PX * W, TILE_SIZE_PX * H, _colorTileLight);

    for(int yy = 0; yy < H; ++yy)
    {
        for(int xx = 0; xx < W; ++xx)
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
                bool bIsCoronationTile = board.IsCoronationTile(yy,xx,Alliance::LIGHT) || board.IsCoronationTile(yy,xx,Alliance::DARK);
                DrawHelper::DrawFilledRect(topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX,
                                           bIsDiagonalDraughts && bIsCoronationTile ? _colorTileCoronation : _colorTileDark);
            }
            //Draw notation

            if(_bShowNotation)
            {
                auto notation = board.GetNotation();
                if(notation == Notation::ALGEBRAIC)
                {
                    std::string sNotation = board.TileToNotation(tile);
                    if(x == 0)
                    {
                        std::string sRank = sNotation.substr(1, 1);
                        const char* rank = sRank.c_str();
                        DrawHelper::DrawWord24(rank, topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX * 3 / 5, 10, _colorNotation);
                    }
                    if(y == H - 1)
                    {
                        std::string sFile = sNotation.substr(0, 1);
                        const char* file = sFile.c_str();
                        DrawHelper::DrawWord24(file, topLeftX + x * TILE_SIZE_PX + TILE_SIZE_PX * 0.47, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX, 10, _colorNotation);
                    }
                }
                else if(notation == Notation::NUMERIC)
                {
                    if(tile.IsDark())
                    {
                        DrawHelper::DrawWord24(board.TileToNotation(tile).c_str(), topLeftX + x * TILE_SIZE_PX, topLeftY + y * TILE_SIZE_PX + TILE_SIZE_PX / 4, 10, _colorNotation);
                    }
                }
            }
        }
    }
}

void View::DrawPieces()
{
    Board& board = _game->GetBoard();

    const float pieceSizeRatio = 0.32f;

    auto redPieces = board.GetPieces(Alliance::DARK);
    auto bluePieces = board.GetPieces(Alliance::LIGHT);

    for(const auto& [i, p] : redPieces)
        if(_bIsPseudo3DPieceStyle)
            DrawPiece3(*p, _colorDarkPiece, _colorDarkPiece2, pieceSizeRatio);
        else
            DrawPiece(*p, _colorDarkPiece, _colorDarkPiece2, pieceSizeRatio);
    for(const auto& [i, p] : bluePieces)
        if(_bIsPseudo3DPieceStyle)
            DrawPiece3(*p, _colorLightPiece, _colorLightPiece2, pieceSizeRatio);
        else
            DrawPiece(*p, _colorLightPiece, _colorLightPiece2, pieceSizeRatio);
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
                DrawHelper::DrawFilledRect(x * TILE_SIZE_PX, y * TILE_SIZE_PX, TILE_SIZE_PX, TILE_SIZE_PX, _colorSelection);
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
    for(int i = 1; i < numStrips; ++i)
    {
        float t = (i - 1.0f) / (numStrips - 1.0f);
        float R = Rmax * (1.0f - t) + Rmin * t;
        DrawHelper::DrawPolygon(cx, cy, R, 36, i % 2 != 0 ? color2 : color);
    }

    if(piece.IsKing())
        DrawCrown(piece, {255,255,0});
}

void View::DrawPiece3(const draughts::Piece &piece, DrawHelper::Color color, DrawHelper::Color color2, float pieceSizeRatio)
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
    const float a_max = pieceSizeRatio * TILE_SIZE_PX;
    const float b_max = 0.5 * a_max;

    if(piece.IsKing())
    {
        DrawHelper::DrawPseudo3DPiece(cx, cy + TILE_SIZE_PX / 10, a_max, b_max, color, color2);
        DrawHelper::DrawPseudo3DPiece(cx, cy - TILE_SIZE_PX / 10, a_max, b_max, color, color2);
    }
    else
    {
        DrawHelper::DrawPseudo3DPiece(cx, cy, a_max, b_max, color, color2);
    }
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

