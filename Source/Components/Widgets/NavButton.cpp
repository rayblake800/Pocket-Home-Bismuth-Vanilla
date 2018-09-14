#include "NavButton.h"
#include "ComponentConfigFile.h"
#include "ComponentConfigKeys.h"

NavButton::NavButton(NavButton::WindowEdge edge) : edge(edge),
ConfigurableImageButton(NavButton::getEdgeComponentKey(edge)) 
{ 
    setWantsKeyboardFocus(false);
}
 
/*
 * Gets the window edge assigned to this NavButton. 
 */
NavButton::WindowEdge NavButton::getEdge()
{
    return edge;
}
    
/*
 * Gets the amount of horizontal margin space needed to keep window
 * content from overlapping with a NavButton.
 */
float NavButton::xMarginFractionNeeded(NavButton::WindowEdge edge)
{
    ComponentConfigFile config;
    ComponentSettings settings = config.getComponentSettings
        (getEdgeComponentKey(edge));
    return std::min(settings.getXFraction() + settings.getWidthFraction(),
            1.0f - settings.getXFraction()) + marginPadding;
}

/*
 * Gets the amount of vertical margin space needed to keep window
 * content from overlapping with a NavButton.
 */
float NavButton::yMarginFractionNeeded(NavButton::WindowEdge edge)
{
    ComponentConfigFile config;
    ComponentSettings settings = config.getComponentSettings
        (getEdgeComponentKey(edge));
    return std::min(settings.getYFraction() + settings.getHeightFraction(),
            1.0f - settings.getYFraction()) + marginPadding;
}


/*
 * Gets the amount of horizontal margin space needed to keep window
 * content from overlapping with this NavButton.
 */
float NavButton::xMarginFractionNeeded() const
{
    return NavButton::xMarginFractionNeeded(edge);
}

/*
 * Gets the amount of vertical margin space needed to keep window
 * content from overlapping with this NavButton.
 */
float NavButton::yMarginFractionNeeded() const
{
    return NavButton::yMarginFractionNeeded(edge);
}

/*
 * Given a window edge, return the component key string for the
 * corresponding NavButton type.
 */
const juce::Identifier& NavButton::getEdgeComponentKey(const WindowEdge edge)
{
    switch(edge)
    {
        case up:
            return ComponentConfigKeys::pageUpKey;
        case down:
            return ComponentConfigKeys::pageDownKey;
        case left:
            return ComponentConfigKeys::pageLeftKey;
        case right:
            return ComponentConfigKeys::pageRightKey;
    }
    DBG("NavButton::" << __func__ << ": Unknown edge value!");
    return juce::Identifier::null;
}
