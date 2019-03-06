#pragma once
/**
 * @file  Widgets_TextEditor.h
 *
 * @brief  A juce::TextEditor that automatically scales its font size to match
 *         its current height.
 */

#include "JuceHeader.h"

namespace Widgets { class TextEditor; }

class Widgets::TextEditor : public juce::TextEditor
{
public:
    using juce::TextEditor::TextEditor;

    virtual ~TextEditor() { }

private:
    /**
     * @brief  Updates the font height, and immediately applies the change to
     *         the edited text.
     *
     * @param newHeight  The new font height to apply.
     */
    void setFontHeight(const int newHeight);

    /**
     * @brief  Update internal text size to match changes to the component's
     *         height.
     */
    virtual void resized() override;
};
