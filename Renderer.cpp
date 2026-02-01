#include "Renderer.h"
#include "UIButton.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Renderer::Renderer() : renderer(nullptr), titleFont(nullptr), buttonFont(nullptr), 
                       statusFont(nullptr), gridStartX(0), gridStartY(0), cellSize(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize(SDL_Renderer* sdlRenderer) {
    renderer = sdlRenderer;
    
    // Try to load fonts - use system default if not available
    titleFont = TTF_OpenFont("arial.ttf", FONT_SIZE_TITLE);
    buttonFont = TTF_OpenFont("arial.ttf", FONT_SIZE_BUTTON);
    statusFont = TTF_OpenFont("arial.ttf", FONT_SIZE_STATUS);
    
    // If system fonts not found, try common font paths
    if (!titleFont) {
        titleFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FONT_SIZE_TITLE);
    }
    if (!buttonFont) {
        buttonFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FONT_SIZE_BUTTON);
    }
    if (!statusFont) {
        statusFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FONT_SIZE_STATUS);
    }
    
    if (!titleFont || !buttonFont || !statusFont) {
        std::cout << "Warning: Could not load fonts. Text rendering may be limited." << std::endl;
    }
    
    return true;
}

void Renderer::cleanup() {
    if (titleFont) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }
    if (buttonFont) {
        TTF_CloseFont(buttonFont);
        buttonFont = nullptr;
    }
    if (statusFont) {
        TTF_CloseFont(statusFont);
        statusFont = nullptr;
    }
}

void Renderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, COLOR_BG_PRIMARY.r, COLOR_BG_PRIMARY.g, 
                          COLOR_BG_PRIMARY.b, COLOR_BG_PRIMARY.a);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

void Renderer::setGridDimensions(int startX, int startY, int size) {
    gridStartX = startX;
    gridStartY = startY;
    cellSize = size;
}

void Renderer::renderMenu(const std::vector<UIButton>& buttons, const std::string& difficultyText) {
    // Render title
    renderText("TIC TAC TOE", 400, 100, FONT_SIZE_TITLE, COLOR_TEXT);
    
    // Render menu buttons
    for (const auto& button : buttons) {
        renderButton(button);
    }
    
    // Render AI difficulty indicator
    renderText(difficultyText, 400, 50, FONT_SIZE_STATUS, COLOR_TEXT);
}

void Renderer::renderBoardSizeSelection(const std::vector<UIButton>& buttons, int selectedSize) {
    // Render title with appropriate font
    renderText("SELECT BOARD SIZE", 400, 40, FONT_SIZE_BUTTON, COLOR_TEXT);
    
    // Render instructions with smaller font
    renderText("Choose your board size and win condition:", 400, 75, FONT_SIZE_SMALL, COLOR_TEXT);
    
    // Render board size options with win conditions using smaller fonts
    std::string sizeTexts[] = {
        "3x3 (3 in a row)",
        "4x4 (4 in a row)", 
        "5x5 (4 in a row)",
        "6x6 (5 in a row)"
    };
    
    // Render size descriptions positioned above each button with shorter text
    int buttonYPositions[] = {160, 215, 270, 325};
    for (int i = 0; i < 4; i++) {
        SDL_Color textColor = (selectedSize == i + 3) ? COLOR_PLAYER_X : COLOR_TEXT;
        renderText(sizeTexts[i], 400, buttonYPositions[i] - 12, FONT_SIZE_SMALL, textColor);
    }
    
    // Render buttons
    for (const auto& button : buttons) {
        renderButton(button);
    }
}

void Renderer::renderGame(const Board& board, const std::vector<UIButton>& buttons, 
                         const std::string& statusMessage, CellState currentPlayer, bool gameEnded) {
    // Render game buttons
    for (const auto& button : buttons) {
        renderButton(button);
    }
    
    // Render board
    WinInfo winInfo = board.checkWin();
    renderBoard(board, winInfo);
    
    // Render status message
    if (!statusMessage.empty()) {
        renderText(statusMessage, 400, 500, FONT_SIZE_STATUS, COLOR_TEXT);
    }
    
    // Render current player indicator
    if (!gameEnded) {
        std::string playerText = "Current Player: ";
        playerText += (currentPlayer == PLAYER_X) ? "X" : "O";
        SDL_Color playerColor = (currentPlayer == PLAYER_X) ? COLOR_PLAYER_X : COLOR_PLAYER_O;
        renderText(playerText, 400, 450, FONT_SIZE_STATUS, playerColor);
    }
    
    // Update and render animations
    updateAnimations();
}

void Renderer::renderSettings(const std::vector<UIButton>& buttons, int currentAILevel) {
    // Render title
    renderText("SETTINGS", 400, 100, FONT_SIZE_TITLE, COLOR_TEXT);
    
    // Render current AI level
    renderText("AI Difficulty Level", 400, 150, FONT_SIZE_BUTTON, COLOR_TEXT);
    
    // Render settings buttons
    for (const auto& button : buttons) {
        renderButton(button);
    }
}

