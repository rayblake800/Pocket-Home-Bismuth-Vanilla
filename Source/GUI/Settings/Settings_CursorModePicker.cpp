#include "Settings_CursorModePicker.h"
#include "Config_MainFile.h"


// Initializes the component with the current cursor visibility setting.
Settings::CursorModePicker::CursorModePicker()
{
    Config::MainFile mainConfig;
    setToggleState(mainConfig.getShowCursor());
    addListener(this);
}


// Updates the cursor visibility mode when the user selects a new mode.
void Settings::CursorModePicker::buttonClicked(juce::Button* cursorSwitch)
{
    Config::MainFile mainConfig;
    mainConfig.setShowCursor(getToggleState());
}
