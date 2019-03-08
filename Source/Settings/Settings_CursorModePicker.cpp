#include "Settings_CursorModePicker.h"
#include "Config_MainFile.h"

/* Localized text class key: */
static const juce::Identifier localeClassKey = "Settings::CursorModePicker";

/* Localized text string keys: */
namespace TextKey
{
    static const constexpr char* visible    = "visible";
    static const constexpr char* notVisible = "notVisible";
}

/* Combo box option IDs: */
namespace ComboBoxID
{
    static const constexpr int visible    = 1;
    static const constexpr int notVisible = 2;
}
/*
 * Initializes the component with the current cursor visibility setting.
 */
Settings::CursorModePicker::CursorModePicker() : 
Locale::TextUser(localeClassKey)
{
    addItem(localeText(TextKey::visible), ComboBoxID::visible);
    addItem(localeText(TextKey::notVisible), ComboBoxID::notVisible);
    Config::MainFile mainConfig;
    setSelectedId(mainConfig.getShowCursor() 
            ? ComboBoxID::visible : ComboBoxID::notVisible);
    addListener(this);
}

/*
 * Updates the cursor visibility mode when the user selects a new mode.
 */
void Settings::CursorModePicker::comboBoxChanged(juce::ComboBox* comboBox)
{
    Config::MainFile mainConfig;
    mainConfig.setShowCursor(getSelectedId() == ComboBoxID::visible);
}
