#include "Game.h"
#include <iostream>
#include <sstream>
#include <cmath>

Game::Game() : window(nullptr), sdlRenderer(nullptr), 
               currentState(MENU), gameMode(PLAYER_VS_AI_MEDIUM), selectedGameMode(PLAYER_VS_AI_MEDIUM),
               currentPlayer(PLAYER_X), humanPlayer(PLAYER_X), aiPlayer(PLAYER_O), 
               gameRunning(true), gameEnded(false), lastFrameTime(0), gameEndTime(0),
               lastAIMoveTime(0), gridStartX(0), gridStartY(0), cellSize(0),
               windowWidth(INITIAL_WINDOW_WIDTH), windowHeight(INITIAL_WINDOW_HEIGHT),
               boardSize(DEFAULT_BOARD_SIZE), winCondition(WIN_CONDITION_3X3),
               board(DEFAULT_BOARD_SIZE, WIN_CONDITION_3X3) {
    
    ai.setLevel(MEDIUM);
}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("Tic Tac Toe - SDL2 Edition",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             windowWidth, windowHeight,
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdlRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize components
    if (!renderer.initialize(sdlRenderer)) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return false;
    }
    
    if (!audioManager.initialize()) {
        std::cerr << "Failed to initialize audio manager!" << std::endl;
        // Continue without audio
    }
    
    // Initialize buttons
    initializeButtons();
    
    // Update grid dimensions
    updateGridDimensions();
    
    return true;
}

void Game::initializeButtons() {
    // Clear existing buttons
    menuButtons.clear();
    boardSizeButtons.clear();
    settingsButtons.clear();
    gameButtons.clear();
    
    // Initialize menu buttons
    menuButtons.emplace_back(300, 200, 200, 60, "Player vs Player");
    menuButtons.emplace_back(300, 280, 200, 60, "vs AI (Easy)");
    menuButtons.emplace_back(300, 360, 200, 60, "vs AI (Medium)");
    menuButtons.emplace_back(300, 440, 200, 60, "vs AI (Hard)");
    menuButtons.emplace_back(300, 520, 200, 60, "Quit");
    
    // Initialize board size selection buttons with better positioning
    boardSizeButtons.emplace_back(300, 160, 200, 45, "3x3 Board");
    boardSizeButtons.emplace_back(300, 215, 200, 45, "4x4 Board");
    boardSizeButtons.emplace_back(300, 270, 200, 45, "5x5 Board");
    boardSizeButtons.emplace_back(300, 325, 200, 45, "6x6 Board");
    boardSizeButtons.emplace_back(300, 400, 200, 45, "Back to Menu");
    
    // Initialize settings buttons
    settingsButtons.emplace_back(200, 200, 150, 50, "Easy AI");
    settingsButtons.emplace_back(375, 200, 150, 50, "Medium AI");
    settingsButtons.emplace_back(550, 200, 150, 50, "Hard AI");
    settingsButtons.emplace_back(350, 400, 100, 50, "Back");
    
    // Initialize game buttons
    gameButtons.emplace_back(50, 50, 120, 40, "New Game");
    gameButtons.emplace_back(180, 50, 120, 40, "Main Menu");
}

void Game::run() {
    while (gameRunning) {
        Uint32 frameStart = SDL_GetTicks();
        
        handleEvents();
        update();
        render();
        
        // Cap frame rate
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
        
        lastFrameTime = frameStart;
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            gameRunning = false;
        }
        
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            windowWidth = e.window.data1;
            windowHeight = e.window.data2;
            updateGridDimensions();
        }
        
        handleKeyboardEvents(e);
        
        switch (currentState) {
            case MENU:
                handleMenuEvents(e);
                break;
            case BOARD_SIZE_SELECTION:
                handleBoardSizeSelectionEvents(e);
                break;
            case PLAYING:
            case GAME_OVER:
                handleGameEvents(e);
                break;
            case SETTINGS:
                handleSettingsEvents(e);
                break;
        }
    }
}

void Game::handleKeyboardEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                if (currentState == PLAYING || currentState == GAME_OVER) {
                    currentState = MENU;
                } else if (currentState == SETTINGS || currentState == BOARD_SIZE_SELECTION) {
                    currentState = MENU;
                } else {
                    gameRunning = false;
                }
                break;
            case SDLK_r:
                if (currentState == PLAYING || currentState == GAME_OVER) {
                    startNewGame();
                }
                break;
            case SDLK_q:
                gameRunning = false;
                break;
            case SDLK_m:
                audioManager.setMuted(!audioManager.isMuted());
                break;
        }
    }
}

