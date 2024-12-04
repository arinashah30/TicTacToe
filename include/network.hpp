/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This header declares the functions for networking in multiplayer mode.
*/

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "globals.hpp"
#include "game.hpp"

void setupServer();
void setupClient();
void sendMove(int row, int col);
pair<int, int> receiveMove();
void sendGame(Game game);
Game receiveGame();

#endif