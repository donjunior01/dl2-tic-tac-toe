#!/usr/bin/env pwsh

Write-Host "Building SDL2 Tic Tac Toe Game..." -ForegroundColor Green

# Add MSYS2 MinGW64 to PATH
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH

# Compile the main SDL2 version
Write-Host "Compiling SDL2 version..." -ForegroundColor Yellow
$result = & g++ -o TicTacToe main.cpp Game.cpp Board.cpp Renderer.cpp AI.cpp AudioManager.cpp UIButton.cpp ScoreTracker.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -std=c++11 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "SDL2 version compiled successfully!" -ForegroundColor Green
} else {
    Write-Host "Failed to compile SDL2 version!" -ForegroundColor Red
    Write-Host $result -ForegroundColor Red
    exit 1
}

# Compile the console version
Write-Host "Compiling console version..." -ForegroundColor Yellow
$result = & g++ -o console_tictactoe console_main.cpp Board.cpp AI.cpp ScoreTracker.cpp -std=c++11 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "Console version compiled successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Build completed successfully!" -ForegroundColor Green
    Write-Host "Run TicTacToe.exe for the SDL2 GUI version" -ForegroundColor Cyan
    Write-Host "Run console_tictactoe.exe for the console version" -ForegroundColor Cyan
} else {
    Write-Host "Failed to compile console version!" -ForegroundColor Red
    Write-Host $result -ForegroundColor Red
    exit 1
}