/**
 * @file SettingsCagegoryItemComponent.h
 * 
 * TODO: documentation
 */
#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../Basic Components/SwitchComponent.h"
#include "SettingsCategoryButton.h"
class SettingsCategoryItemComponent : public Component,
private Button::Listener{
public:
    ScopedPointer<DrawableButton> icon;
    ScopedPointer<Drawable> iconDrawable;
    ScopedPointer<SwitchComponent> toggle;
    ScopedPointer<SettingsCategoryButton> button;

    StretchableLayoutManager layout;

    SettingsCategoryItemComponent(const String &name);

    ~SettingsCategoryItemComponent() {
    }

    void paint(Graphics &g) override;
    void resized() override;

    void buttonClicked(Button *b) override;
    void enablementChanged() override;

    virtual void enabledStateChanged(bool enabled) = 0;
    virtual void updateButtonText() = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsCategoryItemComponent)
};