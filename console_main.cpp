#include <iostream>
#include <limits>
#include "Board.h"
#include "AI.h"

class ConsoleGame {
private:
    Board board;
    AI ai;
    CellState humanPlayer;
    CellState aiPlayer;
    CellState currentPlayer;

public:
    ConsoleGame() : humanPlayer(PLAYER_X), aiPlayer(PLAYER_O), currentPlayer(PLAYER_X) {
        ai.setLevel(MEDIUM);
    }

    void displayBoard() {
        std::cout << "\n   0   1   2\n";
        for (int row = 0; row < 3; row++) {
            std::cout << row << " ";
            for (int col = 0; col < 3; col++) {
                CellState cell = board.getCell(row, col);
                char symbol = ' ';
                if (cell == PLAYER_X) symbol = 'X';
                else if (cell == PLAYER_O) symbol = 'O';
                
                std::cout << " " << symbol << " ";
                if (col < 2) std::cout << "|";
            }
            std::cout << "\n";
            if (row < 2) std::cout << "  ---|---|---\n";
        }
        std::cout << "\n";
    }

    bool getHumanMove() {
        int row, col;
        std::cout << "Enter your move (row col): ";
        
        if (!(std::cin >> row >> col)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter two numbers.\n";
            return false;
        }

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            std::cout << "Invalid position! Use 0-2 for both row and column.\n";
            return false;
        }

        if (board.getCell(row, col) != EMPTY) {
            std::cout << "That position is already taken!\n";
            return false;
        }

        return board.makeMove(row, col, humanPlayer);
    }

    void makeAIMove() {
        std::cout << "AI is thinking...\n";
        auto move = ai.getMove(board, aiPlayer);
        if (move.first != -1 && move.second != -1) {
            board.makeMove(move.first, move.second, aiPlayer);
            std::cout << "AI played at position (" << move.first << ", " << move.second << ")\n";
        }
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    bool checkGameEnd() {
        WinInfo winInfo = board.checkWin();
        
        if (winInfo.hasWinner) {
            displayBoard();
            if (winInfo.winner == humanPlayer) {
                std::cout << "Congratulations! You won!\n";
            } else {
                std::cout << "AI wins! Better luck next time.\n";
            }
            return true;
        }
        
        if (board.isFull()) {
            displayBoard();
            std::cout << "It's a draw!\n";
            return true;
        }
        
        return false;
    }

    void run() {
        std::cout << "=== Console Tic Tac Toe ===\n";
        std::cout << "You are X, AI is O\n";
        std::cout << "Enter moves as 'row col' (0-2 for both)\n\n";

        while (true) {
            displayBoard();
            
            if (currentPlayer == humanPlayer) {
                if (getHumanMove()) {
                    if (checkGameEnd()) break;
                    switchPlayer();
                }
            } else {
                makeAIMove();
                if (checkGameEnd()) break;
                switchPlayer();
            }
        }

        std::cout << "\nThanks for playing!\n";
    }
};

int main() {
    ConsoleGame game;
    game.run();
    return 0;
}