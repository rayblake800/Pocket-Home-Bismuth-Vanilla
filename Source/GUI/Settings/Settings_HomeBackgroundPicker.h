#pragma once
/**
 * @file  Settings_HomeBackgroundPicker.h
 *
 * @brief  Provides UI controls for selecting the HomePage background image.
 */

#include "Widgets_FilePathEditor.h"
#include "Theme_Image_ConfigFile.h"

namespace Settings { class HomeBackgroundPicker; }

class Settings::HomeBackgroundPicker : public Widgets::FilePathEditor,
        public Widgets::FilePathEditor::Listener
{
public:
    /**
     * @brief  Loads the saved background image as the editor's initial value.
     */
    HomeBackgroundPicker();

    virtual ~HomeBackgroundPicker() { }

private:
    /**
     * @brief  Updates the selected image when a new file is selected.
     *
     * @param thisEditor  The updated editor. This should always be the
     *                    HomeBackgroundPicker.
     */
    virtual void fileSelected(Widgets::FilePathEditor* thisEditor) override;

    /* Reads and writes the background image, and ensures the image JSON file
     * resource remains available. */
    Theme::Image::ConfigFile imageConfig;
};
