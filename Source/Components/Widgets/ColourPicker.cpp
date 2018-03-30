#include "ColourPicker.h"

Array<Colour> ColourPicker::savedColours;

ColourPicker::ColourPicker(int numSavedColours, Colour colour):
Localized("ColourPicker"),
numSavedColours(numSavedColours),
selectionCallback([](Colour c){}),	
colour(colour)
{
    RelativeLayoutManager::Layout layout =
    {
        {6,
	    {
	        {&colourPreview,1}
	    }
        },
	{1,
	    {
	        {&rSlider, 1}
	    }
        },
	{1,
	    {
	        {&gSlider, 1}
	    }
        },
	{1,
	    {
	        {&bSlider, 1}
	    }
        },
	{1,
	    {
	        {&aSlider, 1}
	    }
        },
	{1,
	    {
	        {&colourField, 1},
	        {&selectionButton, 1}
	    }
        }
    };
    colourPreview.setColour(colour);
    for(int i = 0; i < numSavedColours; i++)
    {
        ColourButton* cButton = new ColourButton(savedColours[i]);
	colourButtons.add(cButton);
    	addAndMakeVisible(cButton);
	cButton->addListener(this);
    }


    Array<Slider*> sliders = 
    {
        &rSlider,
	&gSlider,
	&bSlider,
	&aSlider
    };
    
    for(Slider* slider : sliders)
    {
        slider->setSliderStyle(Slider::LinearHorizontal);
        slider->setRange(0,0xff,1);
	slider->addListener(this);
	slider->setTextBoxStyle(Slider::NoTextBox, true,0,0);
    }
    rSlider.setColour(Slider::thumbColourId,Colours::red);    
    gSlider.setColour(Slider::thumbColourId,Colours::green);    
    bSlider.setColour(Slider::thumbColourId,Colours::blue);    
    aSlider.setColour(Slider::thumbColourId,Colour(0x77777777));    
    setSlidersToColour();
    
    colourField.addListener(this);
    setEditorText();
    colourField.setJustification(Justification::centred);    
    selectionButton.setButtonText(localeText(set_colour));
    selectionButton.addListener(this);
    layoutManager.setLayout(layout,this);

}

/**
 * Gets the current selected colour.
 */
Colour ColourPicker::getSelectedColour()
{
    return colour;
}

/**
 * Sets a new colour value.
 */
void ColourPicker::setSelectedColour(Colour colour, bool runCallback)
{
    this->colour = colour;
    setSlidersToColour();
    setEditorText();
    if(runCallback)
    {
        updateColourButtons();
	selectionCallback(colour);
    }
}

/**
 * Assigns a callback function to run when a colour is selected.
 */
void ColourPicker::setSelectionCallback(std::function<void(Colour)> callback)
{
    selectionCallback = callback;
}

/**
 * Apply the current colour selection to the sliders.
 */
void ColourPicker::setSlidersToColour()
{
	rSlider.setValue(colour.getRed());
	gSlider.setValue(colour.getGreen());
	bSlider.setValue(colour.getBlue());
	aSlider.setValue(colour.getAlpha());
}

/**
 * Set the text field to the current color's 8 digit hex value.
 */
void ColourPicker::setEditorText()
{
     String colourStr = colour.toString();
     while(colourStr.length() < 8)
     {
        colourStr = '0' + colourStr;
     }
     colourField.setText("0x"+colourStr);
}

/**
 * If the current colour selection isn't one of the preset colour buttons,
 * add it to the list and remove the oldest value from the list.
 */
void ColourPicker::updateColourButtons()
{
    if(!savedColours.contains(colour))
    {
        savedColours.insert(0,colour);
	savedColours.resize(numSavedColours);
	for(int i = 0; i < numSavedColours; i++)
	{
	   colourButtons[i]->setColour(savedColours[i]);
	}
    }
}



/**
 * Updates the color preview and text box when slider values change.
 */
