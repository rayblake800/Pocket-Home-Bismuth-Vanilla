/**
 * @file FileSelectTextEditor.h
 * @author Anthony Brown
 * 
 * This component allows a user to either type in a text value, or click a
 * button to open a file picker and choose a file/directory path.
 */

#pragma once
#include <set>
#include "../../JuceLibraryCode/JuceHeader.h"

class FileSelectTextEditor : public Component,
public TextEditor::Listener,
public Button::Listener {
public:
    FileSelectTextEditor(const String& selectionTitle = imgSelectTitle,
            const String& selectionText = imgSelectText,
            WildcardFileFilter fileFilter = imageFilter,
            const String& componentName = String("fileSelectTextEditor"));
    virtual ~FileSelectTextEditor();

    void setText(String path);
    String getText();

    void showFileSelectButton(bool shouldShow);

    class Listener {
    public:
        Listener();
        virtual ~Listener();
        //Receives alerts whenever a file is selected or focus is lost.
        virtual void fileSelected(FileSelectTextEditor* edited) = 0;
    };

    void addFileSelectListener(Listener * listener);
private:
    void notifyListeners();
    void textEditorFocusLost(TextEditor& editor) override;
    void textEditorReturnKeyPressed(TextEditor & editor) override;
    void buttonClicked(Button* button) override;
    void resized() override;


    TextEditor filePath;
    TextButton fileSelectButton;
    bool showButton;
    WildcardFileFilter fileFilter;
    String selectionTitle;
    String selectionText;
    std::set<Listener*> listeners;
    ImagePreviewComponent imagePreview;

    //Default selection properties: image selection
    static const WildcardFileFilter imageFilter;
    static const String imgSelectTitle;
    static const String imgSelectText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileSelectTextEditor)
};