#include "ColourPicker.h"
#include "ComponentConfigFile.h"

juce::Array<juce::Colour> ColourPicker::savedColours;

const juce::uint32 ColourPicker::sliderColorMasks [4] = 
{
    0x00ff0000,
    0x0000ff00,
    0x000000ff,
    0xff000000
};
    
const juce::uint32 ColourPicker::sliderThumbColors [4] =
{
  0xffff0000,
  0xff00ff00,
  0xff0000ff,
  0x77777777
};

ColourPicker::ColourPicker(int numSavedColours, juce::Colour colour):
Localized("ColourPicker"),
numSavedColours(numSavedColours),
selectionCallback([](juce::Colour c){}),	
colour(colour)
{
    using namespace juce;
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout(
    {
        Row(50, 
        { 
            RowItem(&colourPreview, 40),
            RowItem(nullptr, 10) //Empty space to hold color buttons
        }),
        Row(10, { RowItem(&colourSliders[0]) }),
        Row(10, { RowItem(&colourSliders[1]) }),
        Row(10, { RowItem(&colourSliders[2]) }),
        Row(10, { RowItem(&colourSliders[3]) }),
	Row(10,
        {
            RowItem(&colourField),
            RowItem(&selectionButton)
        })
    });
    colourPreview.setColour(colour);
    for(int i = 0; i < numSavedColours; i++)
    {
        ColourButton* cButton = new ColourButton(savedColours[i]);
	colourButtons.add(cButton);
    	addAndMakeVisible(cButton);
	cButton->addListener(this);
    }
    
    for(int i = 0; i < 4; i++)
    {
        colourSliders[i].setSliderStyle(Slider::LinearHorizontal);
        colourSliders[i].setRange(0,0xff,1);
	colourSliders[i].addListener(this);
	colourSliders[i].setTextBoxStyle(Slider::NoTextBox, true,0,0);
        colourSliders[i].setColour(Slider::thumbColourId,
                Colour(sliderThumbColors[i]));   
        sliderBackgrounds[i].setColourComponent(sliderColorMasks[i]);
        addAndMakeVisible(sliderBackgrounds[i]);
        sliderBackgrounds[i].toBack();
    }  
    
    ComponentConfigFile config;
    colourField.setFont(Font(config.getFontHeight
            (ComponentConfigFile::smallText)));
    colourField.setJustification(Justification::centred);
    colourField.addListener(this);
    
    selectionButton.setButtonText(localeText(set_colour));
    selectionButton.addListener(this);
    
    layoutManager.setLayout(layout,this);
    setSlidersToColour();
    setEditorText();
}

/**
 * Gets the current selected colour.
 */
juce::Colour ColourPicker::getSelectedColour()
{
    return colour;
}

/**
 * Sets a new colour value.
 */
void ColourPicker::setSelectedColour(juce::Colour colour, bool runCallback)
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
void ColourPicker::setSelectionCallback
(std::function<void(juce::Colour)> callback)
{
    selectionCallback = callback;
}

/**
 * Apply the current colour selection to the sliders.
 */
void ColourPicker::setSlidersToColour()
{
    using namespace juce;
    for(int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setColour(colour);
        uint32 colourValue = colour.getARGB() & sliderColorMasks[i];
        while(colourValue > 0xff)
        {
            colourValue >>= 8;
        }
        colourSliders[i].setValue(colourValue);
    }
}

/**
 * Set the text field to the current color's 8 digit hex value.
 */
