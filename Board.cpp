#include "Board.h"

Board::Board(int size, int winCond) : boardSize(size), winCondition(winCond) {
    resize(size, winCond);
}

void Board::resize(int newSize, int newWinCondition) {
    boardSize = newSize;
    winCondition = newWinCondition;
    grid.resize(boardSize);
    for (int i = 0; i < boardSize; i++) {
        grid[i].resize(boardSize);
    }
    reset();
}

void Board::reset() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            grid[i][j] = EMPTY;
        }
    }
}

bool Board::makeMove(int row, int col, CellState player) {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) return false;
    if (grid[row][col] != EMPTY) return false;
    
    grid[row][col] = player;
    return true;
}

CellState Board::getCell(int row, int col) const {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) return EMPTY;
    return grid[row][col];
}

int Board::getSize() const {
    return boardSize;
}

int Board::getWinCondition() const {
    return winCondition;
}

bool Board::checkLine(int startRow, int startCol, int deltaRow, int deltaCol, WinInfo& winInfo) const {
    CellState firstCell = grid[startRow][startCol];
    if (firstCell == EMPTY) return false;
    
    std::vector<int> cells;
    int count = 0;
    int row = startRow;
    int col = startCol;
    
    // Check in the positive direction
    while (row >= 0 && row < boardSize && col >= 0 && col < boardSize && 
           grid[row][col] == firstCell && count < winCondition) {
        cells.push_back(row * boardSize + col);
        count++;
        row += deltaRow;
        col += deltaCol;
    }
    
    if (count >= winCondition) {
        winInfo.hasWinner = true;
        winInfo.winner = firstCell;
        winInfo.winningCells = cells;
        return true;
    }
    
    return false;
}

WinInfo Board::checkWin() const {
    WinInfo winInfo;
    
    // Check all possible starting positions and directions
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (grid[row][col] == EMPTY) continue;
            
            // Check horizontal (right)
            if (col <= boardSize - winCondition) {
                if (checkLine(row, col, 0, 1, winInfo)) return winInfo;
            }
            
            // Check vertical (down)
            if (row <= boardSize - winCondition) {
                if (checkLine(row, col, 1, 0, winInfo)) return winInfo;
            }
            
            // Check diagonal (down-right)
            if (row <= boardSize - winCondition && col <= boardSize - winCondition) {
                if (checkLine(row, col, 1, 1, winInfo)) return winInfo;
            }
            
            // Check diagonal (down-left)
            if (row <= boardSize - winCondition && col >= winCondition - 1) {
                if (checkLine(row, col, 1, -1, winInfo)) return winInfo;
            }
        }
    }
    
    return winInfo;
}

bool Board::isFull() const {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j] == EMPTY) return false;
        }
    }
    return true;
}

std::vector<std::pair<int, int>> Board::getEmptyCells() const {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (grid[i][j] == EMPTY) {
                emptyCells.push_back({i, j});
            }
        }
    }
    return emptyCells;
}
