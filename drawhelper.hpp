#pragma once
#include <initializer_list>

namespace view
{
    class DrawHelper
    {
    public:
        class Color
        {
        public:
            Color(std::initializer_list<int> list)
            {
               int k = 0;
               for(auto it = list.begin(); it != list.end(); ++it)
               {
                   ++k;
                   if(k == 1)
                       red = *it;
                   else if(k == 2)
                       green = *it;
                   else if(k == 3)
                       blue = *it;
                   else if(k == 4)
                       alfa = *it;
               }
            }
        public:
            int red = 255;
            int green = 255;
            int blue = 255;
            int alfa = 255;
        };
    public:
        static void DrawArrow(float x1, float y1, float x2, float y2, Color color = {255,255,255});
        static void DrawWord24(const char *word, float x_, float y_, float space, Color color = {255,255,255});
        static void DrawWord10(const char *word, float x_, float y_, float space, Color color = {255,255,255});
        static void DrawFilledRect(float x, float y, float w, float h, Color color = {255,255,255});
        static void DrawPolygon(float centerX, float centerY, float radius, int numSides = 5,
                         Color color = {255,255,255});
        static void DrawEllipse(float centerX, float centerY, float a, float b, Color color = {255,255,255}, int N = 36);
        static void DrawPseudo3DPiece(float centerX, float centerY, float a, float b, Color color = {255,255,255}, Color color2 = {255,255,255}, int N = 36);

    };
}