void Game::handleMenuEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        for (auto& button : menuButtons) {
            button.handleMouseMove(e.motion.x, e.motion.y);
        }
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        for (size_t i = 0; i < menuButtons.size(); i++) {
            menuButtons[i].handleMouseClick(e.button.x, e.button.y);
            if (menuButtons[i].isClicked()) {
                audioManager.playSound(SOUND_BUTTON_CLICK);
                
                switch (i) {
                    case 0: // Player vs Player
                        selectedGameMode = PLAYER_VS_PLAYER;
                        currentState = BOARD_SIZE_SELECTION;
                        break;
                    case 1: // vs AI (Easy)
                        selectedGameMode = PLAYER_VS_AI_EASY;
                        ai.setLevel(EASY);
                        currentState = BOARD_SIZE_SELECTION;
                        break;
                    case 2: // vs AI (Medium)
                        selectedGameMode = PLAYER_VS_AI_MEDIUM;
                        ai.setLevel(MEDIUM);
                        currentState = BOARD_SIZE_SELECTION;
                        break;
                    case 3: // vs AI (Hard)
                        selectedGameMode = PLAYER_VS_AI_HARD;
                        ai.setLevel(HARD);
                        currentState = BOARD_SIZE_SELECTION;
                        break;
                    case 4: // Quit
                        gameRunning = false;
                        break;
                }
                
                menuButtons[i].resetClick();
                break;
            }
        }
    }
}

void Game::handleBoardSizeSelectionEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        for (auto& button : boardSizeButtons) {
            button.handleMouseMove(e.motion.x, e.motion.y);
        }
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        for (size_t i = 0; i < boardSizeButtons.size(); i++) {
            boardSizeButtons[i].handleMouseClick(e.button.x, e.button.y);
            if (boardSizeButtons[i].isClicked()) {
                audioManager.playSound(SOUND_BUTTON_CLICK);
                
                switch (i) {
                    case 0: // 3x3 Board
                        setBoardSize(3);
                        gameMode = selectedGameMode;
                        startNewGame();
                        break;
                    case 1: // 4x4 Board
                        setBoardSize(4);
                        gameMode = selectedGameMode;
                        startNewGame();
                        break;
                    case 2: // 5x5 Board
                        setBoardSize(5);
                        gameMode = selectedGameMode;
                        startNewGame();
                        break;
                    case 3: // 6x6 Board
                        setBoardSize(6);
                        gameMode = selectedGameMode;
                        startNewGame();
                        break;
                    case 4: // Back to Menu
                        currentState = MENU;
                        break;
                }
                
                boardSizeButtons[i].resetClick();
                break;
            }
        }
    }
}

void Game::handleGameEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        for (auto& button : gameButtons) {
            button.handleMouseMove(e.motion.x, e.motion.y);
        }
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        // Check game buttons first
        for (size_t i = 0; i < gameButtons.size(); i++) {
            gameButtons[i].handleMouseClick(e.button.x, e.button.y);
            if (gameButtons[i].isClicked()) {
                audioManager.playSound(SOUND_BUTTON_CLICK);
                
                switch (i) {
                    case 0: // New Game
                        startNewGame();
                        break;
                    case 1: // Main Menu
                        currentState = MENU;
                        break;
                }
                
                gameButtons[i].resetClick();
                return;
            }
        }
        
        // Handle grid clicks only if game is active and it's human player's turn
        if (!gameEnded && (gameMode == PLAYER_VS_PLAYER || 
                          (gameMode != PLAYER_VS_PLAYER && currentPlayer == humanPlayer))) {
            auto gridPos = getGridPosition(e.button.x, e.button.y);
            if (gridPos.first != -1 && gridPos.second != -1) {
                if (board.getCell(gridPos.first, gridPos.second) == EMPTY) {
                    makeMove(gridPos.first, gridPos.second);
                } else {
                    audioManager.playSound(SOUND_INVALID_MOVE);
                }
            }
        }
    }
}

void Game::handleSettingsEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        for (auto& button : settingsButtons) {
            button.handleMouseMove(e.motion.x, e.motion.y);
        }
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        for (size_t i = 0; i < settingsButtons.size(); i++) {
            settingsButtons[i].handleMouseClick(e.button.x, e.button.y);
            if (settingsButtons[i].isClicked()) {
                audioManager.playSound(SOUND_BUTTON_CLICK);
                
                switch (i) {
                    case 0: // Easy AI
                        ai.setLevel(EASY);
                        break;
                    case 1: // Medium AI
                        ai.setLevel(MEDIUM);
                        break;
                    case 2: // Hard AI
                        ai.setLevel(HARD);
                        break;
                    case 3: // Back
                        currentState = MENU;
                        break;
                }
                
                settingsButtons[i].resetClick();
                break;
            }
        }
    }
}

void Game::update() {
    // Update grid dimensions if window was resized
    updateGridDimensions();
    
    // Update button animations
    float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    for (auto& button : menuButtons) {
        button.updateAnimation(deltaTime);
    }
    for (auto& button : boardSizeButtons) {
        button.updateAnimation(deltaTime);
    }
    for (auto& button : gameButtons) {
        button.updateAnimation(deltaTime);
    }
    for (auto& button : settingsButtons) {
        button.updateAnimation(deltaTime);
    }
    
    if (currentState == PLAYING && !gameEnded) {
        // Handle AI moves
        if (gameMode != PLAYER_VS_PLAYER && currentPlayer == aiPlayer) {
            // Add delay for AI moves to make them visible
            if (SDL_GetTicks() - lastAIMoveTime > 500) { // 500ms delay
                makeAIMove();
                lastAIMoveTime = SDL_GetTicks();
            }
        }
        
        checkGameEnd();
    }
}

