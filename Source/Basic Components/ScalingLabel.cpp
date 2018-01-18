/*
  ==============================================================================

    ScalingLabel.cpp
    Created: 17 Jan 2018 3:37:20pm
    Author:  anthony

  ==============================================================================
 */

#include "ScalingLabel.h"

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
//    Rectangle<int> bounds = getLocalBounds();
//    Font currentFont = getFont();
    setFont(getFont().withHeight(getHeight()-fontPadding));
}