#pragma once

#include "ConfigurableImageButton.h"
#include "FileSelectTextEditor.h"
#include "ScalingLabel.h"
#include "CounterComponent.h"
#include "PageComponent.h"
#include "Utils.h"
#include "Localized.h"

/**
 * @file HomeSettingsPage.h
 * 
 * The HomeSettingsPage allows user configuration of the HomePage, setting the
 * page background, application menu type, and application menu dimensions.
 */


class HomeSettingsPage : public PageComponent, private ComboBox::Listener,
private FileSelectTextEditor::Listener, private Localized
{
public:
    HomeSettingsPage();

    /**
     * Update AppMenu dimensions when the page closes.
     */
    virtual ~HomeSettingsPage();

private:

    /**
     * Initializes the background and AppMenuType combo boxes with values
     * loaded from the MainConfigFile, and updates their labels to match.
     */
    void updateComboBox();
    
    /**
     * If the background type ComboBox is updated, clear the background text
     * field, and update its labels. If the menu type ComboBox is updated,
     * save the changed value to the MainConfigFile
     */
    void comboBoxChanged(ComboBox* box) override;

    /**
     * When a value is set in the background editor, attempt to set a new
     * color or image value for the background, depending on the state of
     * bgTypePicker.
     * 
     * @param edited
     */
    void fileSelected(FileSelectTextEditor* edited);

    //page title label
    ScalingLabel title;
    
    //select the HomePage background type
    ScalingLabel bgTypeLabel;
    ComboBox bgTypePicker;
    
    //enter a hex color or select an image path
    ScalingLabel bgLabel;
    FileSelectTextEditor bgEditor;

    //Choose the AppMenu type to use
    ScalingLabel menuPickerLabel;
    ComboBox menuTypePicker;

    //Set the max column count for AppMenu grids
    ScalingLabel columnCountLabel;
    CounterComponent columnCounter;

    //Set the max row count for AppMenu grids
    ScalingLabel rowCountLabel;
    CounterComponent rowCounter;

    //localized text keys;
    static const constexpr char * title_text = "title_text";
    static const constexpr char * background_text = "background_text";
    static const constexpr char * default_bg = "default_bg";
    static const constexpr char * color_bg = "color_bg";
    static const constexpr char * image_bg = "image_bg";
    static const constexpr char * bg_color_hex_value = "bg_color_hex_value";
    static const constexpr char * bg_image_path = "bg_image_path";
    static const constexpr char * invalid_color = "invalid_color";
    static const constexpr char * choose_background = "choose_background";
    static const constexpr char * choose_bg_image = "choose_bg_image";
    static const constexpr char * menu_type_text = "menu_type_text";
    static const constexpr char * menu_columns = "menu_columns";
    static const constexpr char * menu_rows = "menu_rows";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
