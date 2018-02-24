/**
 * @file ConnectionSettingsComponent.h
 * 
 * TODO: documentation
 */
#pragma once

#include "../../PageComponent.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageComponent.h"
#include "../../../JuceLibraryCode/JuceHeader.h"

class ConnectionSettingsComponent : public Component,
private Button::Listener {
public:
    ConnectionSettingsComponent(std::function<void()> openConnectionPage,
            const String& name = String());

    virtual ~ConnectionSettingsComponent();

    virtual void enabledStateChanged(bool enabled) = 0;

    virtual void updateButtonText() = 0;

protected:
    void setIcon(String iconAsset);
    void setIconVisible(bool visible);
    void setToggleState(bool toggleState);
    void setPageButtonEnabled(bool isEnabled);
    void setPageButtonText(String text);
    void resized() override;
private:

    void buttonClicked(Button *b) override;

    void enablementChanged() override;

    class ConnectionButton : public Button {
    public:
        ConnectionButton(const String &name = String());
        virtual ~ConnectionButton();
        void setText(const String &text);
    private:
        void paintButton
        (Graphics &g, bool isMouseOverButton, bool isButtonDown) override;
        void resized() override;

        String displayText;
        Rectangle<int> pillBounds;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionButton)
    };

    DrawableImageComponent icon;
    SwitchComponent toggle;
    ConnectionButton pageButton;
    StretchableLayoutManager layout;
    std::function<void()> openConnectionPage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionSettingsComponent)
};