#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>

enum CellState {
    EMPTY = 0,
    PLAYER_X = 1,
    PLAYER_O = 2
};

struct WinInfo {
    bool hasWinner;
    CellState winner;
    std::vector<int> winningCells; // Indices of winning cells
    
    WinInfo() : hasWinner(false), winner(EMPTY) {}
};

class Board {
public:
    Board(int size = 3, int winCondition = 3);
    void reset();
    void resize(int newSize, int newWinCondition);
    bool makeMove(int row, int col, CellState player);
    CellState getCell(int row, int col) const;
    WinInfo checkWin() const;
    bool isFull() const;
    std::vector<std::pair<int, int>> getEmptyCells() const;
    int getSize() const;
    int getWinCondition() const;
    
private:
    std::vector<std::vector<CellState>> grid;
    int boardSize;
    int winCondition;
    
    bool checkLine(int startRow, int startCol, int deltaRow, int deltaCol, WinInfo& winInfo) const;
};

#endif
