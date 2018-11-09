#include "Utils.h"
#include "ComponentConfigFile.h"
#include "FileSelectTextEditor.h"

/* Default file filter, allowing image types: */
const juce::WildcardFileFilter FileSelectTextEditor::imageFilter
("*.png;*.jpg;*.jpeg;*.svg;*.gif;*.xpm", "", "Image files");

/* Class localized text key: */
static const juce::Identifier localeClassKey = "FileSelectTextEditor";

/* Localized text value keys: */
static const juce::Identifier defaultTitleKey       = "defaultTitle";
static const juce::Identifier defaultDescriptionKey = "defaultDescription";

/*
 * Creates a new FileSelectTextEditor, optionally setting its initial 
 * properties.
 */
FileSelectTextEditor::FileSelectTextEditor(
        const juce::String& selectionTitle,
        const juce::String& selectionText,
        const juce::WildcardFileFilter fileFilter) :
Locale::TextUser(localeClassKey),
fileFilter(fileFilter),
selectionTitle(selectionTitle),
selectionText(selectionText),
fileSelectButton("...")
{
#if JUCE_DEBUG
    setName("FileSelectTextEditor");
#endif
    if(selectionTitle.isEmpty())
    {
        this->selectionTitle = localeText(defaultTitleKey);
    }
    if(selectionText.isEmpty())
    {
        this->selectionText = localeText(defaultDescriptionKey);
    }
    fileSelectButton.addListener(this);
    filePath.addListener(this);
    ComponentConfigFile config;
    filePath.setFont(juce::Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    addAndMakeVisible(filePath);
    addAndMakeVisible(fileSelectButton);
}

/*
 * Sets the text value held by this editor.
 */
void FileSelectTextEditor::setText
(const juce::String newText, const bool shouldNotify)
{
    filePath.setText(newText, shouldNotify);
    if (shouldNotify)
    {
        notifyListeners();
    }
}

/*
 * Gets the text value held by this editor.
 */
juce::String FileSelectTextEditor::getText() const
{
    return filePath.getText();
}

/*
 * Sets the visibility of the editor's file selection button.
 */
void FileSelectTextEditor::showFileSelectButton(const bool shouldShow)
{
    if(shouldShow == fileSelectButton.isShowing())
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
        fileSelectButton.setVisible(false);
    }
    resized();
}

/*
 * Adds a Listener to the list of Listeners that will be notified of changes.
 */
void FileSelectTextEditor::addFileSelectListener(Listener * listener)
{
    listeners.insert(listener);
}

/*
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

/*
 * Calls notifyListeners when the editor loses focus.
 */
void FileSelectTextEditor::textEditorFocusLost(juce::TextEditor& editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

/*
 * Calls notifyListeners when the editor is focused and the return key is 
 * pressed.
 */
void FileSelectTextEditor::textEditorReturnKeyPressed(juce::TextEditor & editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

/*
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
    if (!fileSelectButton.isVisible())
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
