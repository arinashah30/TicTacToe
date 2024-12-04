/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This header declares the functions used for rendering graphical elements of the game.
*/

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "globals.hpp"
#include "game.hpp"

void displayStartScreen();
void displayModeChoice();
void displayDifficultyChoice();
void gameOverScreen(GAMESTATUS gameOver, PLAYER player);
void initStatusBar();
void updateStatusBar(GAMEMODE mode, PLAYER activeTurn, DIFFICULTY difficulty);
void drawGame(Game game);

#endif