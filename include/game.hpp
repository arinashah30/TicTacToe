/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This header defines the `Game` class, which encapsulates the state and logic of the Tic Tac Toe game.
It includes member variables for the grid, player turn, game status, mode, and difficulty, as well
as functions for updating the game state, handling player moves, and managing AI functionality.
*/

#ifndef GAME_HPP
#define GAME_HPP

#include "globals.hpp"
#include <sstream>

using namespace std;

class Game
{
public:
    vector<vector<int>> grid = vector<vector<int>>(3, vector<int>(3, 0)); // game grid represented as a 3x3 matrix
    PLAYER activeTurn; // player whose turn is currently active
    GAMESTATUS status; // current status of the game
    GAMEMODE mode; // selected game mode
    DIFFICULTY difficulty; // difficulty if in single player mode

    /**
     * @brief Default constructor to initialize the game state with a mode and difficulty level.
     * @param mode The game mode (default is NO_MODE).
     * @param difficulty The difficulty level (default is DEFAULT).
     */
    Game(GAMEMODE mode = NO_MODE, DIFFICULTY difficulty = DEFAULT)
    {
        activeTurn = X;
        status = PLAYING;
        this->mode = mode;
        this->difficulty = difficulty;
    }

    /**
     * @brief Constructs a new game state based on an existing game and a move.
     * @param oldGame The existing game state.
     * @param move The move to apply to the new game state.
     */
    Game(const Game &oldGame, pair<int, int> move)
    {
        this->activeTurn = oldGame.activeTurn;
        this->status = oldGame.status;
        this->mode = oldGame.mode;
        this->difficulty = oldGame.difficulty;
        this->grid = oldGame.grid;

        this->playerMove(move.first, move.second); // make the move
    }

    void drawBoard();
    vector<pair<int, int>> availablePositions();
    void resetGame();
    void updateGameStatus();
    void playerMove(int row, int col);
    Game getNewState(pair<int, int> move);
    int score(PLAYER player);
    string serialize() const;
    void deserialize(const string &data);
    bool checkEmptyCell(int row, int col);
};

int minimax(Game game, pair<int, int> &move, PLAYER computer);

#endif