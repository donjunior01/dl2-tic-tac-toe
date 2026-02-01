#ifndef SCORETRACKER_H
#define SCORETRACKER_H

#include "Board.h"
#include <string>

struct GameStats {
    int xWins;
    int oWins;
    int draws;
    int totalGames;
    
    GameStats() : xWins(0), oWins(0), draws(0), totalGames(0) {}
};

class ScoreTracker {
public:
    ScoreTracker();
    
    // Score management
    void recordWin(CellState winner);
    void recordDraw();
    void resetStats();
    
    // Getters
    GameStats getStats() const;
    int getXWins() const;
    int getOWins() const;
    int getDraws() const;
    int getTotalGames() const;
    
    // Display helpers
    std::string getScoreString() const;
    std::string getStatsString() const;
    
    // File persistence (optional)
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
private:
    GameStats stats;
};

#endif