void ColourPicker::sliderValueChanged(Slider* slider)
{
    colour = Colour(
            (uint8_t) rSlider.getValue(),
	    (uint8_t) gSlider.getValue(),
            (uint8_t) bSlider.getValue(),
            (uint8_t) aSlider.getValue());	     
    setEditorText();
    colourPreview.setColour(colour);
}

/**
 * Updates the sliders and colour preview after the text box value changes.
 */
void ColourPicker::textEditorFocusLost(TextEditor& editor)
{
    colour = Colour(editor.getText().getHexValue32());
    setEditorText();
    setSlidersToColour();
    colourPreview.setColour(colour);
}

/**
 * Updates the sliders and colour preview after the text box value changes.
 */
void ColourPicker::textEditorReturnKeyPressed(TextEditor& editor) 
{
    colour = Colour(editor.getText().getHexValue32());
    setEditorText();
    setSlidersToColour();
    colourPreview.setColour(colour);
}

/**
 * If a colour thumbnail is clicked, apply it to the sliders, text box,
 * and preview.  If the apply button is clicked, add the selection colour
 * to the preview thumbnails and run the selection callback.
 */
void ColourPicker::buttonClicked(Button* button) 
{
    DBG("click!");
    if(button == &selectionButton)
    {
	updateColourButtons();
	selectionCallback(colour);
	return;	
    }
    for(ColourButton* cButton : colourButtons)
    {
        if(cButton == button)
	{
	  setSelectedColour(cButton->getColour());
	  DBG("Clicked!");
	  break;
	}
    }
}

/**
 * Re-apply component layout using the layout manager.
 */
void ColourPicker::resized()
{
    Rectangle<int> bounds = getLocalBounds().reduced(getWidth() / 20);
    int padding = 3;
    if(numSavedColours > 0)
    {
        int btnHeight = (getHeight() / numSavedColours)
	       	- padding * (numSavedColours+1);
        int btnWidth = std::min(getWidth() / 3, btnHeight);
	bounds.setWidth(bounds.getWidth() - (btnWidth + padding * 2));
	int left = bounds.getRight() + padding;
	int top = bounds.getY() + padding;
	for(ColourButton* btn : colourButtons)
	{
            btn->setBounds(left,top,btnWidth,btnHeight);
	    top += btnHeight + padding;
	}
	
    }
    layoutManager.layoutComponents(bounds,padding,padding);
}

ColourPicker::ColourBox::ColourBox(Colour colour, bool drawBorder) : 
colour(colour),
drawBorder(drawBorder)
{
    setInterceptsMouseClicks(false,false);
}

Colour ColourPicker::ColourBox::getColour()
{
    return colour;
}

void ColourPicker::ColourBox::setColour(Colour colour)
{
    this->colour = colour;
    repaint();
}

void ColourPicker::ColourBox::paint(Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds();
    if(!colour.isOpaque())
    {
        float checkSize = bounds.getHeight() / 8;
	g.fillCheckerBoard(bounds.toFloat(),checkSize,checkSize,
	        Colour(0x05050505), Colour(0x01010101));
    }
    g.setColour(colour);
    g.fillRect(bounds);
    if(drawBorder)
    {
    	g.setColour(findColour(TextEditor::outlineColourId));
    	g.drawRect(bounds, 1+getWidth()/35);
    }
}

ColourPicker::ColourButton::ColourButton(Colour colour) 
: colourBox(colour,false),	
Button("colourButton")
{
    addAndMakeVisible(colourBox);
}

Colour ColourPicker::ColourButton::getColour()
{
    return colourBox.getColour();
}

void ColourPicker::ColourButton::setColour(Colour colour)
{
    colourBox.setColour(colour);
}

void ColourPicker::ColourButton::resized()
{
    colourBox.setBounds(getLocalBounds());
}

/**
 * Change the outline color when the button is down.
 */
void ColourPicker::ColourButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    g.setColour(findColour(isButtonDown ? 
			      TextEditor::focusedOutlineColourId
			    : TextEditor::outlineColourId));
    g.drawRect(getLocalBounds(), getWidth()/35+1);
}    
