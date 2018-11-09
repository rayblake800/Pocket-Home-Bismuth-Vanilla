#pragma once
#include "JuceHeader.h"

/**
 * @file CounterComponent.h
 * 
 * CounterComponent is a UI widget that allows the user to enter
 * or adjust an integer field. The user can directly enter a number
 * into the number field, or increment/decrement its value by pressing
 * the plus and minus buttons next to the number field.
 */

class CounterComponent : public juce::Component, private juce::Button::Listener,
        private juce::TextEditor::Listener
{
public: 
    /**
     * @param initialValue  Sets the initial value of the counter.
     * 
     * @param minimum       Sets the lowest value the counter can have.
     * 
     * @param maximum       Sets the highest value the counter can have.
     *                       If the maximum is less than the minimum, their 
     *                       values will be swapped.
     */
    CounterComponent(int initialValue = 0, int minimum = 0, int maximum = 9);

    virtual ~CounterComponent() { }

    /**
     * @return the integer value held by the counter.
     */
    int getValue();

    /**
     * Sets the counter's stored integer to a new value.
     * 
     * @param newValue  If this value is outside of the counter's permitted 
     *                   range, it will be rounded to the closest valid integer.
     */
    void setValue(int newValue);

    /**
     * Sets a new minimum value, updating the counter value if necessary.
     * 
     * @param newMin   The new minimum value. If the maximum is less than the 
     *                  new minimum, their values will be swapped.
     */
    void setMinimum(int newMin);

    /**
     * Sets a new maximum value, updating the counter value if necessary.
     * 
     * @param newMin   The new maximum value. If the maximum is less than the 
     *                  new minimum, their values will be swapped.
     */
    void setMaximum(int newMax);

private:   
    /**
     * Increments or decrements the counter value when the plus or minus 
     * buttons are pressed.
     * 
     * @param button
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * Updates the value in the counter's TextEditor.  This parses the text as 
     * an integer, passes it to setValue, and then sets the editor's text to the
     * string representation of the counter's value.
     */
    void updateNumberField();

    /**
     * Updates the number field when it loses focus. This should not be called
     * directly.
     * 
     * @param editor
     */
    void textEditorFocusLost(juce::TextEditor& editor);

    /**
     * Updates the number field if it's focused and the user presses the return 
     * key. This should not be called directly.
     * 
     * @param editor
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * Updates the number field if it's focused and the user presses the escape 
     * key. This should not be called directly.
     * 
     * @param editor
     */
    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override;

    /**
     * Arranges the buttons and number field to fit in the component bounds.
     */
    void resized() override;

    //Press to decrement the counter value.
    juce::TextButton minusButton;
    
    //Press to increment the counter value.
    juce::TextButton plusButton;
    
    //Display the counter value or directly edit it.
    juce::TextEditor textField;
    
    //Current counter value
    int count;
    
    //Smallest permitted counter value
    int minimum;
    
    //Largest permitted counter value
    int maximum;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CounterComponent)
};
