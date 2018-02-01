#include "ScalingLabel.h"
#include "../Utils.h"

ScalingLabel::ScalingLabel(const String &componentName,
        const String &labelText, const int& fontPadding) 
: Label(componentName, labelText),
        fontPadding(fontPadding)
{
}

ScalingLabel::~ScalingLabel()
{
}

void ScalingLabel::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    String text = getText();
    int numLines = 1;
    for(int i = 0;i < text.length(); i++){
        if(text[i] == '\n'){
            numLines++;
        }
    }
    Font resizedFont = getFont().withHeight(getHeight()/numLines-fontPadding);
    int width=resizedFont.getStringWidth(getText());
    if(width>bounds.getWidth())
    {
        resizedFont.setHeight(bounds.getWidth()*resizedFont.getHeight()/width);
    }
    setFont(resizedFont);
}