void Game::render() {
    renderer.clearScreen();
    
    switch (currentState) {
        case MENU:
            {
                std::string difficultyText = "Current AI: ";
                switch (ai.getLevel()) {
                    case EASY: difficultyText += "Easy"; break;
                    case MEDIUM: difficultyText += "Medium"; break;
                    case HARD: difficultyText += "Hard"; break;
                }
                difficultyText += " | " + scoreTracker.getStatsString();
                renderer.renderMenu(menuButtons, difficultyText);
            }
            break;
        case BOARD_SIZE_SELECTION:
            renderer.renderBoardSizeSelection(boardSizeButtons, boardSize);
            break;
        case PLAYING:
        case GAME_OVER:
            renderer.renderGame(board, gameButtons, statusMessage, currentPlayer, gameEnded);
            break;
        case SETTINGS:
            renderer.renderSettings(settingsButtons, ai.getLevel());
            break;
    }
    
    renderer.present();
}

void Game::startNewGame() {
    board.reset();
    currentPlayer = PLAYER_X;
    gameEnded = false;
    statusMessage = "";
    currentState = PLAYING;
    gameEndTime = 0;
    lastAIMoveTime = SDL_GetTicks();
    
    // Clear all animations to ensure clean board
    renderer.clearAllAnimations();
    
    // Set up players based on game mode
    if (gameMode == PLAYER_VS_PLAYER) {
        humanPlayer = PLAYER_X; // Both players are human, but we track turns
        aiPlayer = EMPTY; // No AI
    } else {
        humanPlayer = PLAYER_X;
        aiPlayer = PLAYER_O;
    }
}

void Game::makeMove(int row, int col) {
    if (board.makeMove(row, col, currentPlayer)) {
        audioManager.playSound(SOUND_PIECE_PLACE);
        renderer.startPieceAnimation(row, col, currentPlayer);
        
        checkGameEnd();
        if (!gameEnded) {
            switchPlayer();
        }
    }
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

void Game::checkGameEnd() {
    WinInfo winInfo = board.checkWin();
    
    if (winInfo.hasWinner) {
        gameEnded = true;
        gameEndTime = SDL_GetTicks();
        currentState = GAME_OVER;
        
        // Record the win
        scoreTracker.recordWin(winInfo.winner);
        
        // Start win animation
        renderer.startWinLineAnimation(winInfo.winningCells, boardSize);
        
        // Set status message
        if (gameMode == PLAYER_VS_PLAYER) {
            statusMessage = (winInfo.winner == PLAYER_X) ? "Player X Wins!" : "Player O Wins!";
        } else {
            if (winInfo.winner == humanPlayer) {
                statusMessage = "You Win!";
                audioManager.playSound(SOUND_WIN);
            } else {
                statusMessage = "AI Wins!";
                audioManager.playSound(SOUND_WIN);
            }
        }
    } else if (board.isFull()) {
        gameEnded = true;
        gameEndTime = SDL_GetTicks();
        currentState = GAME_OVER;
        statusMessage = "It's a Draw!";
        
        // Record the draw
        scoreTracker.recordDraw();
        
        audioManager.playSound(SOUND_DRAW);
    }
}

void Game::makeAIMove() {
    auto move = ai.getMove(board, aiPlayer);
    
    if (move.first != -1 && move.second != -1) {
        makeMove(move.first, move.second);
    }
}

std::pair<int, int> Game::getGridPosition(int mouseX, int mouseY) {
    if (mouseX < gridStartX || mouseX >= gridStartX + cellSize * boardSize ||
        mouseY < gridStartY || mouseY >= gridStartY + cellSize * boardSize) {
        return {-1, -1};
    }
    
    int col = (mouseX - gridStartX) / cellSize;
    int row = (mouseY - gridStartY) / cellSize;
    
    return {row, col};
}

void Game::updateGridDimensions() {
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    cellSize = std::min(windowWidth, windowHeight - 150) / boardSize;
    if (cellSize < 30) cellSize = 30; // Minimum cell size for larger boards
    
    gridStartX = (windowWidth - cellSize * boardSize) / 2;
    gridStartY = (windowHeight - cellSize * boardSize) / 2 + 50;
    
    renderer.setGridDimensions(gridStartX, gridStartY, cellSize);
}

void Game::setBoardSize(int size) {
    boardSize = size;
    winCondition = getWinConditionForSize(size);
    board.resize(boardSize, winCondition);
    updateGridDimensions();
}

int Game::getWinConditionForSize(int size) {
    switch (size) {
        case 3: return WIN_CONDITION_3X3;
        case 4: return WIN_CONDITION_4X4;
        case 5: return WIN_CONDITION_5X5;
        case 6: return WIN_CONDITION_6X6;
        default: return 3;
    }
}

void Game::cleanup() {
    renderer.cleanup();
    audioManager.cleanup();
    
    if (sdlRenderer) {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}