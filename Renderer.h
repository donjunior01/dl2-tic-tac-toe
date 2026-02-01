#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>
#include "Board.h"
#include "Constants.h"

struct PieceAnimation {
    int row, col;
    CellState player;
    float progress;        // 0.0 to 1.0
    Uint32 startTime;
    bool active;
    
    PieceAnimation() : row(0), col(0), player(EMPTY), progress(0.0f), startTime(0), active(false) {}
};

struct WinLineAnimation {
    std::vector<int> winningCells;
    float progress;        // 0.0 to 1.0
    Uint32 startTime;
    bool active;
    int boardSize;         // Store board size for proper rendering
    
    WinLineAnimation() : progress(0.0f), startTime(0), active(false), boardSize(3) {}
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool initialize(SDL_Renderer* renderer);
    void cleanup();
    
    // Main rendering functions
    void renderMenu(const std::vector<class UIButton>& buttons, const std::string& difficultyText);
    void renderBoardSizeSelection(const std::vector<class UIButton>& buttons, int selectedSize);
    void renderGame(const Board& board, const std::vector<class UIButton>& buttons, 
                   const std::string& statusMessage, CellState currentPlayer, bool gameEnded);
    void renderSettings(const std::vector<class UIButton>& buttons, int currentAILevel);
    
    // Individual rendering functions
    void renderBoard(const Board& board, const WinInfo& winInfo);
    void renderPiece(int row, int col, CellState player, float scale = 1.0f, float alpha = 1.0f);
    void renderButton(const class UIButton& button);
    void renderText(const std::string& text, int x, int y, int fontSize, SDL_Color color, bool centered = true);
    
    // Animation functions
    void updateAnimations();
    void startPieceAnimation(int row, int col, CellState player);
    void startWinLineAnimation(const std::vector<int>& winningCells, int boardSize);
    void clearAllAnimations();
    
    // Utility functions
    void setGridDimensions(int startX, int startY, int cellSize);
    void clearScreen();
    void present();
    
private:
    SDL_Renderer* renderer;
    TTF_Font* titleFont;
    TTF_Font* buttonFont;
    TTF_Font* statusFont;
    
    // Grid dimensions
    int gridStartX, gridStartY;
    int cellSize;
    
    // Animation elements
    std::vector<PieceAnimation> pieceAnimations;
    WinLineAnimation winLineAnim;
    
    // Helper functions
    float easeOutBounce(float t);
    float easeInOutQuad(float t);
    void drawThickLine(int x1, int y1, int x2, int y2, int thickness, SDL_Color color);
    void drawThickCircle(int centerX, int centerY, int radius, int thickness, SDL_Color color);
    SDL_Texture* createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);
};

#endif