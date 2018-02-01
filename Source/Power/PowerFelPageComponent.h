/**
 * @File PowerFelPageComponent.h
 * 
 * PowerFelPageComponent gives the user the option to either reboot into Fel
 * (flashing) mode, or cancel and return to the last page.  If the system is
 * not a PocketCHIP or the i2c bus is otherwise inaccessible, nothing will
 * happen if the user clicks the confirmation button.
 */

#pragma once
#include "../Basic Components/ScalingLabel.h"
#include "../GridLayoutManager.h"

class PowerFelPageComponent : public Component, private Button::Listener {
public:
    
    PowerFelPageComponent();
    ~PowerFelPageComponent();
private:
    //Scale and position child components to fit the page.
    GridLayoutManager layoutManager;
    //Ask for confirmation
    ScalingLabel infoLine1;
    //Tell the user where to find flashing instructions.
    ScalingLabel infoLine2;
    TextButton yesButton;//Confirm, and reboot into Fel mode
    TextButton noButton;//Cancel, and close this page


    bool debounce;
    void paint(Graphics &g) override;
    void resized() override;
    void buttonStateChanged(Button*) override;
    void buttonClicked(Button*) override;
    void setSleep();
    
    Colour bgColor;
    ChildProcess child;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerFelPageComponent)
};
