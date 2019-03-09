#pragma once
/**
 * @file Layout_Group_Manager.h
 * 
 * @brief  Updates and applies a Group::RelativeLayout to a set of Components.
 */

#include "Layout_Group_RelativeLayout.h"
#include "Layout_Transition_Type.h"
#include "JuceHeader.h"

namespace Layout { namespace Group { class Manager; } } 

class Layout::Group::Manager
{
public:

    Manager() { }

    virtual ~Manager() { }

    /**
     * @brief  Gets the current component group layout held by this Manager.
     * 
     * @return  A copy of the layout saved with setLayout().
     */
    RelativeLayout getLayout() const;
    
    /**
     * @brief  Set a new Component group layout removing any previously set 
     *         layout, and optionally places all layout components within
     *         a containing Component.
     * 
     * @param layout         Defines the position and weight of all components. 
     * 
     * @param parentToInit   If a non-null Component pointer is provided, this 
     *                       will add all components in the layout as children 
     *                       of parentToInit and make them visible.
     */
    void setLayout(const RelativeLayout& layout,
            juce::Component* parentToInit = nullptr);
    
    /**
     *  @brief  Changes the current layout, immediately applies the updated 
     *          layout to all components in the layout, and optionally animates
     *          the transition.
     *
     * @param newLayout    The new layout to apply.
     * 
     * @param parent       The parent component of all layout components.
     *                     All components in the old layout will be removed from
     *                     this component's children. All components in the new 
     *                     layout will be added to this component as children.  
     *                     Layout components will be placed within this 
     *                     component's bounds.
     * 
     * @param transition   Optional transition animation to apply when updating
     *                     the layout.
     * 
     * @param duration     If animating the transition, this defines the
     *                     animation duration in milliseconds.
     */
    void transitionLayout(
            const RelativeLayout& newLayout,
            juce::Component* parent,
            const Transition::Type transition
                = Transition::Type::none,
            const unsigned int duration = 0);

    /**
     * Adds all components in the layout to a parent component, and makes them
     * all visible.
     */
    void addComponentsToParent(juce::Component* parent);

    /**
     * Arranges the components within a bounding rectangle, optionally applying
     * a transition animation to all components in the layout.
     * 
     * @param bounds      The rectangle components will be positioned within.
     * 
     * @param transition  The optional transition animation to apply.
     * 
     * @param duration    The duration of any transition animation, in
     *                    milliseconds.
     */
    void layoutComponents(
            const juce::Rectangle<int>& bounds,
            const Transition::Type transition
                = Transition::Type::none,
            const unsigned int duration = 0);

    /**
     * Remove all saved component layout parameters.
     * 
     * @param removeComponentsFromParent   If true, all components will also
     *                                     be removed from their parent 
     *                                     component.
     */
    void clearLayout(bool removeComponentsFromParent = false);

#if JUCE_DEBUG
    /**
     * Print out the layout to the console for debugging
     */
    void printLayout();
#endif

private:
    /* Current managed layout: */
    RelativeLayout layout;

    /* The sum of RowItem weights for each row. */
    juce::Array<unsigned int> xWeightSums;

    /* The sum of all row layout weights. */
    unsigned int yWeightSum = 0;
};
