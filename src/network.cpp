/*
Author: Arina Shah
Class: ECE4122 (A)
Last Date Modified: 12/3/2024
Description:
This file manages the networking functionality for multiplayer mode. It includes functions
to send and receive game states over a socket and set up the server and client connections.
*/

#include "network.hpp"

using namespace std;
using namespace sf;

TcpSocket socket;

/**
 * @brief Sets up the server for a multiplayer game by creating a listener, accepting a client connection, 
 *        and performing a handshake to ensure the connection is ready.
 * 
 * @throws Exits the program with an error message if any part of the setup process fails.
 */
void setupServer()
{
    TcpListener listener;
    if (listener.listen(54000) != Socket::Done)
    {
        cerr << "Failed to bind listener socket to port 54000" << endl;
        exit(1);
    }

    cout << "Waiting for a client to connect..." << endl;
    if (listener.accept(socket) != Socket::Done)
    {
        cerr << "Failed to accept client connection!" << endl;
        exit(1);
    }

    cout << "Client connected! Sending handshake..." << endl;

    // Handshake: Send "ready" to the client
    string readyMessage = "ready";
    if (socket.send(readyMessage.c_str(), readyMessage.size() + 1) != Socket::Done)
    {
        cerr << "Failed to send handshake message to client!" << endl;
        exit(1);
    }

    // Wait for acknowledgment from client
    char buffer[128];
    size_t received;
    if (socket.receive(buffer, sizeof(buffer), received) != Socket::Done)
    {
        cerr << "Failed to receive acknowledgment from client!" << endl;
        exit(1);
    }

    if (string(buffer) != "ack")
    {
        cerr << "Invalid acknowledgment from client!" << endl;
        exit(1);
    }

    cout << "Handshake complete. Ready to start the game!" << endl;
}

/**
 * @brief Sets up the client for a multiplayer game by connecting to the server and performing a handshake
 *        to ensure the connection is ready.
 * 
 * @throws Exits the program with an error message if the connection or handshake fails.
 */
void setupClient()
{
    if (socket.connect("127.0.0.1", 54000) != Socket::Done)
    {
        cerr << "Failed to connect to server!" << endl;
        exit(1);
    }

    cout << "Connected to server! Waiting for handshake..." << endl;

    // Handshake: Wait for "ready" message from the server
    char buffer[128];
    size_t received;
    if (socket.receive(buffer, sizeof(buffer), received) != Socket::Done)
    {
        cerr << "Failed to receive handshake message from server!" << endl;
        exit(1);
    }

    if (string(buffer) != "ready")
    {
        cerr << "Invalid handshake message from server!" << endl;
        exit(1);
    }

    // Send acknowledgment to server
    string ackMessage = "ack";
    if (socket.send(ackMessage.c_str(), ackMessage.size() + 1) != Socket::Done)
    {
        cerr << "Failed to send acknowledgment to server!" << endl;
        exit(1);
    }

    cout << "Handshake complete. Ready to start the game!" << endl;
}

/**
 * @brief Sends the player's move (row and column) to the opponent over the socket.
 * 
 * @param row The row index of the player's move (0-based).
 * @param col The column index of the player's move (0-based).
 * 
 * @throws Outputs an error message if the move fails to send.
 */
void sendMove(int row, int col)
{
    string move = to_string(row) + " " + to_string(col);
    if (socket.send(move.c_str(), move.size() + 1) != Socket::Done)
    {
        cerr << "Error sending move!" << endl;
    }
}

/**
 * @brief Receives the opponent's move (row and column) from the socket.
 * 
 * @return A pair of integers representing the row and column of the opponent's move.
 * 
 * @throws Outputs an error message if the move fails to receive.
 */
pair<int, int> receiveMove()
{
    char buffer[128];
    size_t received;
    if (socket.receive(buffer, sizeof(buffer), received) != Socket::Done)
    {
        cerr << "Error receiving move!" << endl;
    }
    int row, col;
    sscanf(buffer, "%d %d", &row, &col);
    return {row, col};
}

/**
 * @brief Sends the serialized game state to the opponent over the socket and waits for acknowledgment
 *        to ensure successful delivery.
 * 
 * @param game The current game state to be serialized and sent.
 * 
 * @throws Outputs an error message if the game state fails to send or acknowledgment fails to receive.
 */
void sendGame(Game game)
{
    string gameData = game.serialize();
    bool ackReceived = false;

    while (!ackReceived)
    {
        // Send the serialized game state
        if (socket.send(gameData.c_str(), gameData.size() + 1) != Socket::Done)
        {
            cerr << "Error sending game!" << endl;
            continue;
        }

        // Wait for acknowledgment
        char buffer[128];
        size_t received;
        if (socket.receive(buffer, sizeof(buffer), received) == Socket::Done)
        {
            if (string(buffer) == "ACK")
            {
                ackReceived = true; // Acknowledgment received
            }
        }
    }
}

/**
 * @brief Receives the serialized game state from the opponent over the socket, deserializes it into a
 *        `Game` object, and sends an acknowledgment back to confirm successful receipt.
 * 
 * @return The deserialized `Game` object representing the received game state.
 * 
 * @throws Outputs an error message if the game state fails to receive or acknowledgment fails to send.
 */
Game receiveGame()
{
    char buffer[1024];
    size_t received;
    Game receivedGame;
    bool gameReceived = false;

    while (!gameReceived)
    {
        // Receive the serialized game state
        if (socket.receive(buffer, sizeof(buffer), received) == Socket::Done)
        {
            receivedGame.deserialize(string(buffer)); // Deserialize game state
            gameReceived = true;

            // Send acknowledgment
            if (socket.send("ACK", 4) != Socket::Done)
            {
                cerr << "Error sending acknowledgment!" << endl;
                gameReceived = false; // Resend if acknowledgment fails
            }
        }
    }

    return receivedGame;
}
