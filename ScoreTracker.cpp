#include "ScoreTracker.h"
#include <sstream>
#include <fstream>
#include <iostream>

ScoreTracker::ScoreTracker() {}

void ScoreTracker::recordWin(CellState winner) {
    if (winner == PLAYER_X) {
        stats.xWins++;
    } else if (winner == PLAYER_O) {
        stats.oWins++;
    }
    stats.totalGames++;
}

void ScoreTracker::recordDraw() {
    stats.draws++;
    stats.totalGames++;
}

void ScoreTracker::resetStats() {
    stats = GameStats();
}

GameStats ScoreTracker::getStats() const {
    return stats;
}

int ScoreTracker::getXWins() const {
    return stats.xWins;
}

int ScoreTracker::getOWins() const {
    return stats.oWins;
}

int ScoreTracker::getDraws() const {
    return stats.draws;
}

int ScoreTracker::getTotalGames() const {
    return stats.totalGames;
}

std::string ScoreTracker::getScoreString() const {
    std::stringstream ss;
    ss << "X: " << stats.xWins << " | O: " << stats.oWins << " | Draws: " << stats.draws;
    return ss.str();
}

std::string ScoreTracker::getStatsString() const {
    std::stringstream ss;
    ss << "Games Played: " << stats.totalGames << " | " << getScoreString();
    return ss.str();
}

bool ScoreTracker::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << stats.xWins << " " << stats.oWins << " " << stats.draws << " " << stats.totalGames;
    file.close();
    return true;
}

bool ScoreTracker::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file >> stats.xWins >> stats.oWins >> stats.draws >> stats.totalGames;
    file.close();
    return true;
}