/**
 * @file HomeSettingsPage.h
 * 
 * The HomeSettingsPage allows user configuration of the HomePage, setting the
 * page background, application menu type, and application menu dimensions.
 */

#pragma once

#include "ConfigurableImageButton.h"
#include "FileSelectTextEditor.h"
#include "ScalingLabel.h"
#include "CounterComponent.h"
#include "PageComponent.h"
#include "Utils.h"
#include "ColourPage.h"
#include "MainConfigFile.h"

class HomeSettingsPage : public PageComponent, private ComboBox::Listener,
private FileSelectTextEditor::Listener
{
public:
    HomeSettingsPage(PageFactoryInterface& pageFactory, 
            MainConfigFile& config);

    ~HomeSettingsPage() { }

private:
    /**
     * Update AppMenu dimensions when the page closes.
     */
    void pageRemovedFromStack() override;

    void comboBoxChanged(ComboBox*) override;

    void pageButtonClicked(Button* button) override;


    void fileSelected(FileSelectTextEditor* edited);

    void updateComboBox();
    MainConfigFile& config;
    
    ScalingLabel title;
    Colour bgColor;
    ScalingLabel bgTitle;
    ScalingLabel bgLabel;
    ComboBox bgTypePicker;
    FileSelectTextEditor bgEditor;

    ScalingLabel menuPickerLabel;
    ComboBox menuTypePicker;

    ScalingLabel columnCountLabel;
    CounterComponent columnCounter;

    ScalingLabel rowCountLabel;
    CounterComponent rowCounter;
    TextButton colourPageBtn;
    ColourPage colourPage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
