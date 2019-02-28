#include "Theme_Image_HomeBackgroundPicker.h"

/*
 * Loads the saved background image as the editor's initial value.
 */
Theme::Image::HomeBackgroundPicker::HomeBackgroundPicker()
{
    setText(imageConfig.getHomeBackgroundPath(), false);
    addFileSelectListener(this);
}

/*
 * Updates the selected image when a new file is selected.
 */
void Theme::Image::HomeBackgroundPicker::fileSelected
(FileSelectTextEditor* thisEditor)
{
    jassert(thisEditor == this);
    imageConfig.setHomeBackgroundPath(getText());
}
