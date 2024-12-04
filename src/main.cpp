/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This is the main entry point for the Tic Tac Toe game. It handles the game setup, user input,
game modes (Single Player or Multiplayer), and the main game loop. It integrates the logic
from other modules to coordinate game flow, including drawing the grid, detecting mouse clicks,
and managing turn-based gameplay.
*/


#include "globals.hpp"
#include "game.hpp"
#include "network.hpp"
#include "graphics.hpp"
#include <thread>
#include <chrono>

using namespace sf;
using namespace std;

int main()
{
    if (!font.loadFromFile("assets/Roboto-Regular.ttf"))
    {
        cerr << "Failed to load font!" << endl;
        return -1;
    }

    srand(time(nullptr));

    PLAYER player = NONE;
    PLAYER computer = NONE;
    GAMEMODE mode = NO_MODE;
    DIFFICULTY difficulty = DEFAULT;
    Game game;

    while (window.isOpen())
    {
        Event event;

        // Choose game mode
        if (mode == NO_MODE)
        {
            displayModeChoice();
            while (1)
            {
                if (Keyboard::isKeyPressed(Keyboard::Num1))
                {
                    mode = MULTIPLAYER;
                    break;
                }
                else if (Keyboard::isKeyPressed(Keyboard::Num2))
                {
                    mode = SINGLE_PLAYER;
                    break;
                }
            }

            // Choose difficulty of computer if playing single player game
            if (mode == SINGLE_PLAYER)
            {
                this_thread::sleep_for(chrono::milliseconds(250));
                displayDifficultyChoice();
                while (1)
                {
                    if (Keyboard::isKeyPressed(Keyboard::Num1))
                    {
                        difficulty = EASY;
                        break;
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::Num2))
                    {
                        difficulty = HARD;
                        break;
                    }
                }
            }
        }

        // User chooses if they want to be player X or O
        if (player == NONE)
        {
            displayStartScreen();

            while (1)
            {
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                    {
                        window.close();
                        return 0;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::X))
                {
                    player = X;
                    break;
                }
                else if (Keyboard::isKeyPressed(Keyboard::O))
                {
                    player = O;
                    break;
                }
            }

            if (mode == SINGLE_PLAYER)
            {
                if (player == X) computer = O;
                else if (player == O) computer = X;
                game = Game(mode, difficulty); // shared between user and computer
            }
            else if (mode == MULTIPLAYER)
            {
                if (player == X)
                {
                    setupServer();
                    game = Game(mode, difficulty); // if multiplayer game, player X (server) creates the game and sends it to O (client)
                    sendGame(game);
                }
                else if (player == O)
                {
                    setupClient();
                    game = receiveGame(); // get the game from the server
                }
            }
        }

        initStatusBar();

        // draw initial state
        drawGame(game);

        while (game.status == PLAYING)
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    return 0;
                }
            }

            if (game.activeTurn == player) // if it is this player's turn
            {
                while (true)
                {
                    if (window.waitEvent(event))
                    {
                        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) // wait until player clicks a cell
                        {
                            // get mouse position
                            int mouseX = event.mouseButton.x;
                            int mouseY = event.mouseButton.y;

                            if (mouseY > statusBarHeight) // make sure click is below the status bar
                            {
                                mouseY -= statusBarHeight;

                                // Determine the cell clicked
                                int row = mouseY / cellSize;
                                int col = mouseX / cellSize;

                                if (row >= 0 && row < 3 && col >= 0 && col < 3) // check click is within the grid bounds
                                {
                                    game.playerMove(row, col);
                                    if (game.mode == MULTIPLAYER) sendGame(game);
                                }
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                if (game.mode == MULTIPLAYER) // opponent move
                {
                    game = receiveGame(); // get most recent version of game from opponent
                }
                else if (game.mode == SINGLE_PLAYER) // computer move
                {
                    this_thread::sleep_for(chrono::milliseconds(1000)); //delay for computer move

                    if (game.difficulty == EASY) // easy mode selects random empty cell
                    {
                        bool foundEmptyCell = false;
                        int row, col;

                        while (!foundEmptyCell)
                        {
                            row = rand() % 3;
                            col = rand() % 3;
                            if (game.checkEmptyCell(row, col))
                            {
                                foundEmptyCell = true;
                            }
                        }
                        game.playerMove(row, col);
                    }
                    else if (game.difficulty == HARD) // hard mode uses minimax algorithm to find optimal move
                    {
                        pair<int, int> move;
                        minimax(game, move, computer);
                        game.playerMove(move.first, move.second);
                    }
                }
            }


            drawGame(game); //draw new game state


            if (game.mode == MULTIPLAYER) // sync game state
            {
                if (game.activeTurn == player)
                {
                    sendGame(game); // send game state after the turn
                }
                else
                {
                    game = receiveGame(); // receive game state before the next turn
                }
            }
        }

        this_thread::sleep_for(chrono::milliseconds(500));

        // Game Over
        while (1)
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    return 0;
                }
            }
            gameOverScreen(game.status, player);
            if (Keyboard::isKeyPressed(Keyboard::R)) // press R to restart game
            {
                break;
            }
        }

        game.resetGame();
    }

    return 0;
}