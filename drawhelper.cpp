#include "drawhelper.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstring>

using namespace view;

void DrawHelper::DrawArrow(float x1, float y1, float x2, float y2, Color color)
{
    const double arrowSideLength = 20;
    double alpha = atan2(y2 - y1, x2 - x1);
    double leftX = x2 - arrowSideLength * cos(alpha - M_PI / 12);
    double leftY = y2 - arrowSideLength * sin(alpha - M_PI / 12);
    double rightX = x2 - arrowSideLength * cos(alpha + M_PI / 12);;
    double rightY = y2 - arrowSideLength * sin(alpha + M_PI / 12);;
    glColor3f(color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(leftX, leftY);
    glVertex2f(x2, y2);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(rightX, rightY);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1);
}

void DrawHelper::DrawWord24(const char *word, float x_, float y_, float space, Color color)
{
    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    for(unsigned int i = 0; i < strlen(word); ++i)
    {
        glRasterPos2f(x_ + i * space, y_);
        char ch = word[i];
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    }
}

void DrawHelper::DrawWord10(const char *word, float x_, float y_, float space, Color color)
{
    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    for(unsigned int i = 0; i < strlen(word); ++i)
    {
        glRasterPos2f(x_ + i * space, y_);
        char ch = word[i];
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)ch);
    }
}

void DrawHelper::DrawFilledRect(float x, float y, float w, float h, Color color)
{
    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void DrawHelper::DrawPolygon(float centerX, float centerY, float radius, int numSides, Color color)
{
    glColor3f(color.red / 255.f, color.green / 255.f, color.blue / 255.f);
    glBegin(GL_POLYGON);
    for(int a = 0; a < numSides; ++a)
    {
        float x = centerX + radius * cos(2 * M_PI * a / numSides);
        float y = centerY + radius * sin(2 * M_PI * a / numSides);
        glVertex2f(x, y);
    }
    glEnd();
}
