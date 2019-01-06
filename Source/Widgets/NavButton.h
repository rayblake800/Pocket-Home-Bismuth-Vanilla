#pragma once
/**
 * @file NavButton.h
 * 
 * @brief A directional navigation button assigned to one edge of the window.
 *        Button position and direction are chosen based on the assigned edge.
 */

#include "Theme_Image_Component.h"
#include "Layout_Component_Manager.h"
#include "DrawableImageButton.h"

/**
 *  NavButtons are meant to be used for navigating between page components and
 * scrolling page content. NavButton's relative position and size are defined in
 * the layout.json configuration file, and its image resources and colors are
 * defined in the imageAssets.json configuration file.
 * 
 *  Unless layout.json has been edited to place them somewhere else, NavButtons 
 * sit centered against the side of the window assigned to them on construction,
 * pointing towards the assigned edge.
 * 
 *  Because NavButtons are meant to be placed on the margins of the window,
 * methods are provided to find the margin fractions needed to keep other window
 * content from overlapping with the arrow buttons.
 */
class NavButton : public Theme::Image::Component<DrawableImageButton>
{
public:
    /**
     * @brief  Defines the four available button varieties.
     */
    enum WindowEdge
    {
        up,
        down,
        left,
        right
    };
    
    /**
     * @param edge  The button variety to create.
     */
    NavButton(WindowEdge edge);
    
    virtual ~NavButton() { }
    
    /**
     * @brief  Gets the window edge assigned to this NavButton.
     * 
     * @return  The WindowEdge value set when the button was created. 
     */
    WindowEdge getEdge();
    
    /**
     * @brief  Gets the amount of horizontal margin space needed to keep window
     *         content from overlapping with a NavButton.
     * 
     * @param edge  Sets which NavButton type's margin values are returned.
     * 
     * @return  The fraction of the window's width that should be set aside
     *          as margin space to keep window content and the NavButton
     *          from overlapping.
     */
    static float xMarginFractionNeeded(WindowEdge edge);
    
    /**
     * Gets the amount of vertical margin space needed to keep window
     * content from overlapping with a NavButton.
     * 
     * @param edge  Sets which NavButton type's margin values are returned.
     * 
     * @return  The fraction of the window's height that should be set aside
     *          as margin space to keep window content and the NavButton
     *          from overlapping.
     */
    static float yMarginFractionNeeded(WindowEdge edge);
    
    /**
     * Gets the amount of horizontal margin space needed to keep window
     * content from overlapping with this NavButton.
     * 
     * @return  The fraction of the window's width that should be set aside
     *          as margin space to keep window content and this NavButton
     *          from overlapping.
     */
    float xMarginFractionNeeded() const;
    
    /**
     * Gets the amount of vertical margin space needed to keep window
     * content from overlapping with this NavButton.
     * 
     * @return  The fraction of the window's height that should be set aside
     *          as margin space to keep window content and this NavButton
     *          from overlapping.
     */
    float yMarginFractionNeeded() const;
    
    /**
     * @brief  Applies the navigation button bounds defined in the layout.json
     *         file.
     */
    void applyConfigBounds();

private:
    /**
     * Given a window side, return the component key string for the
     * corresponding NavButton type.
     * 
     * @param edge  One edge of the application window.
     * 
     * @return  The component key string for the NavButton placed on that
     *          side.
     */
    static const juce::Identifier& getEdgeComponentKey(const WindowEdge edge);

    /* Finds appropriate bounds using config data and window size. */
    Layout::Component::Manager boundsManager;
    
    WindowEdge edge;
    
    //Extra padding space to add to button margins
    static const constexpr float marginPadding = 0.02;
};
