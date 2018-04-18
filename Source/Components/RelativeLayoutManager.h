/**
 * @file RelativeLayoutManager.h
 * 
 * RelativeLayoutManager arranges components in an arbitrary bounding rectangle.
 * Components are arranged into rows and resized to fit the available space,
 * with weight values determining their sizes in proportion to each other.
 */
#pragma once
#include "JuceHeader.h"

class RelativeLayoutManager
{
public:

    RelativeLayoutManager() { }

    virtual ~RelativeLayoutManager() { }

    /**
     * Defines one component's space in a RelativeLayout row.
     */
    struct ComponentLayout
    {
        /**
         * @param component     Either a valid component pointer, or nullptr to
         *                       specify an area of empty space.
         * 
         * @param horizWeight   The amount of horizontal space to give this
         *                       component, relative to the other components in
         *                       the row.
         */
        ComponentLayout(Component* component, int horizWeight) :
        component(component),
        horizWeight(horizWeight) { };
        
        Component* component;
        int horizWeight;
    };

    /**
     * Defines one row in a RelativeLayout
     */
    struct RowLayout
    {
        /**
         * @param vertWeight  The amount of vertical space to give this row,
         *                     relative to the other rows in the layout.
         * 
         * @param components  The layout of all components in the row. An empty
         *                     component list may be used to specify an empty
         *                     space.
         */
        RowLayout (int vertWeight, std::vector<ComponentLayout> components) :
        vertWeight(vertWeight),
        components(components) { };
        
        int vertWeight;
        std::vector<ComponentLayout> components;
    };

    /**
     * This type allows an entire layout to be declared at once in a
     * brace enclosed initializer list.
     */
    typedef std::vector<RowLayout> Layout;

    /**
     * Set a new Component layout, removing all old layout definitions.
     * 
     * @param rows          Defines the position and weight of all components. 
     *                       Null components may be used to define empty spaces. 
     * 
     * @param parentToInit  If a non-null component pointer is provided, This 
     *                       will add all components in the layout as children 
     *                       of parentToInit and make them visible.
     */
    void setLayout(const Layout& layout, Component* parentToInit = nullptr);

    /**
     * Adds all components in the layout to a parent component, and makes them
     * all visible.
     */
    void addComponentsToParent(Component* parent);

    /**
     * Arranges the components within a bounding rectangle.
     * 
     * @param bounds    The rectangle components will be positioned within.
     * 
     * @param xPadding  The space in pixels to leave between components in a 
     *                   row, and between components and the left and right 
     *                   edges of the bounds.
     * 
     * @param yPadding  The space in pixels to leave between rows, and between
     *                   the rows and the top and bottom edges of the bounds.
     */
    void layoutComponents(Rectangle<int> bounds, int xPadding, int yPadding);

    /**
     * Remove all saved component layout parameters.
     * 
     * @param removeComponentsFromParent  If true, all components will also
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
    //Current layout definition
    Layout layout;
    //Holds the sum of component weights for each row
    Array<int> horizWeightSums;
    //holds the sum of component row weights.
    int vertWeightSum = 0;
};
