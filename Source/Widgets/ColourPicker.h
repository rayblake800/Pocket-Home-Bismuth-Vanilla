#pragma once
/**
 * @file ColourPicker.h
 * 
 * @brief A UI component that allows the user to select a color value.
 */

#include "Layout_Group_Manager.h"
#include "Locale/TextUser.h"
#include "JuceHeader.h"

class ColourPicker : public juce::Component, public Locale::TextUser,
        private juce::Slider::Listener,
        private juce::TextEditor::Listener,
        private juce::Button::Listener
{
public:
    /**
     * @param numSavedColours  Sets the number of quick colour selection buttons
     *                         to show.
     *
     * @param colour           Sets the initial colour value to show in the
     *                         ColourPicker.
     */
    ColourPicker(int numSavedColours = 5, 
            juce::Colour colour = juce::Colours::white);

    virtual ~ColourPicker() { }
    
    enum ColourIds
    {
        //Light area of the checkerboard pattern drawn behind color previews.
        checkerboardLight = 0x1900600,
        //Dark area of the checkerboard pattern drawn behind color previews.
        checkerboardDark  = 0x1900601,
        //Outline color for slider backgrounds and color previews.
        outline           = 0x1900602,
        //Outline color used on saved color buttons when held down.
        focusedOutline    = 0x1900603
    };

    /**
     * Gets the current selected colour.
     */
    juce::Colour getSelectedColour();

    /**
     * @brief  Sets a new colour value.
     *
     * @param colour
     *
     * @param runCallback  Sets if the colour selection callback should
     *                     be called for this xolour change.
     */
    void setSelectedColour(juce::Colour colour, bool runCallback = false);

    /**
     * @brief  Assigns a callback function to run when a colour is selected.
     *
     * @param callback  The function to run. The new colour will be passed in
     *                  as a parameter.
     */
    void setSelectionCallback(std::function<void(juce::Colour)> callback);

private:
    /**
     * @brief  Apply the current colour selection to the sliders.
     */
    void setSlidersToColour();

    /**
     * @brief  Set the text field to the current color's 8 digit hex value.
     */
    void setEditorText();

    /**
     * @brief  If the current colour selection isn't one of the preset colour buttons,
     * add it to the list and remove the oldest value from the list.
     */
    void updateColourButtons();

    /**
     * @brief  Updates the color preview and text box when slider values change.
     */
    void sliderValueChanged(juce::Slider* slider) override;

    /**
     * @brief  Updates the sliders and colour preview after the text box value changes.
     */
    void textEditorFocusLost(juce::TextEditor& editor) override;

    /**
     * @brief  Updates the sliders and colour preview after the text box value changes.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * If a colour thumbnail is clicked, apply it to the sliders, text box,
     * and preview.  If the apply button is clicked, add the selection colour
     * to the preview thumbnails and run the selection callback.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * Re-apply component layout using the layoutManager,
     */
    void resized() override;

    /**
     * A square component that shows a colour value.
     */
    class ColourBox: public juce::Component
    {
    public: 
        ColourBox(juce::Colour colour = juce::Colour(), bool drawBorder = true);
		
	virtual ~ColourBox() { };

	juce::Colour getColour();

	void setColour(juce::Colour colour);

    private:
	void paint(juce::Graphics& g) override;
		
	juce::Colour colour;
	bool drawBorder;
    };

    /**
     * A ColourBox button, used for re-selecting previous selected colours.
     */
    class ColourButton : public juce::Button
    {
    public:
	ColourButton(juce::Colour colour = juce::Colour());

	virtual ~ColourButton() { }

	juce::Colour getColour();

	void setColour(juce::Colour colour);
	
    private:
	/**
	 * Change the outline color when the button is down.
	 */
	virtual void paintButton
	(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;	

	void resized() override;

	ColourBox colourBox;
    };
    

    //Current color selection.
    juce::Colour colour;

    //Handles child component layout
    Layout::Group::Manager layoutManager;
    
    //Shows the current color selection over a grid background.
    ColourBox colourPreview;

    //Holds the buttons used to re-select previously selected colors.
    juce::OwnedArray<ColourButton> colourButtons;
    //Shares the last selected colours between all colour pickers.
    static juce::Array<juce::Colour> savedColours;
    //Sets how many saved colours are shown.
    int numSavedColours = 0;

    //Runs when the user selects a color.
    std::function<void(juce::Colour)> selectionCallback;

    //Red, green, blue, and alpha color component sliders.
    juce::Slider colourSliders [4];
    
    /**
     * Bitmasks for selecting each slider's color component.  For each slider at
     * index i, (color & sliderColors[i]) selects that slider's color component
     * from the current color.
     */
    static const juce::uint32 sliderColorMasks [4];
    
    /**
     * Defines the thumb color of each color slider
     */
    static const juce::uint32 sliderThumbColors [4];
    
    /**
     * Draws a gradient showing all slider values behind a color slider.
     */
    class SliderBackground : public juce::Component
    {
    public:
        SliderBackground() { }
        
        virtual ~SliderBackground() { }
        
        /**
         * Sets the colour component used to draw this slider background.
         * 
         * @param colourMask  The sliderColorMask of this component's
         *                         slider.
         */
        void setColourComponent(juce::uint32 colourMask);
        
        /**
         * Updates this component with the current selected colour value.
         * 
         * @param colour  The colour selected by the ColourPicker holding this
         *                component.
         */
        void setColour(juce::Colour colour);
        
    private:     
        /**
         * Draws a gradient of all possible slider color values.  Given the
         * current selected color, this shows what the color would become for 
         * each slider value.
         * 
         * @param g  The Juce graphics object used for drawing.
         */
        void paint(juce::Graphics& g) override;
        
         juce::uint32 colourComponent = 0;
         //darkest color in the component's gradient
         juce::Colour minColour;
         //brightest color in the component's gradient
         juce::Colour maxColour;
    };
    
    SliderBackground sliderBackgrounds [4];
    
    //Holds the colour value as an eight digit hex number string.
    juce::TextEditor colourField;

    //Clicking this confirms the color selection.
    juce::TextButton selectionButton;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourPicker)
};
