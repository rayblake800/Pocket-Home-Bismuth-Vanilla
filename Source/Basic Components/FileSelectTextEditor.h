/**
 * @file FileSelectTextEditor.h
 * @author Anthony Brown
 * 
 * This component allows a user to either type in a text value, or click a
 * button to open a file picker and choose a file/directory path.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class FileSelectTextEditor : public Component, public Button::Listener {
public:
    FileSelectTextEditor(const String& selectionTitle = imgSelectTitle,
            const String& selectionText = imgSelectText,
            WildcardFileFilter fileFilter = imageFilter,
            const String& componentName = String("fileSelectTextEditor"));
    virtual ~FileSelectTextEditor();

    void setText(String path);
    String getText();

    void showFileSelectButton(bool shouldShow);
private:
    void buttonClicked(Button* button) override;
    void resized() override;


    TextEditor filePath;
    TextButton fileSelectButton;
    bool showButton;
    WildcardFileFilter fileFilter;
    String selectionTitle;
    String selectionText;
    ImagePreviewComponent imagePreview;

    //Default selection properties: image selection
    static const WildcardFileFilter imageFilter;
    static const String imgSelectTitle;
    static const String imgSelectText;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileSelectTextEditor)
};