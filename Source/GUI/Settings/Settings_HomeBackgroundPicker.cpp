#include "Settings_HomeBackgroundPicker.h"

// Loads the saved background image as the editor's initial value.
Settings::HomeBackgroundPicker::HomeBackgroundPicker()
{
    setText(imageConfig.getHomeBackgroundPath(), false);
    addFileSelectListener(this);
}


// Updates the selected image when a new file is selected.
void Settings::HomeBackgroundPicker::fileSelected
(Widgets::FilePathEditor* thisEditor)
{
    jassert(thisEditor == this);
    imageConfig.setHomeBackgroundPath(getText());
}
