#include "AudioManager.h"
#include "Constants.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AudioManager::AudioManager() : backgroundMusic(nullptr), masterVolume(1.0f), 
                               soundVolume(1.0f), musicVolume(0.5f), muted(false) {}

AudioManager::~AudioManager() {
    cleanup();
}

bool AudioManager::initialize() {
    if (Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNK_SIZE) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    // Set initial volumes
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * musicVolume));
    
    // Generate procedural sounds as fallback
    generateProceduralSounds();
    
    return true;
}

void AudioManager::cleanup() {
    // Free all sound effects
    for (auto& pair : sounds) {
        if (pair.second) {
            Mix_FreeChunk(pair.second);
        }
    }
    sounds.clear();
    
    // Free background music
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    
    Mix_CloseAudio();
}

bool AudioManager::loadSound(SoundEffect effect, const std::string& filename) {
    Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
    if (!sound) {
        std::cout << "Warning: Could not load sound " << filename << ". SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    // Free existing sound if any
    if (sounds[effect]) {
        Mix_FreeChunk(sounds[effect]);
    }
    
    sounds[effect] = sound;
    return true;
}

void AudioManager::playSound(SoundEffect effect) {
    if (muted) return;
    
    auto it = sounds.find(effect);
    if (it != sounds.end() && it->second) {
        int volume = (int)(MIX_MAX_VOLUME * masterVolume * soundVolume);
        Mix_VolumeChunk(it->second, volume);
        Mix_PlayChannel(-1, it->second, 0);
    }
}

void AudioManager::playBackgroundMusic(const std::string& filename) {
    if (muted) return;
    
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
    }
    
    backgroundMusic = Mix_LoadMUS(filename.c_str());
    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1); // Loop indefinitely
    } else {
        std::cout << "Warning: Could not load background music " << filename << std::endl;
    }
}

void AudioManager::stopBackgroundMusic() {
    Mix_HaltMusic();
}

void AudioManager::setMasterVolume(float volume) {
    masterVolume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
}

void AudioManager::setSoundVolume(float volume) {
    soundVolume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * musicVolume * masterVolume));
}

void AudioManager::setMuted(bool mute) {
    muted = mute;
    if (muted) {
        Mix_PauseMusic();
    } else {
        Mix_ResumeMusic();
    }
}

bool AudioManager::isMuted() const {
    return muted;
}

void AudioManager::generateProceduralSounds() {
    // Generate simple beep for piece placement
    sounds[SOUND_PIECE_PLACE] = generateBeepSound(800, 200);
    
    // Generate win sound (ascending tones)
    int winFreqs[] = {523, 659, 784, 1047}; // C, E, G, C
    int winDurations[] = {150, 150, 150, 300};
    sounds[SOUND_WIN] = generateToneSequence(winFreqs, winDurations, 4);
    
    // Generate draw sound (neutral tone)
    sounds[SOUND_DRAW] = generateBeepSound(440, 500);
    
    // Generate button click sound
    sounds[SOUND_BUTTON_CLICK] = generateBeepSound(1000, 100);
    
    // Generate invalid move sound (low buzz)
    sounds[SOUND_INVALID_MOVE] = generateBeepSound(200, 300);
}

Mix_Chunk* AudioManager::generateBeepSound(int frequency, int duration) {
    int sampleRate = AUDIO_FREQUENCY;
    int samples = (sampleRate * duration) / 1000;
    
    Uint16* buffer = new Uint16[samples];
    
    for (int i = 0; i < samples; i++) {
        double time = (double)i / sampleRate;
        double wave = sin(2.0 * M_PI * frequency * time);
        
        // Apply envelope to avoid clicks
        double envelope = 1.0;
        if (i < samples * 0.1) {
            envelope = (double)i / (samples * 0.1);
        } else if (i > samples * 0.9) {
            envelope = (double)(samples - i) / (samples * 0.1);
        }
        
        buffer[i] = (Uint16)(wave * envelope * 16383); // 16-bit audio
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW((Uint8*)buffer, samples * sizeof(Uint16));
    return chunk;
}

Mix_Chunk* AudioManager::generateToneSequence(const int* frequencies, const int* durations, int count) {
    int sampleRate = AUDIO_FREQUENCY;
    int totalSamples = 0;
    
    // Calculate total samples needed
    for (int i = 0; i < count; i++) {
        totalSamples += (sampleRate * durations[i]) / 1000;
    }
    
    Uint16* buffer = new Uint16[totalSamples];
    int currentSample = 0;
    
    for (int tone = 0; tone < count; tone++) {
        int samples = (sampleRate * durations[tone]) / 1000;
        
        for (int i = 0; i < samples; i++) {
            double time = (double)i / sampleRate;
            double wave = sin(2.0 * M_PI * frequencies[tone] * time);
            
            // Apply envelope
            double envelope = 1.0;
            if (i < samples * 0.1) {
                envelope = (double)i / (samples * 0.1);
            } else if (i > samples * 0.9) {
                envelope = (double)(samples - i) / (samples * 0.1);
            }
            
            buffer[currentSample++] = (Uint16)(wave * envelope * 16383);
        }
    }
    
    Mix_Chunk* chunk = Mix_QuickLoad_RAW((Uint8*)buffer, totalSamples * sizeof(Uint16));
    return chunk;
}