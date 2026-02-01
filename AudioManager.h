#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL2/SDL_mixer.h>

#include <string>
#include <map>

enum SoundEffect {
    SOUND_PIECE_PLACE,
    SOUND_WIN,
    SOUND_DRAW,
    SOUND_BUTTON_CLICK,
    SOUND_INVALID_MOVE
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void cleanup();
    
    // Sound management
    bool loadSound(SoundEffect effect, const std::string& filename);
    void playSound(SoundEffect effect);
    void playBackgroundMusic(const std::string& filename);
    void stopBackgroundMusic();
    
    // Volume control
    void setMasterVolume(float volume); // 0.0 to 1.0
    void setSoundVolume(float volume);
    void setMusicVolume(float volume);
    
    // Mute control
    void setMuted(bool muted);
    bool isMuted() const;
    
    // Generate procedural sounds (fallback if no sound files)
    void generateProceduralSounds();
    
private:
    std::map<SoundEffect, Mix_Chunk*> sounds;
    Mix_Music* backgroundMusic;
    
    float masterVolume;
    float soundVolume;
    float musicVolume;
    bool muted;
    
    // Helper functions
    Mix_Chunk* generateBeepSound(int frequency, int duration);
    Mix_Chunk* generateToneSequence(const int* frequencies, const int* durations, int count);
};

#endif