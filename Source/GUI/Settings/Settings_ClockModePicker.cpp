#include "Settings_ClockModePicker.h"
#include "Config_MainFile.h"

// Localized text class key:
static const juce::Identifier localeClassKey = "Settings::ClockModePicker";

// Localized text string keys:
namespace TextKey
{
    static const constexpr char* hideClock  = "hideClock";
    static const constexpr char* mode12Hour = "mode12Hour";
    static const constexpr char* mode24Hour = "mode24Hour";
}


// Combo box option IDs:
namespace ComboBoxID
{
    static const constexpr int hideClock  = 1;
    static const constexpr int mode12Hour = 2;
    static const constexpr int mode24Hour = 3;
}


// Initializes the component with the current saved clock mode.
Settings::ClockModePicker::ClockModePicker() : Locale::TextUser(localeClassKey)
{
    addItem(localeText(TextKey::hideClock),  ComboBoxID::hideClock);
    addItem(localeText(TextKey::mode12Hour), ComboBoxID::mode12Hour);
    addItem(localeText(TextKey::mode24Hour), ComboBoxID::mode24Hour);
    Config::MainFile mainConfig;
    int initialID = ComboBoxID::hideClock;
    if (mainConfig.getShowClock())
    {
        initialID = mainConfig.get24HourEnabled()
                ? ComboBoxID::mode24Hour : ComboBoxID::mode12Hour;
    }
    setSelectedId(initialID, juce::NotificationType::dontSendNotification);
    addListener(this);
}


// Updates the clock mode when the user selects a new mode.
void Settings::ClockModePicker::comboBoxChanged(juce::ComboBox* comboBox)
{
    Config::MainFile mainConfig;
    const int selectedId = getSelectedId();
    if (selectedId == ComboBoxID::hideClock)
    {
        mainConfig.setShowClock(false);
    }
    else
    {
        mainConfig.setShowClock(true);
        mainConfig.set24HourEnabled(selectedId == ComboBoxID::mode24Hour);
    }
}