void ColourPicker::setEditorText()
{
    using namespace juce;
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
void ColourPicker::sliderValueChanged(juce::Slider* slider)
{
    using namespace juce;
    uint32 colorValue = 0;
    for(int i = 0; i < 4; i++)
    {
        colorValue += ((uint32) colourSliders[i].getValue() 
                * (sliderColorMasks[i] & 0x01010101));
    }
    colour = Colour(colorValue);
    for(int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setColour(colour);
    }	     
    setEditorText();
    colourPreview.setColour(colour);
}

/**
 * Updates the sliders and colour preview after the text box value changes.
 */
void ColourPicker::textEditorFocusLost(juce::TextEditor& editor)
{
    using namespace juce;
    colour = Colour(editor.getText().getHexValue32());
    setEditorText();
    setSlidersToColour();
    colourPreview.setColour(colour);
}

/**
 * Updates the sliders and colour preview after the text box value changes.
 */
void ColourPicker::textEditorReturnKeyPressed(juce::TextEditor& editor) 
{
    using namespace juce;
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
void ColourPicker::buttonClicked(juce::Button* button) 
{
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
    using namespace juce;
    int padding = 3;
    layoutManager.layoutComponents(getLocalBounds());
    for(int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setBounds(colourSliders[i].getBounds());
    }
    if(numSavedColours > 0)
    {
        int btnHeight = (colourPreview.getHeight() / numSavedColours)
	       	- padding;
        int btnWidth = getWidth() - colourPreview.getWidth() - 2*padding;
	int left = colourPreview.getRight() + padding;
	int top = colourPreview.getY() + padding;
	for(ColourButton* btn : colourButtons)
	{
            btn->setBounds(left,top,btnWidth,btnHeight);
	    top += btnHeight + padding;
	}
	
    }
}

ColourPicker::ColourBox::ColourBox(juce::Colour colour, bool drawBorder) : 
colour(colour),
drawBorder(drawBorder)
{
    setInterceptsMouseClicks(false,false);
}

juce::Colour ColourPicker::ColourBox::getColour()
{
    return colour;
}

void ColourPicker::ColourBox::setColour(juce::Colour colour)
{
    this->colour = colour;
    repaint();
}

void ColourPicker::ColourBox::paint(juce::Graphics& g)
{
    using namespace juce;
    Rectangle<int> bounds = getLocalBounds();
    if(!colour.isOpaque())
    {
        float checkSize = bounds.getHeight() / 8;
	g.fillCheckerBoard(bounds.toFloat(),checkSize,checkSize,
                findColour(ColourPicker::checkerboardLight),
                findColour(ColourPicker::checkerboardDark));
    }
    g.setColour(colour);
    g.fillRect(bounds);
    if(drawBorder)
    {
    	g.setColour(findColour(ColourPicker::outline));
    	g.drawRect(bounds, 1+getWidth()/35);
    }
}

ColourPicker::ColourButton::ColourButton(juce::Colour colour) 
: colourBox(colour, true),	
Button("colourButton")
{
    addAndMakeVisible(colourBox);
}

juce::Colour ColourPicker::ColourButton::getColour()
{
    return colourBox.getColour();
}

void ColourPicker::ColourButton::setColour(juce::Colour colour)
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
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using namespace juce;
    g.setColour(findColour(isButtonDown ? 
			      ColourPicker::outline
			    : ColourPicker::focusedOutline));
    g.drawRect(getLocalBounds(), getWidth()/35 + 2);
}    

void ColourPicker::SliderBackground::setColourComponent
(juce::uint32 colourMask)
{
    colourComponent = colourMask;
}
        
/*
 * Updates this component with the current selected colour value.
 */
void ColourPicker::SliderBackground::setColour(juce::Colour colour)
{
    using namespace juce;
    uint32 colorARGB = colour.getARGB();
    minColour = Colour(colorARGB & (~colourComponent));
    maxColour = Colour(colorARGB | colourComponent);
    repaint();
}
     
/*
 * Draws a gradient of all possible slider color values.  Given the
 * current selected color, this shows what the color would become for 
 * each slider value.
 */
void ColourPicker::SliderBackground::paint(juce::Graphics& g)
{
    using namespace juce;
    float checkSize = getHeight() / 8;
    g.fillCheckerBoard(getLocalBounds().toFloat(),checkSize,checkSize,
            findColour(ColourPicker::ColourIds::checkerboardLight),
            findColour(ColourPicker::ColourIds::checkerboardDark));
    g.setGradientFill(ColourGradient::horizontal(minColour, 0,
            maxColour, getWidth()));
    g.fillRect(getLocalBounds());
    g.setColour(findColour(ColourPicker::ColourIds::outline));
    g.drawRect(getLocalBounds(), checkSize);
}