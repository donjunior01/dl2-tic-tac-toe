#include "UIButton.h"
#include "Constants.h"

UIButton::UIButton(int x, int y, int w, int h, const std::string& txt) 
    : rect{x, y, w, h}, text(txt), hovered(false), pressed(false), 
      clicked(false), hoverAnimation(0.0f) {}

void UIButton::handleMouseMove(int mouseX, int mouseY) {
    hovered = isPointInRect(mouseX, mouseY);
}

void UIButton::handleMouseClick(int mouseX, int mouseY) {
    if (isPointInRect(mouseX, mouseY)) {
        clicked = true;
        pressed = true;
    }
}

bool UIButton::isClicked() const {
    return clicked;
}

void UIButton::resetClick() {
    clicked = false;
    pressed = false;
}

bool UIButton::isHovered() const {
    return hovered;
}

bool UIButton::isPressed() const {
    return pressed;
}

SDL_Rect UIButton::getRect() const {
    return rect;
}

std::string UIButton::getText() const {
    return text;
}

float UIButton::getHoverAnimation() const {
    return hoverAnimation;
}

void UIButton::updateAnimation(float deltaTime) {
    const float animationSpeed = 1000.0f / BUTTON_HOVER_DURATION; // transitions per second
    
    if (hovered) {
        hoverAnimation += animationSpeed * deltaTime;
        if (hoverAnimation > 1.0f) hoverAnimation = 1.0f;
    } else {
        hoverAnimation -= animationSpeed * deltaTime;
        if (hoverAnimation < 0.0f) hoverAnimation = 0.0f;
    }
}

bool UIButton::isPointInRect(int x, int y) const {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}