#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <SDL2/SDL.h>

#include <string>

class UIButton {
public:
    UIButton(int x, int y, int w, int h, const std::string& text);
    
    // Event handling
    void handleMouseMove(int mouseX, int mouseY);
    void handleMouseClick(int mouseX, int mouseY);
    bool isClicked() const;
    void resetClick();
    
    // State queries
    bool isHovered() const;
    bool isPressed() const;
    
    // Getters
    SDL_Rect getRect() const;
    std::string getText() const;
    float getHoverAnimation() const;
    
    // Animation update
    void updateAnimation(float deltaTime);
    
private:
    SDL_Rect rect;
    std::string text;
    bool hovered;
    bool pressed;
    bool clicked;
    float hoverAnimation;  // 0.0 to 1.0 for smooth hover transitions
    
    bool isPointInRect(int x, int y) const;
};

#endif