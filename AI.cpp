#include "AI.h"
#include <random>
#include <algorithm>
#include <climits>
#include <vector>

AI::AI(AILevel level) : difficulty(level) {}

std::pair<int, int> AI::getMove(const Board& board, CellState aiPlayer) {
    switch (difficulty) {
        case EASY:
            return getRandomMove(board);
        case MEDIUM:
            return getMediumMove(board, aiPlayer);
        case HARD:
            return getHardMove(board, aiPlayer);
        default:
            return getRandomMove(board);
    }
}

void AI::setLevel(AILevel level) {
    difficulty = level;
}

AILevel AI::getLevel() const {
    return difficulty;
}

std::pair<int, int> AI::getRandomMove(const Board& board) {
    auto emptyCells = board.getEmptyCells();
    if (emptyCells.empty()) {
        return {-1, -1}; // No valid moves
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, emptyCells.size() - 1);
    
    return emptyCells[dis(gen)];
}

std::pair<int, int> AI::getMediumMove(const Board& board, CellState aiPlayer) {
    // 1. Try to win
    auto winMove = findWinningMove(board, aiPlayer);
    if (winMove.first != -1) {
        return winMove;
    }
    
    // 2. Block opponent from winning
    CellState opponent = getOpponent(aiPlayer);
    auto blockMove = findBlockingMove(board, opponent);
    if (blockMove.first != -1) {
        return blockMove;
    }
    
    // 3. Take center if available
    if (board.getCell(1, 1) == EMPTY) {
        return {1, 1};
    }
    
    // 4. Take corners
    std::vector<std::pair<int, int>> corners = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    for (const auto& corner : corners) {
        if (board.getCell(corner.first, corner.second) == EMPTY) {
            return corner;
        }
    }
    
    // 5. Take any available move
    return getRandomMove(board);
}

std::pair<int, int> AI::getHardMove(const Board& board, CellState aiPlayer) {
    auto emptyCells = board.getEmptyCells();
    if (emptyCells.empty()) {
        return {-1, -1};
    }
    
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = {-1, -1};
    
    CellState humanPlayer = getOpponent(aiPlayer);
    
    for (const auto& cell : emptyCells) {
        Board tempBoard = board;
        tempBoard.makeMove(cell.first, cell.second, aiPlayer);
        
        int score = minimax(tempBoard, 0, false, aiPlayer, humanPlayer);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = cell;
        }
    }
    
    return bestMove;
}

int AI::minimax(Board board, int depth, bool isMaximizing, CellState aiPlayer, CellState humanPlayer) {
    WinInfo winInfo = board.checkWin();
    
    // Terminal states
    if (winInfo.hasWinner) {
        if (winInfo.winner == aiPlayer) {
            return 10 - depth; // Prefer faster wins
        } else {
            return depth - 10; // Prefer slower losses
        }
    }
    
    if (board.isFull()) {
        return 0; // Draw
    }
    
    if (isMaximizing) {
        int maxScore = INT_MIN;
        auto emptyCells = board.getEmptyCells();
        
        for (const auto& cell : emptyCells) {
            Board tempBoard = board;
            tempBoard.makeMove(cell.first, cell.second, aiPlayer);
            int score = minimax(tempBoard, depth + 1, false, aiPlayer, humanPlayer);
            maxScore = std::max(maxScore, score);
        }
        
        return maxScore;
    } else {
        int minScore = INT_MAX;
        auto emptyCells = board.getEmptyCells();
        
        for (const auto& cell : emptyCells) {
            Board tempBoard = board;
            tempBoard.makeMove(cell.first, cell.second, humanPlayer);
            int score = minimax(tempBoard, depth + 1, true, aiPlayer, humanPlayer);
            minScore = std::min(minScore, score);
        }
        
        return minScore;
    }
}

CellState AI::getOpponent(CellState player) {
    return (player == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

int AI::evaluateBoard(const Board& board, CellState aiPlayer, CellState humanPlayer) {
    WinInfo winInfo = board.checkWin();
    
    if (winInfo.hasWinner) {
        if (winInfo.winner == aiPlayer) {
            return 10;
        } else {
            return -10;
        }
    }
    
    return 0; // Draw or game not finished
}

std::pair<int, int> AI::findWinningMove(const Board& board, CellState player) {
    auto emptyCells = board.getEmptyCells();
    
    for (const auto& cell : emptyCells) {
        Board tempBoard = board;
        tempBoard.makeMove(cell.first, cell.second, player);
        
        WinInfo winInfo = tempBoard.checkWin();
        if (winInfo.hasWinner && winInfo.winner == player) {
            return cell;
        }
    }
    
    return {-1, -1}; // No winning move found
}

std::pair<int, int> AI::findBlockingMove(const Board& board, CellState opponent) {
    return findWinningMove(board, opponent);
}