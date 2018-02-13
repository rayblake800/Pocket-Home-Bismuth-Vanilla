#include "../../PocketHomeApplication.h"
#include "SettingsCategoryButton.h"

SettingsCategoryButton::SettingsCategoryButton(const String &name)
: Button(name),
displayText(name) {
}

void SettingsCategoryButton::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) {
    const auto& bounds = pillBounds;
    float borderThick = 4.0f;

    g.setColour(Colours::white);
    isButtonDown ? setAlpha(0.5f) : setAlpha(1.0f);

    if (isEnabled()) {
        g.drawRoundedRectangle(bounds.getX() + borderThick, bounds.getY() + borderThick,
                bounds.getWidth() - 2 * borderThick, bounds.getHeight() - 2 * borderThick,
                1, borderThick);
    }

    // TODO: write button text as grey if choice is completely unset?
    g.setFont(20);
    g.drawText(displayText, bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            Justification::centred);
}

void SettingsCategoryButton::resized() {
    pillBounds.setSize(getLocalBounds().getWidth(), 42);
    fitRectInRect(pillBounds, getLocalBounds(), Justification::centred, false);
}

void SettingsCategoryButton::setText(const String &text) {
    displayText = text;
    repaint();
}