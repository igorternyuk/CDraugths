#pragma once

namespace view
{
    class DrawHelper
    {
    public:
        struct Color
        {
            int red;
            int green;
            int blue;
        };
    public:
        static void DrawArrow(float x1, float y1, float x2, float y2, Color color = {255,255,255});
        static void DrawWord(const char *word, float x_, float y_, float space, Color color = {255,255,255});
        static void DrawFilledRect(float x, float y, float w, float h, Color color = {255,255,255});
        static void DrawPolygon(float centerX, float centerY, float radius, int numSides = 5,
                         Color color = {255,255,255});
    };
}
