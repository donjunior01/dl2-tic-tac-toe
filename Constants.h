#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Window settings
const int INITIAL_WINDOW_WIDTH = 800;
const int INITIAL_WINDOW_HEIGHT = 600;
const int TARGET_FPS = 60;
const int FRAME_DELAY = 1000 / TARGET_FPS;

// Board size options
const int MIN_BOARD_SIZE = 3;
const int MAX_BOARD_SIZE = 6;
const int DEFAULT_BOARD_SIZE = 3;

// Win condition rules (minimum alignment needed to win for each board size)
const int WIN_CONDITION_3X3 = 3;  // 3 in a row for 3x3
const int WIN_CONDITION_4X4 = 4;  // 4 in a row for 4x4
const int WIN_CONDITION_5X5 = 4;  // 4 in a row for 5x5 (not 5 to keep it playable)
const int WIN_CONDITION_6X6 = 5;  // 5 in a row for 6x6

// Colors (RGB format)
const SDL_Color COLOR_BG_PRIMARY = {26, 26, 46, 255};      // #1a1a2e
const SDL_Color COLOR_BG_SECONDARY = {22, 33, 62, 255};    // #16213e
const SDL_Color COLOR_PLAYER_X = {255, 107, 107, 255};     // #ff6b6b
const SDL_Color COLOR_PLAYER_O = {78, 205, 196, 255};      // #4ecdc4
const SDL_Color COLOR_GRID = {247, 215, 148, 255};         // #f7d794
const SDL_Color COLOR_BUTTON_DEFAULT = {15, 52, 96, 255};  // #0f3460
const SDL_Color COLOR_BUTTON_HOVER = {233, 69, 96, 255};   // #e94560
const SDL_Color COLOR_TEXT = {241, 241, 241, 255};         // #f1f1f1
const SDL_Color COLOR_WIN_HIGHLIGHT = {149, 225, 211, 255}; // #95e1d3

// Grid settings
const int GRID_LINE_THICKNESS = 8;
const int PIECE_THICKNESS = 12;

// Animation settings
const int PIECE_ANIMATION_DURATION = 300; // milliseconds
const int BUTTON_HOVER_DURATION = 150;    // milliseconds
const float PIECE_START_SCALE = 0.5f;
const float BUTTON_HOVER_SCALE = 1.05f;

// Font sizes
const int FONT_SIZE_TITLE = 48;
const int FONT_SIZE_BUTTON = 24;
const int FONT_SIZE_STATUS = 20;
const int FONT_SIZE_SMALL = 16;

// Audio settings
const int AUDIO_FREQUENCY = 44100;
const int AUDIO_CHANNELS = 2;
const int AUDIO_CHUNK_SIZE = 2048;

// Animation settings (additional)
const float WIN_LINE_ANIMATION_DURATION = 1000.0f; // milliseconds
const float MENU_TRANSITION_DURATION = 300.0f;     // milliseconds
const float PIECE_BOUNCE_HEIGHT = 0.2f;            // Scale factor for bounce effect

#endif
