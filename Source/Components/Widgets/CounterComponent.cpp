#include "Utils.h"
#include "CounterComponent.h"

CounterComponent::CounterComponent
(int initialValue, int minimum, int maximum) :
count(initialValue),
minimum(minimum),
maximum(maximum),
minusButton("-"),
plusButton("+")
{
    minusButton.addListener(this);
    plusButton.addListener(this);
    textField.addListener(this);
    textField.setJustification(Justification::centred);
    textField.setInputFilter
            (new TextEditor::LengthAndCharacterRestriction(-1, "1234567890"),
            true);
    addAndMakeVisible(minusButton);
    addAndMakeVisible(textField);
    addAndMakeVisible(plusButton);
    setValue(initialValue);
    textField.setText(String(count));
}

/**
 * @return the integer value held by the counter.
 */
int CounterComponent::getValue()
{
    return count;
}

/**
 * Sets the counter's stored integer to a new value.
 */
void CounterComponent::setValue(int newValue)
{
    int value = median<int>(minimum, newValue, maximum);
    if (value != count)
    {
        count = value;
        textField.setText(String(count));
    }
}

/**
 * Sets a new minimum value, updating the counter value if necessary.
 */
void CounterComponent::setMinimum(int newMin)
{
    minimum = newMin;
    setValue(count);
}

/**
 * Sets a new maximum value, updating the counter value if necessary.
 */
void CounterComponent::setMaximum(int newMax)
{
    maximum = newMax;
    setValue(count);
}

/**
 * Increments or decrements the counter value when the plus or minus 
 * buttons are pressed.
 */
void CounterComponent::buttonClicked(Button* button)
{
    if (button == &minusButton)
    {
        setValue(count - 1);
    }
    else if (button == &plusButton)
    {
        setValue(count + 1);
    }
}

/**
 * Updates the value in the counter's TextEditor.  This parses the text as 
 * an integer, passes it to setValue, and then sets the editor's text to the
 * string representation of the counter's value.
 */
void CounterComponent::updateNumberField()
{
    setValue(textField.getText().getIntValue());
}

/**
 * Updates the number field when it loses focus.
 */
void CounterComponent::textEditorFocusLost(TextEditor& editor)
{
    updateNumberField();
}

/**
 * Updates the number field if it's focused and the user presses the return key.
 */
void CounterComponent::textEditorReturnKeyPressed(TextEditor& editor)
{
    updateNumberField();
}

/**
 * Update the number field if it's focused and the user presses the escape key.
 */
void CounterComponent::textEditorEscapeKeyPressed(TextEditor& editor)
{
    updateNumberField();
}

/**
 * Arranges the buttons and number field to fit in the component bounds.
 */
void CounterComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    if (bounds.isEmpty())
    {
        return;
    }
    if (bounds.getWidth() < bounds.getHeight()*3)
    {
        bounds = bounds.withSizeKeepingCentre(bounds.getWidth(),
                bounds.getWidth() / 3);
    }
    int btnSize = bounds.getWidth() / 3;
    Rectangle<int> minusBounds = bounds.withWidth(btnSize);
    Rectangle<int> plusBounds = minusBounds.withX
            (bounds.getWidth() - btnSize);
    bounds.setX(bounds.getX() + btnSize);
    bounds.setWidth(bounds.getWidth() - btnSize * 2);
    minusButton.setBounds(minusBounds);
    plusButton.setBounds(plusBounds);
    textField.setBounds(bounds);
}