void Renderer::renderBoard(const Board& board, const WinInfo& winInfo) {
    int boardSize = board.getSize();
    
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, COLOR_GRID.a);
    
    // Vertical lines
    for (int i = 1; i < boardSize; i++) {
        SDL_Rect line = {gridStartX + i * cellSize - GRID_LINE_THICKNESS/2, gridStartY, 
                        GRID_LINE_THICKNESS, cellSize * boardSize};
        SDL_RenderFillRect(renderer, &line);
    }
    
    // Horizontal lines
    for (int i = 1; i < boardSize; i++) {
        SDL_Rect line = {gridStartX, gridStartY + i * cellSize - GRID_LINE_THICKNESS/2, 
                        cellSize * boardSize, GRID_LINE_THICKNESS};
        SDL_RenderFillRect(renderer, &line);
    }
    
    // Draw pieces
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            CellState cell = board.getCell(row, col);
            if (cell != EMPTY) {
                renderPiece(row, col, cell);
            }
        }
    }
    
    // Highlight winning line if game is over
    if (winInfo.hasWinner) {
        SDL_SetRenderDrawColor(renderer, COLOR_WIN_HIGHLIGHT.r, COLOR_WIN_HIGHLIGHT.g, 
                              COLOR_WIN_HIGHLIGHT.b, 128); // Semi-transparent
        
        for (int cellIndex : winInfo.winningCells) {
            int row = cellIndex / boardSize;
            int col = cellIndex % boardSize;
            SDL_Rect highlight = {gridStartX + col * cellSize + 10, 
                                 gridStartY + row * cellSize + 10,
                                 cellSize - 20, cellSize - 20};
            SDL_RenderFillRect(renderer, &highlight);
        }
    }
}

void Renderer::renderPiece(int row, int col, CellState player, float scale, float alpha) {
    int centerX = gridStartX + col * cellSize + cellSize / 2;
    int centerY = gridStartY + row * cellSize + cellSize / 2;
    int size = (cellSize / 3) * scale;
    
    if (player == PLAYER_X) {
        SDL_Color color = COLOR_PLAYER_X;
        color.a = (Uint8)(255 * alpha);
        
        // Draw X as two thick lines
        drawThickLine(centerX - size, centerY - size, centerX + size, centerY + size, 
                     PIECE_THICKNESS, color);
        drawThickLine(centerX + size, centerY - size, centerX - size, centerY + size, 
                     PIECE_THICKNESS, color);
    } else if (player == PLAYER_O) {
        SDL_Color color = COLOR_PLAYER_O;
        color.a = (Uint8)(255 * alpha);
        
        // Draw O as a thick circle
        drawThickCircle(centerX, centerY, size, PIECE_THICKNESS, color);
    }
}

void Renderer::renderButton(const UIButton& button) {
    SDL_Color bgColor = button.isHovered() ? COLOR_BUTTON_HOVER : COLOR_BUTTON_DEFAULT;
    
    // Apply hover scale
    SDL_Rect rect = button.getRect();
    if (button.isHovered()) {
        int scaleIncrease = (int)(rect.w * 0.05f);
        rect.x -= scaleIncrease / 2;
        rect.y -= scaleIncrease / 2;
        rect.w += scaleIncrease;
        rect.h += scaleIncrease;
    }
    
    // Draw button background
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b, COLOR_TEXT.a);
    SDL_RenderDrawRect(renderer, &rect);
    
    // Draw button text
    renderText(button.getText(), rect.x + rect.w/2, rect.y + rect.h/2, FONT_SIZE_BUTTON, COLOR_TEXT);
}

void Renderer::renderText(const std::string& text, int x, int y, int fontSize, SDL_Color color, bool centered) {
    TTF_Font* font = nullptr;
    
    // Select appropriate font based on size with better scaling
    if (fontSize >= FONT_SIZE_TITLE && titleFont) {
        font = titleFont;
    } else if (fontSize >= FONT_SIZE_BUTTON && buttonFont) {
        font = buttonFont;
    } else if (fontSize >= FONT_SIZE_STATUS && statusFont) {
        font = statusFont;
    } else if (statusFont) {
        // Use status font for small text as well
        font = statusFont;
    }
    
    if (font) {
        SDL_Texture* textTexture = createTextTexture(text, font, color);
        if (textTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
            
            // Scale down the text if it's using a larger font for smaller font sizes
            float scale = 1.0f;
            if (fontSize < FONT_SIZE_STATUS && font == statusFont) {
                scale = (float)fontSize / FONT_SIZE_STATUS;
            }
            
            int scaledWidth = (int)(textWidth * scale);
            int scaledHeight = (int)(textHeight * scale);
            
            SDL_Rect destRect;
            if (centered) {
                destRect = {x - scaledWidth/2, y - scaledHeight/2, scaledWidth, scaledHeight};
            } else {
                destRect = {x, y, scaledWidth, scaledHeight};
            }
            
            SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
            SDL_DestroyTexture(textTexture);
        }
    } else {
        // Fallback: simple rectangle representation with smaller size
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        int textWidth = text.length() * (fontSize / 4); // Reduced from /3 to /4
        int textHeight = fontSize * 0.8f; // Reduced height
        
        SDL_Rect textRect;
        if (centered) {
            textRect = {x - textWidth/2, y - textHeight/2, textWidth, textHeight};
        } else {
            textRect = {x, y, textWidth, textHeight};
        }
        
        SDL_RenderDrawRect(renderer, &textRect);
    }
}

