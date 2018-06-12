#pragma once
#include "JuceHeader.h"

/**
 * @file RelativeLayoutManager.h
 * 
 * @brief Arranges components in an arbitrary bounding rectangle.
 * 
 * Components are arranged into rows and resized to fit the available space,
 * with weight values determining their sizes in proportion to each other.
 */

/* Layout Declaration Template:

Layout layout =
{
    .xMarginFraction = 0,
    .yMarginFraction = 0.1,
    .rows = 
    {
        {
            .rowWeight = 20,
            .yPaddingWeight = 2,
            .rowItems = 
            {
                {
                    .component = nullptr,
                    .componentWeight = 10,
                    .xPaddingWeight = 2
                }
            }
        }
    }
 }
 * 
 */

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
         * Points to a component in the layout, or nullptr to add an empty space
         * in the layout.
         */
        Component* component;
        /**
         * Component width = total width * componentWeight / horizontalWeightSum
         */
        int componentWeight;
        /**
         * totalWidth * xPaddingWeight / horizontalWeightSum pixels will be left
         * empty around this component.  Padding space is divided evenly between
         * the left and right sides of the component.      
         */
        int xPaddingWeight;
    };

    /**
     * Defines one row of components in a RelativeLayout
     */
    struct RowLayout
    {
        /**
         * Row height = total height * rowWeight / verticalWeightSum
         */
        int rowWeight;
        /**
         * totalWidth * yPaddingWeight / verticalWeightSum pixels will be left
         * empty around this row.  Padding space is divided evenly between the
         * top and bottom of the row.
         */
        int yPaddingWeight;
        /**
         * Holds all ComponentLayouts in the row.
         */
        std::vector<ComponentLayout> rowItems;
    };

    struct Layout
    {
        /**
         * The fraction of the total layout space that should be left empty on
         * the left and right sides of the layout.  This space is divided evenly
         * between the left and right sides.
         */
        float xMarginFraction;
        /**
         * The fraction of the total layout space that should be left empty on
         * the top and bottom of the layout.  This space is divided evenly
         * between the top and bottom.
         */
        float yMarginFraction;
        /**
         * Holds the layouts of all component rows.
         */
        std::vector<RowLayout> rows;
    };

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
     */
    void layoutComponents(Rectangle<int> bounds);

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
    Array<int> xWeightSums;
    //holds the sum of component row weights.
    int yWeightSum = 0;
};
