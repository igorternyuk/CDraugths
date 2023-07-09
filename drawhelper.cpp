#include "drawhelper.h"
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
    glColor4f(color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alfa / 255.f);
    for(unsigned int i = 0; i < strlen(word); ++i)
    {
        glRasterPos2f(x_ + i * space, y_);
        char ch = word[i];
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    }
}

void DrawHelper::DrawWord10(const char *word, float x_, float y_, float space, Color color)
{
    glColor4f(color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alfa / 255.f);
    for(unsigned int i = 0; i < strlen(word); ++i)
    {
        glRasterPos2f(x_ + i * space, y_);
        char ch = word[i];
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)ch);
    }
}

void DrawHelper::DrawFilledRect(float x, float y, float w, float h, Color color)
{
    glColor4f(color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alfa / 255.f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void DrawHelper::DrawPolygon(float centerX, float centerY, float radius, int N, Color color)
{
    glColor4f(color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alfa / 255.f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < N; ++i)
    {
        float x = centerX + radius * cos(2 * M_PI * i / N);
        float y = centerY + radius * sin(2 * M_PI * i / N);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawHelper::DrawEllipse(float centerX, float centerY, float a, float b, Color color, int N)
{
    glColor4f(color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alfa / 255.f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < N; ++i)
    {
        float x = centerX + a * cos(2 * M_PI * i / N);
        float y = centerY + b * sin(2 * M_PI * i / N);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawHelper::DrawPseudo3DPiece(float centerX, float centerY, float a, float b, Color color, Color color2, int N)
{
    const float cx = centerX;
    const float cy = centerY;

    const int numStrips = 7;
    const float a_max = a;
    const float a_min = 0.05f * a_max;
    const float b_max = b;
    const float b_min = 0.5 * a_min;

    DrawHelper::DrawEllipse(cx, cy + 0.5*b_max, a_max, b_max, color2,  N);
    DrawHelper::DrawFilledRect(cx - a_max, cy - 0.5*b_max, 2.0 * a_max, b_max, color2);
    for(int i = 1; i <= numStrips; ++i)
    {
        float t = (i - 1.0f) / (numStrips - 1.0f);
        float a = a_max * (1.0f - t) + a_min * t;
        float b = b_max * (1.0f - t) + b_min * t;
        DrawHelper::DrawEllipse(cx, cy - 0.5 * b_max, a, b, i % 2 != 0 ? color2 : color, N);
    }
}
