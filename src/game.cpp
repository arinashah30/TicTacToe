/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This file implements the core game logic for Tic Tac Toe. It includes functions to manage
the game state, update the grid, check for game-over conditions, serialize/deserialize the
game state for multiplayer mode, and implement AI moves for single-player mode.
*/

#include "game.hpp"
#include <map>

/**
 * @brief Draws the game board and current state of the grid.
 */
void Game::drawBoard()
{

    window.clear(Color::White);

    // Draw the Tic-Tac-Toe grid
    RectangleShape line;

    // Vertical lines
    for (int i = 1; i < 3; ++i)
    {
        line.setSize(Vector2f(5, windowHeight));
        line.setPosition(i * cellSize, statusBarHeight);
        line.setFillColor(Color::Black);
        window.draw(line);
    }

    // Horizontal lines
    for (int i = 1; i < 3; ++i)
    {
        line.setSize(Vector2f(windowWidth, 5));
        line.setPosition(0, i * cellSize + statusBarHeight);
        line.setFillColor(Color::Black);
        window.draw(line);
    }

    Text text;

    text.setFont(font);
    text.setCharacterSize(100);

    // Draw X's and O's in correct positions
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (grid[row][col] == 1)
            { // Draw X
                text.setString("X");
                text.setFillColor(Color::Red);
                FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                text.setPosition(col * cellSize + cellSize / 2.0f, row * cellSize + cellSize / 2.0f + statusBarHeight);
                window.draw(text);
            }
            else if (grid[row][col] == 2)
            { // Draw O
                text.setString("O");
                text.setFillColor(Color::Blue);
                FloatRect textBounds = text.getLocalBounds();
                text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                text.setPosition(col * cellSize + cellSize / 2.0f, row * cellSize + cellSize / 2.0f + statusBarHeight);
                window.draw(text);
            }
        }
    }
}

/**
 * @brief Checks if a specific cell on the grid is empty.
 * 
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 * @return True if the cell is empty (value 0), otherwise false.
 */
bool Game::checkEmptyCell(int row, int col) {
    return grid[row][col] == 0;
}

/**
 * @brief Updates the game status by checking for a win, draw, or ongoing game.
 * 
 * This function checks each row, column, and diagonal for a win condition.
 * If no winner is found and all cells are filled, the game is a draw. Otherwise,
 * the game continues.
 */
