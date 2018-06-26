#pragma once
#include "ConfigurableImageButton"

/**
 * @file ArrowButton.h
 * 
 * @brief A directional navigation button assigned to one edge of the window.
 *        Button position and direction are chosen based on the assigned edge.
 */

class ArrowButton : public ConfigurableImageButton
{
public:
    /**
     */
    enum WindowEdge
    {
        up,
        down,
        left,
        right
    };
    
    /**
     * @param side
     */
    ArrowButton(WindowEdge side);
    
    virtual ~ArrowButton();
    
    /**
     * @return
     */
    float horizMarginFractionNeeded() const;
    
    /**
     * @return
     */
    float vertMarginFractionNeeded() const;
private:
    /**
     * @param side
     * 
     * @return
     */
    static String getComponentKey(ScreenSide side);
    
};