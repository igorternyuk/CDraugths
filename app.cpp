#include "app.hpp"
#include "view.hpp"
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <iostream>

using namespace view;

#define WINDOW_TITLE "CDraughts"
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

/*
    POLISH 10x10
    BRAZILIAN
    DRAUGHTS64
    CHECKERS

    AI - AI
    Human - AI
    AI - HUMAN
    Human - Human

void glutMotionFunc(void (*func)(int x, int y));
void glutPassiveMotionFunc(void (*func)(int x, int y));
*/
void App::Run(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    int WINDOW_WIDTH = View::GetWindowWidth();
    int WINDOW_HEIGHT = View::GetWindowHeight();
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
    glutInitWindowPosition((screenWidth - WINDOW_WIDTH) / 2, (screenHeight - WINDOW_HEIGHT) / 2);
    glutCreateWindow(WINDOW_TITLE);
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH,WINDOW_HEIGHT,0,-1,1);
    glutDisplayFunc(&App::DisplayFunc);
    glutMouseFunc(&App::MouseFunc);
    glutTimerFunc(500, &App::Timer, 0);
    glutKeyboardFunc(&App::KeyboardFunc);
    glutMainLoop();
}

void App::Update()
{
    View* view = View::GetInstance();
    view->Update();
}

void App::DisplayFunc()
{
    View* view = View::GetInstance();
    glClear(GL_COLOR_BUFFER_BIT);
    view->Render();
    glutSwapBuffers();
}

void App::Timer(int)
{
    Update();
    DisplayFunc();
    glutTimerFunc(500, Timer, 0);
}

void App::MouseFunc(int button, int state, int x, int y)
{
    View* view = View::GetInstance();
    view->OnMouseEvent(button, state, x, y);
}

void App::KeyboardFunc(unsigned char key, int x, int y)
{
    View* view = View::GetInstance();
    view->OnKeyboardEvent(key, x, y);
}
