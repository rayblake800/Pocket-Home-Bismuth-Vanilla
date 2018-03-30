/**
 * @file ColourPicker.h
 * 
 * @brief A UI component that allows the user to select a color value.
 */
#pragma once
#include "RelativeLayoutManager.h"
#include "Localized.h"
#include "JuceHeader.h"

class ColourPicker : public Component, private  Slider::Listener,
private TextEditor::Listener, private Button::Listener, private Localized
{
public:
    /**
     * @param numSavedColours  Sets the number of quick colour selection buttons
     *                         to show.
     *
     * @param colour           Sets the initial colour value to show in the
     *                         ColourPicker.
     */
    ColourPicker(int numSavedColours = 5, Colour colour = Colours::white);

    virtual ~ColourPicker() { }

    /**
     * Gets the current selected colour.
     */
    Colour getSelectedColour();

    /**
     * Sets a new colour value.
     *
     * @param colour
     *
     * @param runCallback  Sets if the colour selection callback should
     *                     be called for this xolour change.
     */
    void setSelectedColour(Colour colour, bool runCallback = false);

    /**
     * Assigns a callback function to run when a colour is selected.
     *
     * @param callback  The function to run. The new colour will be passed in
     *                  as a parameter.
     */
    void setSelectionCallback(std::function<void(Colour)> callback);

protected:
    /**
     * Apply the current colour selection to the sliders.
     */
    void setSlidersToColour();

    /**
     * Set the text field to the current color's 8 digit hex value.
     */
    void setEditorText();

    /**
     * If the current colour selection isn't one of the preset colour buttons,
     * add it to the list and remove the oldest value from the list.
     */
    void updateColourButtons();

    /**
     * Updates the color preview and text box when slider values change.
     */
    void sliderValueChanged(Slider* slider) override;

    /**
     * Updates the sliders and colour preview after the text box value changes.
     */
    void textEditorFocusLost(TextEditor& editor) override;

    /**
     * Updates the sliders and colour preview after the text box value changes.
     */
    void textEditorReturnKeyPressed(TextEditor& editor) override;

    /**
     * If a colour thumbnail is clicked, apply it to the sliders, text box,
     * and preview.  If the apply button is clicked, add the selection colour
     * to the preview thumbnails and run the selection callback.
     */
    void buttonClicked(Button* button) override;

    /**
     * Re-apply component layout using the layoutManager,
     */
    void resized() override;

    /**
     * A square component that shows a colour value.
     */
    class ColourBox: public Component
    {
    public: 
        ColourBox(Colour colour = Colour(), bool drawBorder = true);
		
	virtual ~ColourBox() { };

	Colour getColour();

	void setColour(Colour colour);

    private:
	void paint(Graphics& g) override;
		
	Colour colour;
	bool drawBorder;
    };

    /**
     * A ColourBox button, used for re-selecting previous selected colours.
     */
    class ColourButton : public Button
    {
    public:
	ColourButton(Colour colour = Colour());

	virtual ~ColourButton() { }

	Colour getColour();

	void setColour(Colour colour);
	
    private:
	/**
	 * Change the outline color when the button is down.
	 */
	void paintButton
	(Graphics &g, bool isMouseOverButton, bool isButtonDown);	

	void resized() override;

	ColourBox colourBox;
    };
private:

    Colour colour;

    //Handles child component layout
    RelativeLayoutManager layoutManager;
    
    //Shows the current color selection over a grid background.
    ColourBox colourPreview;

    //Holds the buttons used to re-select previously selected colors.
    OwnedArray<ColourButton> colourButtons;
    //Shares the last selected colours between all colour pickers.
    static Array<Colour> savedColours;
    //Sets how many saved colours are shown.
    int numSavedColours = 0;

    //Runs when the user selects a color.
    std::function<void(Colour)> selectionCallback;

    //Red, green, blue, and alpha color component sliders.
    Slider rSlider;
    Slider gSlider;
    Slider bSlider;
    Slider aSlider;

    //Holds the colour value as an eight digit hex number string.
    TextEditor colourField;

    //Clicking this confirms the color selection.
    TextButton selectionButton;

    static const constexpr char * set_colour = "set_colour"; 

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourPicker)
};
