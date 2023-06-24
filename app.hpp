#pragma once

namespace App
{
    void Run(int argc, char *argv[]);
    //Handle user's input
    void MouseFunc(int button, int state, int x, int y);
    void KeyboardFunc(unsigned char key, int x, int y);
    void MouseMotionFunc(int x, int y);
    //Update game state
    void Timer(int);
    void Update();
    //Display funciton
    void DisplayFunc();
};
