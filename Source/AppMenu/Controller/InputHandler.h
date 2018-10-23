#include "JuceHeader.h"
#include "AppMenu.h"


class AppMenu::InputHandler : public juce::MouseListener,
    public juce::KeyListener
{
private:
    virtual void mouseDown(const juce::MouseEvent& event) final override;
    virtual bool keyPressed(const juce::KeyPress& keyPress,
            juce::Component* sourceComponent) final override;

};
