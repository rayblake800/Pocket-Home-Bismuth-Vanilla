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
#include "Page_Component.h"
#include "AppMenu_SettingsComponents.h"
#include "Utils.h"
#include "Locale/TextUser.h"

/**
 * @brief  The page component used to hold all HomePage and AppMenu settings
 *         control components.
 */
class HomeSettingsPage : public Page::Component, public Locale::TextUser,
    private juce::ComboBox::Listener, private FileSelectTextEditor::Listener
{
public:
    /**
     * @brief  Initializes the page layout.
     */
    HomeSettingsPage();

    /**
     * @brief  Updates AppMenu settings when the page closes.
     */
    virtual ~HomeSettingsPage();

private:
    /**
     * @brief  Initializes the background combo box with values loaded from 
     *         the main config file, and updates its labels to match.
     */
    void updateComboBox();
    
    /**
     * @brief  Handles background ComboBox selections.
     *
     * If the background type ComboBox is updated, clear the background text
     * field, and update its labels.
     */
    void comboBoxChanged(juce::ComboBox* box) override;

    /**
     * @brief  Attempts to set a new color or image value for the background 
     *         when the background editor is updated, depending on the state of
     *         bgTypePicker.
     * 
     * @param edited  The bgEditor component.
     */
    virtual void fileSelected(FileSelectTextEditor* edited) override;

    /* Page title label: */
    ScalingLabel title;
    
    /* Used to select the HomePage background type: */
    ScalingLabel bgTypeLabel;
    juce::ComboBox bgTypePicker;
    
    /* Used to set the HomePage's background color or image: */
    ScalingLabel bgLabel;
    FileSelectTextEditor bgEditor;

    /* Used to select AppMenu format settings: */
    AppMenu::SettingsComponents menuComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
