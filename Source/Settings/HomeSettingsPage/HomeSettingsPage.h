#pragma once
/**
 * @file  HomeSettingsPage.h
 * 
 * @brief  Allows the user to configure the HomePage, setting the page 
 *         background, the application menu type, and the application menu 
 *         dimensions.
 */

#include "FileSelectTextEditor.h"
#include "ScalingLabel.h"
#include "CounterComponent.h"
#include "Page_Component.h"
#include "Utils.h"
#include "Locale/TextUser.h"

class HomeSettingsPage : public Page::Component, public Locale::TextUser,
    private juce::ComboBox::Listener, private FileSelectTextEditor::Listener
{
public:
    HomeSettingsPage();

    /**
     * @brief  Updates AppMenu dimensions when the page closes.
     */
    virtual ~HomeSettingsPage();

private:
    /**
     * @brief  Initializes the background and AppMenuType combo boxes with 
     *         values loaded from Config resources and updates their labels to 
     *         match.
     */
    void updateComboBox();

    /**
     * @brief  Updates the menu row counter, and shows or hides the menu column
     *         counter based on the selected menu format and saved menu
     *         dimensions.
     */
    void updateMenuCounters();
    
    /**
     * @brief  Handles ComboBox selections.
     *
     * If the background type ComboBox is updated, clear the background text
     * field, and update its labels. If the menu type ComboBox is updated,
     * save the changed value to the MainConfigFile
     */
    void comboBoxChanged(juce::ComboBox* box) override;

    /**
     * @brief  Atttempt to set a new color or image value for the background 
     *         when the background editor is updated, depending on the state of
     *         bgTypePicker.
     * 
     * @param edited  The bgEditor component.
     */
    virtual void fileSelected(FileSelectTextEditor* edited) override;

    //page title label
    ScalingLabel title;
    
    //select the HomePage background type
    ScalingLabel bgTypeLabel;
    juce::ComboBox bgTypePicker;
    
    //enter a hex color or select an image path
    ScalingLabel bgLabel;
    FileSelectTextEditor bgEditor;

    //Choose the AppMenu type to use
    ScalingLabel menuPickerLabel;
    juce::ComboBox menuTypePicker;

    //Set the max column count for AppMenu grids
    ScalingLabel columnCountLabel;
    CounterComponent columnCounter;

    //Set the max row count for AppMenu grids
    ScalingLabel rowCountLabel;
    CounterComponent rowCounter;
    
    // Don't forget to update these if adding/removing/re-ordering menu types!
    // Keep in the same order used in MainConfigFile::menuTypes
    static const constexpr char * scrolling_menu = "scrolling_menu";
    static const constexpr char * paged_menu = "paged_menu";
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
