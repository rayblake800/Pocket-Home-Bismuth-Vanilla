#include "ScalingTextButton.h"
    
/*
 * Sets the maximum text height scale to use when rendering this button's text.
 */
void ScalingTextButton::setMaxTextScale(Layout::Component::TextSize textScale)
{
    maxTextScale = textScale;
    resized();
}

/*
 * Gets the maximum text height scale to use when rendering this button's text.
 */
Layout::Component::TextSize ScalingTextButton::getMaxTextScale()
{
    return maxTextScale;
}
