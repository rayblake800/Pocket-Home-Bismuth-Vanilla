#include "Utils.h"
#include "ComponentConfigFile.h"
#include "FileSelectTextEditor.h"

const juce::WildcardFileFilter FileSelectTextEditor::imageFilter
("*.png;*.jpg;*.jpeg;*.svg;*.gif;*.xpm", "", "Image files");

FileSelectTextEditor::FileSelectTextEditor(
        const juce::String& selectionTitle,
        const juce::String& selectionText,
        juce::WildcardFileFilter fileFilter,
        const juce::String& componentName) :
Localized("FileSelectTextEditor"),
fileFilter(fileFilter),
selectionTitle(selectionTitle),
selectionText(selectionText),
filePath(componentName),
fileSelectButton("..."),
showButton(true)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("FileSelectTextEditor");
#    endif
    if(selectionTitle.isEmpty())
    {
        this->selectionTitle = localeText(img_select_title);
    }
    if(selectionText.isEmpty())
    {
        this->selectionText = localeText(img_select_text);
        
    }
    fileSelectButton.addListener(this);
    filePath.addListener(this);
    ComponentConfigFile config;
    filePath.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    addAndMakeVisible(filePath);
    addAndMakeVisible(fileSelectButton);
}

/**
 * Set the initial text value for the file selection text editor.
 */
void FileSelectTextEditor::setText(juce::String path, bool shouldNotify)
{
    filePath.setText(path, shouldNotify);
    if (shouldNotify)
    {
        notifyListeners();
    }
}

/**
 * @return the text stored within this component's text editor box.
 */
juce::String FileSelectTextEditor::getText() const
{
    return filePath.getText();
}

/**
 * @param shouldShow Sets the file selection button next to the text editor
 *                    box to be either visible or hidden. By default, the 
 *                    button will be shown.
 */
void FileSelectTextEditor::showFileSelectButton(bool shouldShow)
{
    if (shouldShow == showButton)
    {
        return;
    }

    if (shouldShow)
    {
        addAndMakeVisible(fileSelectButton);
    }
    else
    {
        removeChildComponent(&fileSelectButton);
    }
    showButton = shouldShow;
    resized();
}

/**
 * Adds a Listener to the list of Listeners that will be notified
 * of changes.
 */
void FileSelectTextEditor::addFileSelectListener(Listener * listener)
{
    listeners.insert(listener);
}

/**
 * Signals to all listeners that editor data has updated.
 */
void FileSelectTextEditor::notifyListeners()
{
    for (std::set<Listener*>::iterator it = listeners.begin();
         it != listeners.end(); it++)
    {
        (*it)->fileSelected(this);
    }
}

/**
 * Runs notifyListeners when the editor loses focus.
 */
void FileSelectTextEditor::textEditorFocusLost(juce::TextEditor& editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

/**
 * Runs notifyListeners when the editor is focused and the return key
 * is pressed.
 */
void FileSelectTextEditor::textEditorReturnKeyPressed(juce::TextEditor & editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

/**
 * Opens the file selection window when the file selection button is clicked.
 */
void FileSelectTextEditor::buttonClicked(juce::Button* button)
{
    using namespace juce;
    FileBrowserComponent browser(FileBrowserComponent::canSelectFiles |
            FileBrowserComponent::openMode,
            File(),
            &fileFilter,
            &imagePreview);
    FileChooserDialogBox dialogBox(selectionTitle,
            selectionText,
            browser,
            false,
            findColour(fileWindowColourId));
    dialogBox.setColour(FileChooserDialogBox::titleTextColourId, Colours::red);
    jassert(dialogBox.isColourSpecified(FileChooserDialogBox::titleTextColourId));
    Rectangle<int> size = getWindowBounds();
    if (dialogBox.show(size.getWidth(), size.getHeight()))
    {
        File selectedFile = browser.getSelectedFile(0);
        setText(selectedFile.getFullPathName());
        notifyListeners();
    }
}

/**
 * Resize child components to fit within the parent component.
 */
void FileSelectTextEditor::resized()
{
    using namespace juce;
    Rectangle<int> bounds = getLocalBounds();
    if (!showButton)
    {
        filePath.setBounds(bounds);
        return;
    }
    Rectangle<int> buttonBounds = bounds;
    Rectangle<int> textBounds = bounds;

    buttonBounds.setLeft(bounds.getRight() - bounds.getHeight());
    textBounds.setRight(buttonBounds.getX());

    filePath.setBounds(textBounds);
    fileSelectButton.setBounds(buttonBounds);
}
