@echo off
echo Building SDL2 Tic Tac Toe Game...

REM Add MSYS2 MinGW64 to PATH
set PATH=C:\msys64\mingw64\bin;%PATH%

REM Compile the main SDL2 version
echo Compiling SDL2 version...
g++ -o TicTacToe main.cpp Game.cpp Board.cpp Renderer.cpp AI.cpp AudioManager.cpp UIButton.cpp ScoreTracker.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -std=c++11

if %ERRORLEVEL% EQU 0 (
    echo SDL2 version compiled successfully!
) else (
    echo Failed to compile SDL2 version!
    goto :end
)

REM Compile the console version
echo Compiling console version...
g++ -o console_tictactoe console_main.cpp Board.cpp AI.cpp ScoreTracker.cpp -std=c++11

if %ERRORLEVEL% EQU 0 (
    echo Console version compiled successfully!
    echo.
    echo Build completed successfully!
    echo Run TicTacToe.exe for the SDL2 GUI version
    echo Run console_tictactoe.exe for the console version
) else (
    echo Failed to compile console version!
)

:end
pause