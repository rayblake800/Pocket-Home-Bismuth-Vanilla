#include "ScalingTextButton.h"


    
/*
 * Sets the maximum text height scale to use when rendering this button's
 * text.
 */
void ScalingTextButton::setMaxTextScale(ComponentConfigFile::TextSize textScale)
{
    maxTextScale = textScale;
    resized();
}

/*
 * Gets the maximum text height scale to use when rendering this button's
 * text.
 */
ComponentConfigFile::TextSize ScalingTextButton::getMaxTextScale()
{
    return maxTextScale;
}