#include "NavButton.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_JSONKeys.h"

NavButton::NavButton(NavButton::WindowEdge edge) : 
Theme::Image::Component<DrawableImageButton>(getEdgeComponentKey(edge)),
boundsManager(this, getEdgeComponentKey(edge)),
edge(edge)
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
    Layout::Component::ConfigFile config;
    Layout::Component::ConfigLayout layout 
            = config.getLayout(getEdgeComponentKey(edge));
    return std::min(layout.getXFraction() + layout.getWidthFraction(),
            1.0f - layout.getXFraction()) + marginPadding;
}

/*
 * Gets the amount of vertical margin space needed to keep window
 * content from overlapping with a NavButton.
 */
float NavButton::yMarginFractionNeeded(NavButton::WindowEdge edge)
{
    Layout::Component::ConfigFile config;
    Layout::Component::ConfigLayout layout 
            = config.getLayout(getEdgeComponentKey(edge));
    return std::min(layout.getYFraction() + layout.getHeightFraction(),
            1.0f - layout.getYFraction()) + marginPadding;
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
 * Applies the navigation button bounds defined in the layout.json file.
 */
void NavButton::applyConfigBounds()
{
    boundsManager.applyConfigBounds();
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
            return Layout::Component::JSONKeys::pageUp;
        case down:
            return Layout::Component::JSONKeys::pageDown;
        case left:
            return Layout::Component::JSONKeys::pageLeft;
        case right:
            return Layout::Component::JSONKeys::pageRight;
    }
    DBG("NavButton::" << __func__ << ": Unknown edge value!");
    return juce::Identifier::null;
}
