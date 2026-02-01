#ifndef AI_H
#define AI_H

#include "Board.h"
#include <utility>
#include <vector>

enum AILevel {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
};

class AI {
public:
    AI(AILevel level = MEDIUM);
    
    // Get the AI's move for the given board state
    std::pair<int, int> getMove(const Board& board, CellState aiPlayer);
    
    // Set AI difficulty level
    void setLevel(AILevel level);
    AILevel getLevel() const;
    
private:
    AILevel difficulty;
    
    // Different AI strategies
    std::pair<int, int> getRandomMove(const Board& board);
    std::pair<int, int> getMediumMove(const Board& board, CellState aiPlayer);
    std::pair<int, int> getHardMove(const Board& board, CellState aiPlayer);
    
    // Minimax algorithm for hard difficulty
    int minimax(Board board, int depth, bool isMaximizing, CellState aiPlayer, CellState humanPlayer, int maxDepth);
    
    // Helper functions
    CellState getOpponent(CellState player);
    int evaluateBoard(const Board& board, CellState aiPlayer, CellState humanPlayer);
    std::pair<int, int> findWinningMove(const Board& board, CellState player);
    std::pair<int, int> findBlockingMove(const Board& board, CellState player);
};

#endif