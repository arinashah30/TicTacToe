/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This file handles the main graphical elements of the game using SFML. It includes functions for
drawing the game grid, rendering X and O symbols, displaying the status bar, and showing the
game-over screen. It ensures proper alignment of all UI elements, including adjustments for
the status bar height.
*/

#include "graphics.hpp"

RenderWindow window(VideoMode(windowWidth, windowHeight + statusBarHeight), "Tic-Tac-Toe");
Font font;
Text statusBarText;

/**
 * @brief Displays the start screen where the user chooses to play as X or O.
 * 
 * This function renders a message prompting the user to press the X or O key to
 * choose their player character.
 */
void displayStartScreen()
{
    Text startText;
    startText.setFont(font);
    startText.setString("Press the X key or O key to choose your player");
    startText.setCharacterSize(24);
    startText.setFillColor(Color::Black);

    FloatRect textBounds = startText.getLocalBounds();
    startText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    startText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

    window.clear(Color::White);
    window.draw(startText);
    window.display();
}

/**
 * @brief Displays the mode choice screen where the user selects Multiplayer or Single Player.
 * 
 * This function renders a message prompting the user to press 1 for Multiplayer or 2 for Single Player.
 */
void displayModeChoice()
{
    Text txt;
    txt.setFont(font);
    txt.setString("Press 1 for Multiplayer, 2 for Single Player");
    txt.setCharacterSize(24);
    txt.setFillColor(Color::Black);

    FloatRect textBounds = txt.getLocalBounds();
    txt.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    txt.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

    window.clear(Color::White);
    window.draw(txt);
    window.display();
}

/**
 * @brief Displays the difficulty choice screen for single-player mode.
 * 
 * This function renders a message prompting the user to press 1 for Easy difficulty or 2 for Hard difficulty.
 */
void displayDifficultyChoice()
{
    Text modeText;
    modeText.setFont(font);
    modeText.setString("Difficulty: Press 1 for Easy, 2 for Hard");
    modeText.setCharacterSize(24);
    modeText.setFillColor(Color::Black);

    FloatRect textBounds = modeText.getLocalBounds();
    modeText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    modeText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

    window.clear(Color::White);
    window.draw(modeText);
    window.display();
}

/**
 * @brief Displays the game-over screen with the outcome and a prompt to restart the game.
 * 
 * @param gameOver The final game status (X_WIN, O_WIN, or DRAW).
 * @param player The player type (X or O) to customize the message.
 * 
 * This function displays the outcome of the game. It also prompts the user to press R to restart the game.
 */
void gameOverScreen(GAMESTATUS gameOver, PLAYER player)
{
    RectangleShape overlay(Vector2f(windowWidth, windowHeight + statusBarHeight));
    overlay.setFillColor(Color(0, 0, 0, 50));

    Text message;
    message.setFont(font);
    message.setCharacterSize(48);
    message.setFillColor(Color::White);

    if (gameOver == X_WIN)
    {
        message.setString(player == X ? "You Win (X)!" : "Opponent Wins (X)");
    }
    else if (gameOver == O_WIN)
    {
        message.setString(player == O ? "You Win (O)!" : "Opponent Wins (O)");
    }
    else if (gameOver == DRAW)
    {
        message.setString("It's a Draw!");
    }

    FloatRect textBounds = message.getLocalBounds();
    message.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    message.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

    Text restartMessage;
    restartMessage.setFont(font);
    restartMessage.setCharacterSize(24);
    restartMessage.setFillColor(Color::White);
    restartMessage.setString("Press R to Restart");

    FloatRect restartBounds = restartMessage.getLocalBounds();
    restartMessage.setOrigin(restartBounds.width / 2.0f, restartBounds.height / 2.0f);
    restartMessage.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50);

    window.draw(overlay);
    window.draw(message);
    window.draw(restartMessage);
    window.display();
}

/**
 * @brief Initializes the status bar at the top of the window.
 * 
 * The status bar is set up with a default font, size, and position. It is used
 * to display the game mode, active turn, and difficulty level.
 */
void initStatusBar() {
    statusBarText.setFont(font);
    statusBarText.setCharacterSize(24);
    statusBarText.setFillColor(sf::Color::Black);
    statusBarText.setPosition(10, 10);
}

/**
 * @brief Updates the status bar text with the current game mode, active turn, and difficulty level.
 * 
 * @param mode The current game mode (SINGLE_PLAYER or MULTIPLAYER).
 * @param activeTurn The current active player's turn (X or O).
 * @param difficulty The difficulty level (EASY or HARD) for single-player mode.
 * 
 * This function updates the text displayed on the status bar with the latest game state.
 */
void updateStatusBar(GAMEMODE mode, PLAYER activeTurn, DIFFICULTY difficulty) {
    string modeText = (mode == SINGLE_PLAYER) ? "Single Player" : "Multiplayer";
    string turnText = (activeTurn == X) ? "Turn: X" : "Turn: O";

    string difficultyText = "";
    if (mode == SINGLE_PLAYER) {
        difficultyText = (difficulty == EASY) ? " | Difficulty: Easy" : " | Difficulty: Hard";
    }

    statusBarText.setString(modeText + " | " + turnText + difficultyText);
}

/**
 * @brief Draws the current state of the game, including the board and status bar.
 * 
 * @param game The current `Game` object containing the game state.
 * 
 * This function updates the status bar, renders the game grid, and displays the entire window.
 */
void drawGame(Game game) {
    updateStatusBar(game.mode, game.activeTurn, game.difficulty);
    game.drawBoard();
    window.draw(statusBarText);
    window.display();
}