#pragma once
#include "JuceHeader.h"

/**
 * @file RelativeLayoutManager.h
 * 
 * @brief Arranges components in an arbitrary bounding rectangle.
 * 
 * Components are arranged into rows and resized to fit the available space,
 * with weight values determining their sizes in proportion to each other.
 * 
 * Each row in the layout has a vertical weight value, and each component in
 * each row has a horizontal weight value.  Rows and their components are
 * arranged in order within the layout bounds, without overlapping.  The
 * fraction of the layout to assign to each component is calculated based on
 * the ratio of the item's weight to the sum of weight values. Specifically,
 * 
 *  (row height) = (layout height) * (row vertical weight) 
 *          / (sum of all vertical weights)
 * 
 *  (component width) = (layout width) * (component weight) 
 *          / (sum of all horizontal weights in the row)
 */

/*
* Layout Declaration Example:
 -----------------------

using RowItem = RelativeLayoutManager::ComponentLayout;
Layout layout(
{
    {.weight = 1, .rowItems = {}},
    {.weight = 3, .rowItems = {
            RowItem(),         // empty space, weight 1
            RowItem(cpA, 10),  // Component* cpA, weight 10
            RowItem(cpB),      // Component* cpB, weight 1
            RowItem(5)         // empty space, weight 5
        }
    },
    {.weight = 1, .rowItems = { RowItem(cpC) }}
 });
 layout.setXPaddingWeight(1);
 layout.setYPaddingWeight(2);
 layout.setXMarginFraction(1/20);
 layout.setYMarginWeight(2);
 
 -----------------------
 * Resulting layout:
 -----------------------
 * A = Component* cpA
 * B = Component* cpB
 * C = Component* cpC
 * . = margin space
 * _ = empty space
 -----------------------
  
  ....................
  ....................
  .__________________.
  ._AAAAAAAAAA_B_____.
  ._AAAAAAAAAA_B_____.
  ._AAAAAAAAAA_B_____.
  .__________________.
  .__________________.
  .CCCCCCCCCCCCCCCCCC.
  ....................
  ....................
 
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
        ComponentLayout(Component* component, int weight = 1) :
        component(component), weight(weight) { }
        
        
        ComponentLayout(int weight = 1) : weight(weight) { } 
        /**
         * Points to a component in the layout, or nullptr to add an empty space
         * in the layout.
         */
        Component* component = nullptr;
        /**
         * Component width = total width * weight / xWeightSum[rowNumber]
         */
        int weight;
    };

    /**
     * Defines one row of components in a RelativeLayout
     */
    struct RowLayout
    {
        /**
         * Row height = total height * weight / yWeightSum
         */
        int weight;
        /**
         * Holds all ComponentLayouts in the row.
         */
        std::vector<ComponentLayout> rowItems;
    };

    class Layout
    {
    public:
        
        /**
         * @param rows  The rows of components arranged in this Layout
         */
        Layout(std::vector<RowLayout> rows = std::vector<RowLayout>()) : 
        rows(rows) { }
        
        /**
         *  Sets the size of the top and bottom margins of the layout using a
         * weight value.  Each margin's share of the layout's vertical space
         * will be equal to weight divided by the sum of all margin and row
         * weights.  
         * 
         *  Setting the Y-margin weight resets the Y-margin fraction to
         * zero. Margins can be defined by a weight or a fraction, but not both.
         * 
         * @param weight  Both vertical margins will have this assigned as their
         *                weight.
         */
        void setYMarginWeights(const int weight);
        
        /**
         *  Sets the size of the left and right margins of the layout as a
         * fraction of the total layout width.  Each horizontal margin will have
         * its width set as this fraction of the total layout width.
         * 
         * @param fraction  The fraction of the total width to allocate to each
         *                  margin.
         */
        void setXMarginFraction(const float fraction);
        
        
        /**
         *  Sets the size of the top and bottom margins of the layout as a
         * fraction of the total layout height.  Each vertical margin will have
         * its height set as this fraction of the total layout height.
         * 
         *  Setting the Y-margin fraction resets the Y-margin weight to
         * zero. Margins can be defined by a weight or a fraction, but not both.
         * 
         * @param fraction  The fraction of the total height to allocate to each
         *                  margin.
         */
        void setYMarginFraction(const float fraction);
        
        /**
         *  Sets the amount of space to leave between all non-null components
         * in each row, as a fraction of the total layout width.
         *   
         *  Setting the X-padding fraction resets the X-padding weight to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param fraction  The amount of horizontal space to leave between each
         *                  non-null component in the layout, as a fraction of
         *                  the total layout width.
         */
        void setXPaddingFraction(const float fraction);
               
        /**
         *  Sets the amount of space to leave between all non-empty rows in the
         * layout, as a fraction of the total layout height.
         *   
         *  Setting the Y-padding fraction resets the Y-padding weight to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param fraction  The amount of vertical space to leave between each
         *                  non-empty row in the layout, as a fraction of
         *                  the total layout height.
         */
        void setYPaddingFraction(const float fraction);
                 
        /**
         *  Sets the amount of space to leave between all non-null components
         * in each row using a weight value. 
         *   
         *  Setting the X-padding weight resets the X-padding fraction to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param weight  The weight value to assign to each space between
         *                non-null components.  
         */
        void setXPaddingWeight(const int weight);
        
                         
        /**
         *  Sets the amount of space to leave between all non-empty rows in the
         * layout using a weight value. 
         *   
         *  Setting the Y-padding weight resets the Y-padding fraction to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param weight  The weight value to assign to each space between
         *                non-empty rows.  
         */
        void setYPaddingWeight(int weight);
        
        /**
         * Gets the fraction of the width to allocate to each horizontal margin.
         * 
         * @return The size of each horizontal margin, as a fraction of total
         *         layout width.
         */
        float getXMarginFraction() const;
        
        
        /**
         * Gets the fraction of the height to allocate to each vertical margin.
         * 
         * @return The size of each vertical margin, as a fraction of total
         *         layout height, or zero if the vertical margins are defined
         *         using a weight value.
         */
        float getYMarginFraction() const;
        
        /**
         * Gets the weight value assigned to each vertical margin.
         * 
         * @return  The weight value allocated to each vertical margin, or zero
         *          if margins are explicitly defined as a specific fraction of 
         *          the layout height.
         */
        int getYMarginWeight() const;
        
        /**
         * Gets the fraction of the width to allocate to each space between
         * non-null components.
         * 
         * @return  The width fraction to allocate to each padding section, or
         *          zero if horizontal padding is set with a weight value. 
         */
        float getXPaddingFraction() const;
        
        /**
         * Gets the weight value assigned to each horizontal space between
         * non-null layout components.
         * 
         * @return  The weight value to allocate to each horizontal padding 
         *          section.
         */
        int getXPaddingWeight() const;
        
        /**
         * Gets the fraction of the height to allocate to each space between
         * non-empty rows.
         * 
         * @return  The height fraction to allocate to each padding section, or
         *          zero if vertical padding is set with a weight value. 
         */
        float getYPaddingFraction() const;
                    
        /**
         * Gets the weight value assigned to each vertical space between
         * non-empty layout rows.
         * 
         * @return  The weight value to allocate to each padding section
         */
        int getYPaddingWeight() const;
        
        /**
         * Gets the list of all rows in the layout.
         * 
         * @return  the layout's component rows.
         */
        const std::vector<RowLayout>& getRows() const;
        
        /**
         * Inserts a new row into the layout at a specific index.
         * 
         * @param row    The new layout row to add.
         * 
         * @param index  The index where the row will be inserted. Any rows
         *               already located at that index or greater will be pushed
         *               forward.  If the index value is invalid, the closest
         *               valid index will be used.
         */
        void insertRow(const RowLayout row, int index);
        
        /**
         * Adds a new row to the end of the layout.
         * 
         * @param row  The new layout row to add.
         */
        void addRow(const RowLayout row);
    private:
        
        //Margin Sizes:
        //Horizontal margin cannot be defined by a weight value because each
        //row in the layout can have a different horizontal weight sum.
        float xMarginFraction = 0;
        
        int yMarginWeight = 0;
        float yMarginFraction = 0;
        
        //Padding Sizes:
        //defines the amount of horizontal space between non-null components
        int xPaddingWeight = 0;
        float xPaddingFraction = 0;
        
        //defines the amount of vertical space between non-empty rows
        int yPaddingWeight = 0;        
        float yPaddingFraction = 0;
        
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