void Renderer::updateAnimations() {
    Uint32 currentTime = SDL_GetTicks();
    
    // Update piece animations
    for (auto& anim : pieceAnimations) {
        if (anim.active) {
            float elapsed = currentTime - anim.startTime;
            anim.progress = elapsed / PIECE_ANIMATION_DURATION;
            
            if (anim.progress >= 1.0f) {
                anim.progress = 1.0f;
                anim.active = false;
            }
            
            // Render animated piece
            float scale = PIECE_START_SCALE + (1.0f - PIECE_START_SCALE) * easeOutBounce(anim.progress);
            float alpha = anim.progress;
            renderPiece(anim.row, anim.col, anim.player, scale, alpha);
        }
    }
    
    // Update win line animation
    if (winLineAnim.active) {
        float elapsed = currentTime - winLineAnim.startTime;
        winLineAnim.progress = elapsed / WIN_LINE_ANIMATION_DURATION;
        
        if (winLineAnim.progress >= 1.0f) {
            winLineAnim.progress = 1.0f;
        }
        
        // Render pulsing win line effect
        float pulse = 0.5f + 0.5f * sin(winLineAnim.progress * M_PI * 4);
        SDL_Color color = COLOR_WIN_HIGHLIGHT;
        color.a = (Uint8)(255 * pulse);
        
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        
        // Get board size from the first winning cell to calculate row/col correctly
        int boardSize = 3; // Default, but this should be passed or stored
        if (!winLineAnim.winningCells.empty()) {
            // We need to determine board size - for now assume it's stored somewhere accessible
            // This is a limitation that should be addressed by passing board size to renderer
        }
        
        for (int cellIndex : winLineAnim.winningCells) {
            int row = cellIndex / winLineAnim.boardSize;
            int col = cellIndex % winLineAnim.boardSize;
            SDL_Rect highlight = {gridStartX + col * cellSize + 5, 
                                 gridStartY + row * cellSize + 5,
                                 cellSize - 10, cellSize - 10};
            SDL_RenderFillRect(renderer, &highlight);
        }
    }
}

void Renderer::startPieceAnimation(int row, int col, CellState player) {
    PieceAnimation anim;
    anim.row = row;
    anim.col = col;
    anim.player = player;
    anim.progress = 0.0f;
    anim.startTime = SDL_GetTicks();
    anim.active = true;
    
    pieceAnimations.push_back(anim);
}

void Renderer::startWinLineAnimation(const std::vector<int>& winningCells, int boardSize) {
    winLineAnim.winningCells = winningCells;
    winLineAnim.progress = 0.0f;
    winLineAnim.startTime = SDL_GetTicks();
    winLineAnim.active = true;
    winLineAnim.boardSize = boardSize;
}

void Renderer::clearAllAnimations() {
    // Clear all piece animations
    pieceAnimations.clear();
    
    // Clear win line animation
    winLineAnim.active = false;
    winLineAnim.progress = 0.0f;
    winLineAnim.winningCells.clear();
}

float Renderer::easeOutBounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

float Renderer::easeInOutQuad(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

void Renderer::drawThickLine(int x1, int y1, int x2, int y2, int thickness, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    for (int i = -thickness/2; i <= thickness/2; i++) {
        for (int j = -thickness/2; j <= thickness/2; j++) {
            SDL_RenderDrawLine(renderer, x1 + i, y1 + j, x2 + i, y2 + j);
        }
    }
}

void Renderer::drawThickCircle(int centerX, int centerY, int radius, int thickness, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    for (int t = 0; t < thickness; t++) {
        int currentRadius = radius - t/2;
        for (int angle = 0; angle < 360; angle += 1) {
            double rad = angle * M_PI / 180.0;
            int x = centerX + currentRadius * cos(rad);
            int y = centerY + currentRadius * sin(rad);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

SDL_Texture* Renderer::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) {
    if (!font) return nullptr;
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) return nullptr;
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    return textTexture;
}