void Game::updateGameStatus()
{
    // Check rows and columns for a win
    for (int i = 0; i < 3; ++i)
    {
        // Check row
        if (grid[i][0] != 0 && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
        {
            status = grid[i][0] == 1 ? X_WIN : O_WIN;
            return;
        }
        // Check column
        if (grid[0][i] != 0 && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
        {
            status = grid[0][i] == 1 ? X_WIN : O_WIN;
            return;
        }
    }

    // Check diagonals for a win
    if (grid[0][0] != 0 && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
    {
        status = grid[0][0] == 1 ? X_WIN : O_WIN;
        return;
    }
    if (grid[0][2] != 0 && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
    {
        status = grid[0][2] == 1 ? X_WIN : O_WIN;
        return;
    }

    // Check for a draw (if all cells are filled and no one has won)
    bool isDraw = true;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (grid[row][col] == 0)
            {
                isDraw = false; // Empty cell found, so not a draw
                break;
            }
        }
        if (!isDraw)
            break;
    }

    if (isDraw)
    {
        status = DRAW;
        return;
    }

    // Otherwise, the game is still ongoing
    status = PLAYING;
    return;
}

/**
 * @brief Processes a move made by the current player.
 * 
 * @param row The row index of the move.
 * @param col The column index of the move.
 * 
 * This function updates the grid with the player's symbol (X or O) if the cell is empty.
 * It then switches the turn to the other player and updates the game status.
 */
void Game::playerMove(int row, int col)
{
    if (grid[row][col] == 0)
    {
        grid[row][col] = (activeTurn == X) ? 1 : 2; // 1 for X, 2 for O
        activeTurn = (activeTurn == X) ? O : X; // End turn
    }
    updateGameStatus();
}

/**
 * @brief Resets the game to its initial state.
 * 
 * This function clears the grid, sets the game status to `PLAYING`, and prepares
 * the game for a new round.
 */
void Game::resetGame()
{
    window.clear(Color::White);
    for (auto &row : grid)
    {
        for (auto &cell : row)
        {
            cell = 0; // Set each cell to empty
        }
    }
    status = PLAYING; // Reset status to playing
}

/**
 * @brief Calculates the score for the current game state based on the specified player.
 * 
 * @param player The player (X or O) for whom the score is calculated.
 * @return An integer score: 10 for a win, -10 for a loss, and 0 for a draw.
 */
int Game::score(PLAYER player) {
    if (status == X_WIN && player == X) {
        return 10;
    } else if (status == O_WIN && player == O) {
        return 10;
    } else if (status == DRAW) {
        return 0;
    } else if (status == O_WIN && player == X) {
        return -10;
    } else if (status == X_WIN && player == O) {
        return -10;
    }
    return 0;
}

/**
 * @brief Implements the minimax algorithm to determine the optimal move for the computer.
 * 
 * @param game The current game state.
 * @param move A reference to a pair that will store the optimal move (row, col).
 * @param computer The computer's player type (X or O).
 * @return The score of the optimal move.
 * 
 * This function recursively evaluates all possible game states to find the best move
 * for the computer. It uses the `score` function to evaluate terminal states.
 */
int minimax(Game game, pair<int, int>& move, PLAYER computer)
{
    if (game.status != PLAYING) {
        return game.score(computer);
    }
    vector<int> scores;
    vector<pair<int, int>> moves;
    vector<pair<int, int>> availableMoves = game.availablePositions();

    for (pair<int, int> currentMove : availableMoves)
    {
        Game possibleGame = game.getNewState(currentMove);
        scores.push_back(minimax(possibleGame, move, computer));
        moves.push_back(currentMove);
    }

    if (game.activeTurn == computer) {
        auto it = max_element(scores.begin(), scores.end());
        int maxScoreIndex = distance(scores.begin(), it);
        move = moves[maxScoreIndex];
        return scores[maxScoreIndex];
    } else {
        auto it = min_element(scores.begin(), scores.end());
        int minScoreIndex = distance(scores.begin(), it);
        move = moves[minScoreIndex];
        return scores[minScoreIndex];
    }

}

/**
 * @brief Creates a new game state by applying a move to the current game.
 * 
 * @param move A pair representing the row and column of the move.
 * @return A new `Game` object representing the updated state.
 */
Game Game::getNewState(pair<int, int> move) {
    return Game(*this, move);
}

/**
 * @brief Serializes the game state into a string for network transmission.
 * 
 * The serialized string includes the grid, active turn, game status, mode, and difficulty.
 * @return A string representation of the game state.
 */
string Game::serialize() const {
    ostringstream oss;

    // Serialize the grid
    for (const auto& row : grid) {
        for (int cell : row) {
            oss << cell << " "; // Space-separated grid values
        }
    }

    // Serialize activeTurn, status, mode, and difficulty
    oss << activeTurn << " " << status << " " << mode << " " << difficulty;

    return oss.str(); // Return the serialized string
}

/**
 * @brief Deserializes a string to reconstruct the game state.
 * 
 * @param data The string representation of the game state.
 * This function extracts the grid, active turn, game status, mode, and difficulty from the string.
 */
void Game::deserialize(const string& data) {
    istringstream iss(data);

    // Deserialize the grid
    for (auto& row : grid) {
        for (int& cell : row) {
            iss >> cell; // Read grid values
        }
    }

    // Deserialize activeTurn, status, mode, and difficulty
    int turn, gameStatus, gameMode, gameDifficulty;
    iss >> turn >> gameStatus >> gameMode >> gameDifficulty;
    activeTurn = static_cast<PLAYER>(turn);
    status = static_cast<GAMESTATUS>(gameStatus);
    mode = static_cast<GAMEMODE>(gameMode);
    difficulty = static_cast<DIFFICULTY>(gameDifficulty);
}

/**
 * @brief Returns a list of all empty positions on the grid.
 * 
 * @return A vector of pairs, each representing the row and column of an empty cell.
 */
vector<pair<int, int>> Game::availablePositions()
{
    vector<pair<int, int>> positions;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 0)
                positions.push_back({i, j});
        }
    }
    return positions;
}