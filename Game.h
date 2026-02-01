#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <string>
#include <vector>
#include "Board.h"
#include "AI.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "UIButton.h"
#include "ScoreTracker.h"
#include "Constants.h"

enum GameMode {
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI_EASY,
    PLAYER_VS_AI_MEDIUM,
    PLAYER_VS_AI_HARD
};

enum GameState {
    MENU,
    BOARD_SIZE_SELECTION,
    PLAYING,
    GAME_OVER,
    SETTINGS
};

class Game {
public:
    Game();
    ~Game();
    
    bool initialize();
    void run();
    void cleanup();
    
private:
    // Core game loop
    void handleEvents();
    void update();
    void render();
    
    // Event handlers
    void handleMenuEvents(SDL_Event& e);
    void handleBoardSizeSelectionEvents(SDL_Event& e);
    void handleGameEvents(SDL_Event& e);
    void handleSettingsEvents(SDL_Event& e);
    void handleKeyboardEvents(SDL_Event& e);
    
    // Game logic
    void startNewGame();
    void makeMove(int row, int col);
    void switchPlayer();
    void checkGameEnd();
    void resetGame();
    
    // AI logic
    void makeAIMove();
    
    // Utility functions
    std::pair<int, int> getGridPosition(int mouseX, int mouseY);
    void updateGridDimensions();
    void initializeButtons();
    void setBoardSize(int size);
    int getWinConditionForSize(int size);
    
    // SDL components
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    
    // Game components
    Board board;
    AI ai;
    Renderer renderer;
    AudioManager audioManager;
    ScoreTracker scoreTracker;
    
    // Game state
    GameState currentState;
    GameMode gameMode;
    GameMode selectedGameMode; // Store selected mode before board size selection
    CellState currentPlayer;
    CellState humanPlayer;
    CellState aiPlayer;
    bool gameRunning;
    bool gameEnded;
    std::string statusMessage;
    
    // Board configuration
    int boardSize;
    int winCondition;
    
    // UI elements
    std::vector<UIButton> menuButtons;
    std::vector<UIButton> boardSizeButtons;
    std::vector<UIButton> settingsButtons;
    std::vector<UIButton> gameButtons;
    
    // Animation and timing
    Uint32 lastFrameTime;
    Uint32 gameEndTime;
    Uint32 lastAIMoveTime;
    
    // Grid dimensions (calculated based on window size)
    int gridStartX, gridStartY;
    int cellSize;
    
    // Window dimensions
    int windowWidth, windowHeight;
};

#endif