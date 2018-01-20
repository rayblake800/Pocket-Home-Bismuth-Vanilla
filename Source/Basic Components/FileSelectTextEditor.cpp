/*
  ==============================================================================

    FileSelectTextEditor.cpp
    Created: 12 Jan 2018 9:50:15pm
    Author:  anthony

  ==============================================================================
 */
#include "../Utils.h"
#include "FileSelectTextEditor.h"

const WildcardFileFilter FileSelectTextEditor::imageFilter
("*.png;*.jpg;*.jpeg;*.svg;*.gif;*.xpm", "", "Image files");

const String FileSelectTextEditor::imgSelectTitle =
        "Select an image file";
const String FileSelectTextEditor::imgSelectText =
        "Please select an image file";

FileSelectTextEditor::FileSelectTextEditor(const String& selectionTitle,
        const String& selectionText,
        WildcardFileFilter fileFilter,
        const String& componentName) :
fileFilter(fileFilter),
selectionTitle(selectionTitle),
selectionText(selectionText),
filePath(componentName),
fileSelectButton("..."),
showButton(true)
{
    fileSelectButton.addListener(this);
    filePath.addListener(this);
    addAndMakeVisible(filePath);
    addAndMakeVisible(fileSelectButton);
}

FileSelectTextEditor::~FileSelectTextEditor()
{
}

void FileSelectTextEditor::setText(String path)
{
    filePath.setText(path, true);
    notifyListeners();
}

String FileSelectTextEditor::getText()
{
    return filePath.getText();
}

void FileSelectTextEditor::showFileSelectButton(bool shouldShow)
{
    if (shouldShow == showButton)
    {
        return;
    }

    if (shouldShow)
    {
        addAndMakeVisible(fileSelectButton);
    } else
    {
        removeChildComponent(&fileSelectButton);
    }
    showButton = shouldShow;
    resized();
}

FileSelectTextEditor::Listener::Listener()
{
}

FileSelectTextEditor::Listener::~Listener()
{
}

void FileSelectTextEditor::addFileSelectListener(Listener * listener)
{
    listeners.insert(listener);
}

void FileSelectTextEditor::notifyListeners()
{
    for (std::set<Listener*>::iterator it = listeners.begin();
            it != listeners.end(); it++)
    {
        (*it)->fileSelected(this);
    }
}

void FileSelectTextEditor::textEditorFocusLost(TextEditor& editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

void FileSelectTextEditor::textEditorReturnKeyPressed(TextEditor & editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}

void FileSelectTextEditor::buttonClicked(Button* button)
{
    FileBrowserComponent browser(FileBrowserComponent::canSelectFiles |
            FileBrowserComponent::openMode,
            File::nonexistent,
            &fileFilter,
            &imagePreview);
    FileChooserDialogBox dialogBox(selectionTitle,
            selectionText,
            browser,
            false,
            Colours::lightgrey);
    Rectangle<int> size = getWindowSize();
    if (dialogBox.show(size.getWidth(), size.getHeight()))
    {
        File selectedFile = browser.getSelectedFile(0);
        setText(selectedFile.getFullPathName());
        notifyListeners();
    }
}

void FileSelectTextEditor::resized()
{
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
