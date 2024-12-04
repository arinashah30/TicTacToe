/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This header defines global constants, variables, and enums used throughout the project.
*/

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

using namespace sf;
using namespace std;

// Graphics Constants
const int windowWidth = 600;
const int windowHeight = 600;
const int cellSize = windowWidth / 3;
const int statusBarHeight = 50;

// Network Constants
const unsigned short PORT = 54000;
const std::string SERVER_IP = "127.0.0.1";
extern TcpSocket socket;

// Graphics Extern Variables
extern RenderWindow window;
extern Font font;
extern Text statusBarText;

enum PLAYER {
    X,
    O,
    NONE
};

enum GAMESTATUS {
    X_WIN,
    O_WIN,
    DRAW,
    PLAYING
};

enum GAMEMODE {
    MULTIPLAYER,
    SINGLE_PLAYER,
    NO_MODE
};

enum DIFFICULTY {
    EASY,
    HARD,
    DEFAULT
};

#endif