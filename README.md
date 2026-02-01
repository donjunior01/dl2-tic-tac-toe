# 🎮 SDL2 Tic Tac Toe - Advanced Multi-Board Game

A complete, production-ready Tic Tac Toe game implemented in C++ using SDL2 with a retro aesthetic and advanced features including multiple board sizes, AI opponents, and smooth animations.

![Game Preview](https://img.shields.io/badge/Status-Production%20Ready-brightgreen)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B11-orange)
![Graphics](https://img.shields.io/badge/Graphics-SDL2-red)

## 🌟 Features

### 🎯 Game Modes
- **Player vs Player**: Two human players alternate turns on the same computer
- **Player vs AI**: Challenge AI opponents with three difficulty levels:
  - **Easy**: Random move selection
  - **Medium**: Strategic rule-based AI (blocks wins, takes winning moves)
  - **Hard**: Unbeatable Minimax algorithm with alpha-beta pruning

### 📐 Multiple Board Sizes
- **3x3 Board**: Classic tic-tac-toe (3 in a row to win)
- **4x4 Board**: Extended gameplay (4 in a row to win)
- **5x5 Board**: Strategic gameplay (4 in a row to win - balanced)
- **6x6 Board**: Advanced gameplay (5 in a row to win)

### 🎨 Visual Design
- **Retro Aesthetic**: Classic color palette with modern polish
- **Smooth Animations**: 300ms piece placement with easing effects
- **Win Highlights**: Pulsing effects on winning combinations
- **Button Hover Effects**: Smooth transitions and scaling
- **Resizable Window**: Dynamic UI scaling (initial 800x600)

### 🔊 Audio System
- **Procedural Sound Generation**: No external files required
- **Sound Effects**: Piece placement, wins, draws, button clicks
- **Mute Toggle**: Press 'M' to toggle sound on/off

### 📊 Score Tracking
- **Session Statistics**: Track X wins, O wins, draws, and total games
- **Real-time Display**: Statistics shown on menu and during gameplay

## 🚀 Quick Start

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- SDL2, SDL2_ttf, SDL2_mixer libraries

### Windows (MSYS2/MinGW)
```bash
# Install MSYS2 from https://www.msys2.org/
# Install development tools and SDL2 libraries:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer

# Add to PATH: C:\msys64\mingw64\bin
# Build the game:
.\build.ps1
# or
.\build.bat
```

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get install build-essential libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev

# Build the game
g++ -o TicTacToe main.cpp Game.cpp Board.cpp Renderer.cpp AI.cpp AudioManager.cpp UIButton.cpp ScoreTracker.cpp -lSDL2 -lSDL2_ttf -lSDL2_mixer -std=c++11
```

### macOS
```bash
# Install dependencies using Homebrew
brew install sdl2 sdl2_ttf sdl2_mixer

# Build the game
clang++ -o TicTacToe main.cpp Game.cpp Board.cpp Renderer.cpp AI.cpp AudioManager.cpp UIButton.cpp ScoreTracker.cpp -lSDL2 -lSDL2_ttf -lSDL2_mixer -std=c++11
```

## 🎮 How to Play

### Game Flow
1. **Main Menu**: Choose your opponent type (Player vs Player or vs AI)
2. **Board Size Selection**: Select board size (3x3, 4x4, 5x5, or 6x6)
3. **Gameplay**: Click cells to place your pieces
4. **Win/Draw**: Game ends when someone gets the required alignment or board is full

### Controls
#### Mouse
- **Left Click**: Place piece in empty cell
- **Button Hover**: Visual feedback on UI elements
- **Menu Navigation**: Click buttons to navigate

#### Keyboard
- **ESC**: Return to main menu or quit
- **R**: Restart/New Game (during gameplay)
- **Q**: Quit application
- **M**: Mute/unmute sounds

### Win Conditions
- **3x3 Board**: Get 3 pieces in a row (horizontal, vertical, or diagonal)
- **4x4 Board**: Get 4 pieces in a row
- **5x5 Board**: Get 4 pieces in a row (balanced for playability)
- **6x6 Board**: Get 5 pieces in a row

## 🏗️ Architecture

### Core Classes
```cpp
Game        // Main game loop, SDL initialization, state management
Board       // Variable-size grid, win detection, move validation  
Renderer    // SDL2 rendering, animations, UI scaling
AI          // Three difficulty implementations including Minimax
AudioManager// Sound effects and procedural audio generation
UIButton    // Interactive buttons with hover effects
ScoreTracker// Statistics tracking and display
```

### Game States
- **MENU**: Main menu with opponent selection
- **BOARD_SIZE_SELECTION**: Choose board size and win conditions
- **PLAYING**: Active gameplay
- **GAME_OVER**: Game ended, showing results
- **SETTINGS**: AI difficulty configuration

### Color Palette
```cpp
Background Primary:    #1a1a2e (dark navy blue)
Background Secondary:  #16213e (lighter navy)
Player X:              #ff6b6b (bright red)
Player O:              #4ecdc4 (cyan/turquoise)
Grid Lines:            #f7d794 (warm yellow/gold)
Button Default:        #0f3460 (deep blue)
Button Hover:          #e94560 (vibrant pink/red)
Text Primary:          #f1f1f1 (off-white)
Win Highlight:         #95e1d3 (mint green)
```

## 🤖 AI Implementation

### Easy Mode
Random move selection from available cells.

### Medium Mode
Rule-based strategy:
1. Try to win if possible
2. Block opponent from winning
3. Take center if available
4. Take corners
5. Take any available move

### Hard Mode (Minimax Algorithm)
```cpp
function minimax(board, depth, isMaximizing, alpha, beta):
    if terminal state (win/loss/draw):
        return score (win: +10, loss: -10, draw: 0)
    
    if isMaximizing (AI turn):
        maxScore = -infinity
        for each empty cell:
            place AI piece
            score = minimax(board, depth+1, false, alpha, beta)
            undo move
            maxScore = max(maxScore, score)
            alpha = max(alpha, score)
            if beta <= alpha: break (pruning)
        return maxScore
    else (player turn):
        minScore = +infinity
        for each empty cell:
            place player piece
            score = minimax(board, depth+1, true, alpha, beta)
            undo move
            minScore = min(minScore, score)
            beta = min(beta, score)
            if beta <= alpha: break (pruning)
        return minScore
```

## 📁 Project Structure

```
├── main.cpp              # Entry point
├── Game.h/cpp            # Main game class with state management
├── Board.h/cpp           # Variable-size board logic and win detection
├── Renderer.h/cpp        # SDL2 rendering and animations
├── AI.h/cpp              # AI implementations (Easy/Medium/Hard)
├── AudioManager.h/cpp    # Sound management and generation
├── UIButton.h/cpp        # Button UI component with animations
├── ScoreTracker.h/cpp    # Statistics tracking
├── Constants.h           # Game constants and color definitions
├── console_main.cpp      # Console version for testing
├── build.bat             # Windows batch build script
├── build.ps1             # PowerShell build script
└── README.md             # This file
```

## 🔧 Technical Features

### Performance
- **60 FPS Target**: Smooth gameplay with efficient rendering
- **Memory Management**: Proper SDL resource cleanup
- **Dynamic Scaling**: UI adapts to window resizing
- **Optimized Rendering**: Only redraws when necessary

### Cross-Platform Compatibility
- **Windows**: MSYS2/MinGW, Visual Studio
- **Linux**: GCC with standard package managers
- **macOS**: Clang with Homebrew

### Code Quality
- **Modern C++**: C++11 standard with best practices
- **Clean Architecture**: Separation of concerns
- **Error Handling**: Graceful failure management
- **Documentation**: Comprehensive code comments

## 🎯 Advanced Features

### Animations
- **Piece Placement**: 300ms fade-in with bounce effect
- **Win Highlighting**: Pulsing glow on winning combinations
- **Button Interactions**: Smooth hover transitions
- **Board Transitions**: Clean state changes

### Audio System
- **Procedural Generation**: No external audio files needed
- **Sound Effects**: 
  - Piece placement beep
  - Victory jingle (ascending tones)
  - Draw notification
  - Button click feedback
  - Invalid move buzz

### UI/UX Excellence
- **Intuitive Navigation**: Clear menu flow
- **Visual Feedback**: Immediate response to user actions
- **Accessibility**: High contrast colors and clear fonts
- **Responsive Design**: Works at various window sizes

## 🧪 Testing

### Automated Build
```bash
# PowerShell
.\build.ps1

# Batch (Windows)
.\build.bat
```

### Manual Testing
1. **Functionality**: All game modes and board sizes
2. **AI Behavior**: Each difficulty level
3. **UI Responsiveness**: All buttons and interactions
4. **Performance**: Smooth 60 FPS gameplay
5. **Memory**: No leaks or excessive usage

## 🐛 Troubleshooting

### Common Issues

**SDL2 Not Found**
```bash
# Windows: Install via MSYS2
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer

# Linux: Install development packages
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev

# macOS: Install via Homebrew
brew install sdl2 sdl2_ttf sdl2_mixer
```

**Compilation Errors**
- Ensure C++11 or later compiler
- Check SDL2 library paths
- Verify all source files are present

**Runtime Issues**
- Check audio system initialization
- Verify window creation permissions
- Ensure proper graphics drivers

## 🤝 Contributing

### Development Setup
1. Fork the repository
2. Install dependencies (SDL2, SDL2_ttf, SDL2_mixer)
3. Build using provided scripts
4. Test thoroughly before submitting

### Code Style
- Follow existing naming conventions
- Add comments for complex algorithms
- Maintain separation between game logic and rendering
- Use const correctness and RAII principles

## 📄 License

This project is open source and available.

## 🎉 Acknowledgments

- **SDL2 Team**: For the excellent multimedia library
- **MSYS2 Project**: For Windows development environment
- **Open Source Community**: For inspiration and best practices

---

**Ready to play? Run `.\build.ps1` and launch `TicTacToe.exe`! 🎮**
