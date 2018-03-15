/**
 * @file CounterComponent.h
 * 
 * CounterComponent is a UI widget that allows the user to enter
 * or adjust an integer field. The user can directly enter a number
 * into the number field, or increment/decrement its value by pressing
 * the plus and minus buttons next to the number field.
 */
#pragma once

#include "JuceHeader.h"

class CounterComponent : public Component, private Button::Listener,
private TextEditor::Listener {
public:
    /**
     * @param initialValue the intitial value of the counter.
     * @param minimum the lowest value the counter can have.
     * @param maximum the highest value the counter can have.
     * If the maximum is less than the minimum, their values will be
     * swapped.
     */
    CounterComponent(int initialValue = 0,
     int minimum = 0, int maximum = 9);
    
    virtual~CounterComponent() { }
    
    /**
     * @return the integer value held by the counter.
     */
    int getValue();
    
    /**
     * Set the counter's stored integer to a new value.
     * 
     * @param newValue if this value is outside of the counter's
     * permitted range, it will be rounded to the closest
     * valid integer.
     */
    void setValue(int newValue);
    
    /**
     * @param newMin will become the new minimum value, updating
     * the counter value if necessary.  
     * If the maximum is less than the minimum, their values will be
     * swapped.
     */
    void setMinimum(int newMin);
    
    /**
     * @param newMax will become the new maximum value, updating
     * the counter value if necessary.  
     * If the maximum is less than the minimum, their values will be
     * swapped.
     */
    void setMaximum(int newMax);
    
private:
    /**
     * Increment or decrement the counter value when 
     * the plus and minus buttons are pressed.
     * @param button
     */
    void buttonClicked(Button* button) override;
    
    /**
     * Update the value in the counter's TextEditor.
     * This parses the text as an integer, passes it to
     * setValue, and then sets the editor's text to the string
     * representation of the counter's value.
     */
    void updateNumberField();
    
    /**
     * Update the number field when it loses focus.
     */
    void textEditorFocusLost(TextEditor& editor);
    
    /**
     * Update the number field if it's focused and the user presses
     * the return key.
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;
   
    /**
     * Update the number field if it's focused and the user presses
     * the escape key.
     */ 
    void textEditorEscapeKeyPressed(TextEditor& editor) override;
    
    /**
     * Arrange the buttons and number field to fit in the
     * component bounds.
     */
    void resized() override;
    
    //Press to decrement the counter value.
    TextButton minusButton;
    //Press to increment the counter value.
    TextButton plusButton;
    //Display the counter value or directly edit it.
    TextEditor textField;
    //Current counter value
    int count;
    //Smallest permitted counter value
    int minimum;
    //Largest permitted counter value
    int maximum;
};
