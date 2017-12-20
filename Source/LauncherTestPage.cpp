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
#include <sstream>
//[/Headers]

#include "LauncherTestPage.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LauncherTestPage::LauncherTestPage ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (imageButton = new ImageButton ("new button"));
    imageButton->addListener (this);

    imageButton->setImages (false, true, true,
                            ImageCache::getFromMemory (icon_png, icon_pngSize), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    appMenu=new AppMenu();
    addAndMakeVisible(appMenu);
    //[/Constructor]
}

LauncherTestPage::~LauncherTestPage()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    appMenu=nullptr;
    //[/Destructor_pre]

    imageButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LauncherTestPage::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LauncherTestPage::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    imageButton->setBounds (432, 224, 40, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LauncherTestPage::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == imageButton)
    {
        //[UserButtonCode_imageButton] -- add your button handler code here..

        getMainStack().popPage(PageStackComponent::kTransitionTranslateHorizontal);
        //[/UserButtonCode_imageButton]
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

<JUCER_COMPONENT documentType="Component" className="LauncherTestPage" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="4 4 480 272" fill="image: iambread_jpg, 1, 0 0" hasStroke="0"/>
  </BACKGROUND>
  <IMAGEBUTTON name="new button" id="cfbb5d0646892c84" memberName="imageButton"
               virtualName="" explicitFocusOrder="0" pos="432 224 40 40" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="icon_png" opacityNormal="1" colourNormal="0"
               resourceOver="" opacityOver="1" colourOver="0" resourceDown=""
               opacityDown="1" colourDown="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: icon_png, 272, "../../../../Pictures/icon.png"
static const unsigned char resource_LauncherTestPage_icon_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,48,0,0,0,48,8,2,0,0,0,216,96,110,208,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,
0,154,156,24,0,0,0,7,116,73,77,69,7,225,8,18,7,4,41,248,61,253,191,0,0,0,175,73,68,65,84,88,195,99,96,24,5,163,96,152,1,70,146,84,31,58,124,156,60,107,236,108,45,137,84,201,52,216,66,104,212,65,67,206,
65,44,100,235,220,147,212,131,71,214,101,94,201,104,148,141,166,33,44,224,224,230,171,84,87,57,26,101,163,14,26,208,68,93,83,237,3,103,39,60,220,140,44,181,96,214,76,100,110,66,90,250,104,148,141,58,104,
80,38,234,255,31,95,224,74,170,248,83,49,154,172,138,188,47,50,183,182,43,101,52,202,70,29,52,248,154,176,248,83,241,225,75,247,71,163,108,212,65,131,50,81,227,73,185,104,82,45,173,91,70,163,108,52,13,
145,86,24,162,149,132,104,128,64,193,216,53,26,101,163,14,26,5,163,96,20,16,11,0,133,141,45,82,114,105,117,164,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* LauncherTestPage::icon_png = (const char*) resource_LauncherTestPage_icon_png;
const int LauncherTestPage::icon_pngSize = 272;



//[EndFile] You can add extra defines here...
//[/EndFile]
