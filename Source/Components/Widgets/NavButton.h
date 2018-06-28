#pragma once
#include "ConfigurableImageButton.h"

/**
 * @file NavButton.h
 * 
 * @brief A directional navigation button assigned to one edge of the window.
 *        Button position and direction are chosen based on the assigned edge.
 * 
 * NavButtons are meant to be used for navigating between page components and
 * scrolling page content.  NavButtons are defined within the components.json
 * file, where their images, colors, relative sizes, and relative positions are 
 * set.  
 * 
 * Unless components.json has been edited to place them somewhere else, 
 * NavButtons sit centered against the side of the window assigned to them on
 * construction, pointing towards the assigned edge.
 * 
 * Because NavButtons are meant to be placed on the margins of the window,
 * methods are provided to find the margin fractions needed to keep other window
 * content from overlapping with the arrow buttons.
 */

class NavButton : public ConfigurableImageButton
{
public:
    /**
     * Defines the four available button subtypes.
     */
    enum WindowEdge
    {
        up,
        down,
        left,
        right
    };
    
    /**
     * @param edge  The button subType to create.
     */
    NavButton(WindowEdge edge);
    
    virtual ~NavButton() { }
    
    /**
     * Gets the window edge assigned to this NavButton.
     * 
     * @return  The WindowEdge value set when the button was created. 
     */
    WindowEdge getEdge();
    
    /**
     * Gets the amount of horizontal margin space needed to keep window
     * content from overlapping with a NavButton.
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
    static String getEdgeComponentKey(WindowEdge edge);
    
    WindowEdge edge;
};