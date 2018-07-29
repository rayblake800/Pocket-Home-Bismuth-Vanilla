#pragma once
#include <set>
#include "Localized.h"
#include "JuceHeader.h"

/**
 * @file FileSelectTextEditor.h
 * 
 * @brief A file selection UI component.
 * 
 * This component allows a user to either type in a text value, or click a
 * button to open a file picker and choose a file/directory path.
 */

class FileSelectTextEditor : public juce::Component,
public juce::TextEditor::Listener, public juce::Button::Listener,
private Localized
{
public:

    enum ColourIds
    {
        fileWindowColourId = 0x1900300,
        textColourId = 0x1900301
    };
    
    /**
     * @param selectionTitle Sets the title displayed in the file selection 
     *                        window.  If left empty, this will use the default
     *                        image selection title.
     * 
     * @param selectionText  Sets the short description displayed in the file
     *                        selection window.  If left empty, this will use
     *                        the default image selection text.
     * 
     * @param fileFilter     Used to set what types of files may be selected.
     * 
     * @param componentName  Sets the component name value
     */
    FileSelectTextEditor(
            const juce::String& selectionTitle = juce::String(),
            const juce::String& selectionText = juce::String(),
            juce::WildcardFileFilter fileFilter = imageFilter,
            const juce::String& componentName 
            = juce::String("fileSelectTextEditor"));
    
    virtual ~FileSelectTextEditor() { }

    /**
     * Set the initial text value for the file selection text editor.
     * 
     * @param path          The initial text value. The usual purpose of this 
     *                       is to set the path of the last selected file, but 
     *                       any string will be allowed.
     * 
     * @param shouldNotify  Sets whether FileSelectTextEditor::Listener objects
     *                       will be notified of this change.
     */
    void setText(juce::String path, bool shouldNotify = true);

    /**
     * @return the text stored within this component's text editor box.
     */
    juce::String getText();

    /**
     * @param shouldShow Sets the file selection button next to the text editor
     *                    box to be either visible or hidden. By default, the 
     *                    button will be shown.
     */
    void showFileSelectButton(bool shouldShow);

    /**
     * Classes implementing listener can receive updates whenever this 
     * button's text changes.
     */
    class Listener
    {
    public:
        
        Listener() { }
        
        virtual ~Listener() { }
        
        /**
         * This will be called whenever a file is selected, the return 
         * key is pressed while editing, or focus is lost.
         * 
         * @param edited   Points to the editor that changed. 
         */
        virtual void fileSelected(FileSelectTextEditor* edited) = 0;
    };
    
    /**
     * Adds a Listener to the list of Listeners that will be notified
     * of changes.
     * 
     * @param listener  An object that needs to track the contents of this
     *                   editor.
     */
    void addFileSelectListener(Listener * listener);
    
private:
    
    /**
     * Signals to all listeners that editor data has updated.
     */
    void notifyListeners();
    
    /**
     * Runs notifyListeners when the editor loses focus.
     * 
     * @param editor this component's internal TextEditor object.
     */
    void textEditorFocusLost(juce::TextEditor& editor) override;

    /**
     * Runs notifyListeners when the editor is focused and the return key
     * is pressed.
     * 
     * @param editor this component's internal TextEditor object.
     */
    void textEditorReturnKeyPressed(juce::TextEditor & editor) override;

    /**
     * Opens the file selection window when the file selection button is 
     * clicked.
     * 
     * @param button Should be this component's file selection button.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * Resize child components to fit within the parent component.
     */
    void resized() override;

    //Editable text field, shows the selected path, or whatever other value
    //the user types in
    juce::TextEditor filePath;
    
    //Opens the file selection window.
    juce::TextButton fileSelectButton;
    
    //iff true, display fileSelectButton
    bool showButton;
    
    //Sets which files the file selection window will allow.
    juce::WildcardFileFilter fileFilter;
    
    //Title to display on the file selection window.
    juce::String selectionTitle;
    
    //Brief description to display on the file selection window.
    juce::String selectionText;
    
    //Stores all objects that track when this editor's value changes.
    std::set<Listener*> listeners;
    
    //Image preview component to use on the file selection window.
    juce::ImagePreviewComponent imagePreview;

    //Default selection properties for selecting images.
    static const juce::WildcardFileFilter imageFilter;
    static const constexpr char * img_select_title = "img_select_title";
    static const constexpr char * img_select_text = "img_select_text";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileSelectTextEditor)
};
