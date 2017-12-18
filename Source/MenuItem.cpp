/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MenuItem.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MenuItem::MenuItem ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (openButton = new ImageButton ("openButton"));
    openButton->setButtonText (TRANS("new button"));
    openButton->addListener (this);

    openButton->setImages (false, true, false,
                           ImageCache::getFromMemory (arrow_png, arrow_pngSize), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));
    cachedImage_icon_png_1 = ImageCache::getFromMemory (icon_png, icon_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MenuItem::~MenuItem()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    openButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MenuItem::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        float x = 0.0f, y = 0.0f, width = 416.0f, height = 64.0f;
        Colour fillColour = Colour (0xff939089);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 10.000f);
    }

    {
        int x = 68, y = 20, width = 340, height = 30;
        String text (TRANS("Your text goes here"));
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 12, y = 12, width = 44, height = 44;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        g.drawImage (cachedImage_icon_png_1,
                     x, y, width, height,
                     0, 0, cachedImage_icon_png_1.getWidth(), cachedImage_icon_png_1.getHeight());
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MenuItem::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    openButton->setBounds (376, 8, 32, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MenuItem::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == openButton)
    {
        //[UserButtonCode_openButton] -- add your button handler code here..
        //[/UserButtonCode_openButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MenuItem" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <ROUNDRECT pos="0 0 416 64" cornerSize="10" fill="solid: ff939089" hasStroke="0"/>
    <TEXT pos="68 20 340 30" fill="solid: ff000000" hasStroke="0" text="Your text goes here"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <IMAGE pos="12 12 44 44" resource="icon_png" opacity="1" mode="0"/>
  </BACKGROUND>
  <IMAGEBUTTON name="openButton" id="6117d123dba95872" memberName="openButton"
               virtualName="" explicitFocusOrder="0" pos="376 8 32 48" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="0"
               resourceNormal="arrow_png" opacityNormal="1" colourNormal="0"
               resourceOver="" opacityOver="1" colourOver="0" resourceDown=""
               opacityDown="1" colourDown="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: icon_png, 272, "../../../../Pictures/icon.png"
static const unsigned char resource_MenuItem_icon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,48,0,0,0,48,8,2,0,0,0,216,96,110,208,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,
24,0,0,0,7,116,73,77,69,7,225,8,18,7,4,41,248,61,253,191,0,0,0,175,73,68,65,84,88,195,99,96,24,5,163,96,152,1,70,146,84,31,58,124,156,60,107,236,108,45,137,84,201,52,216,66,104,212,65,67,206,65,44,100,
235,220,147,212,131,71,214,101,94,201,104,148,141,166,33,44,224,224,230,171,84,87,57,26,101,163,14,26,208,68,93,83,237,3,103,39,60,220,140,44,181,96,214,76,100,110,66,90,250,104,148,141,58,104,80,38,234,
255,31,95,224,74,170,248,83,49,154,172,138,188,47,50,183,182,43,101,52,202,70,29,52,248,154,176,248,83,241,225,75,247,71,163,108,212,65,131,50,81,227,73,185,104,82,45,173,91,70,163,108,52,13,145,86,24,
162,149,132,104,128,64,193,216,53,26,101,163,14,26,5,163,96,20,16,11,0,133,141,45,82,114,105,117,164,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* MenuItem::icon_png = (const char*) resource_MenuItem_icon_png;
const int MenuItem::icon_pngSize = 272;

// JUCER_RESOURCE: arrow_png, 491, "../../../../Pictures/arrow.png"
static const unsigned char resource_MenuItem_arrow_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,41,0,0,0,81,8,6,0,0,0,79,17,143,90,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,
24,0,0,0,7,116,73,77,69,7,225,12,17,21,52,11,10,140,244,152,0,0,0,29,105,84,88,116,67,111,109,109,101,110,116,0,0,0,0,0,67,114,101,97,116,101,100,32,119,105,116,104,32,71,73,77,80,100,46,101,7,0,0,1,97,
73,68,65,84,104,222,237,218,217,14,131,32,16,133,97,230,188,255,59,211,171,38,93,180,178,204,242,155,212,171,86,49,126,46,192,192,208,122,239,189,193,55,181,214,26,29,170,231,15,50,84,175,127,168,80,125,
238,32,66,117,180,147,6,213,217,1,18,84,191,14,82,160,186,42,64,128,106,164,80,53,84,163,5,43,161,154,41,92,5,213,236,9,21,80,91,189,176,153,89,42,146,14,181,221,87,153,1,53,143,111,46,26,106,94,149,35,
18,106,158,181,56,10,106,23,150,238,121,227,81,237,228,202,5,123,54,18,1,85,224,43,236,217,200,82,168,18,42,69,207,70,150,64,181,120,94,42,84,27,55,152,6,221,110,120,51,122,38,151,222,33,26,234,214,133,
69,66,93,251,217,40,168,123,48,16,1,13,9,173,188,161,97,129,170,39,52,52,236,247,130,134,15,162,60,160,41,67,210,93,104,218,0,127,7,154,134,220,129,166,34,151,161,255,39,121,167,111,18,95,187,241,237,
36,190,199,193,247,221,248,40,8,31,79,226,35,115,252,24,7,63,90,196,143,187,179,166,172,141,14,92,70,102,79,250,27,29,56,141,172,74,155,24,29,56,140,172,78,60,25,29,120,137,164,164,238,238,145,91,164,
3,15,145,248,124,55,126,229,0,126,13,6,126,53,11,29,216,218,66,70,172,98,146,75,116,224,20,178,10,56,140,172,4,14,33,171,129,151,72,2,240,39,146,2,60,69,146,128,135,72,26,240,11,73,4,190,33,169,192,229,
232,39,123,123,0,75,227,0,107,251,214,142,94,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* MenuItem::arrow_png = (const char*) resource_MenuItem_arrow_png;
const int MenuItem::arrow_pngSize = 491;


//[EndFile] You can add extra defines here...
//[/EndFile]
