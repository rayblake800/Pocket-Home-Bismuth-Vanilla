#pragma once
#include "TransitionAnimator.h"
#include "JuceHeader.h"

/**
 * @file LayoutManager.h
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
 *
 *
 * Layout Declaration Example:
 * -----------------------
 *
 * using Row     = LayoutManager::Row;
 * using RowItem = LayoutManager::RowItem;
 * Layout layout(
 * {
 *    Row(1),
 *    Row(3,
 *    {
 *        RowItem(),         // empty space, weight 1
 *        RowItem(cpA, 10),  // Component* cpA, weight 10
 *        RowItem(cpB),      // Component* cpB, weight 1
 *        RowItem(5)         // empty space, weight 5
 *    }),
 *    Row(1, { RowItem(cpC) })
 * });
 * layout.setXPaddingWeight(1);
 * layout.setYPaddingWeight(2);
 * layout.setXMarginFraction(1/20);
 * layout.setYMarginWeight(2);
 *  
 * -----------------------
 * Resulting layout:
 * -----------------------
 * A = Component* cpA
 * B = Component* cpB
 * C = Component* cpC
 * . = margin space
 * | = vertical padding
 * - = horizontal padding
 * _ = empty space
 * -----------------------
 *   
 *   ....................
 *   ....................
 *   .__________________.
 *   .||||||||||||||||||.
 *   .||||||||||||||||||.
 *   ._AAAAAAAAAA-B_____.
 *   ._AAAAAAAAAA-B_____.
 *   ._AAAAAAAAAA-B_____.
 *   .||||||||||||||||||.
 *   .||||||||||||||||||.
 *   .CCCCCCCCCCCCCCCCCC.
 *   ....................
 *   ....................
 *  
 */

class LayoutManager
{
public:

    LayoutManager() { }

    virtual ~LayoutManager() { }

    /**
     * @brief  Defines one component's space in a LayoutManager row.
     */
    class RowItem
    {
    public:
        /**
         * @brief  Creates a RowItem, initializing the item's Component and
         *         weight.
         *
         * @param component  A component to place in the layout, or nullptr
         *                   to add an empty space.
         * 
         * @param weight     Sets this row item's width, relative to the other
         *                   RowItems in the row.
         */
        RowItem(juce::Component* component, const unsigned int weight = 1) :
        component(component), weight(weight) { }
             
        /**
         * @brief  Creates a RowItem with no Component and a specific weight.
         *
         * @param weight     Sets this row item's width, relative to the other
         *                   RowItems in the row.
         */
        RowItem(const unsigned int weight = 1) : weight(weight) { }
        
        /**
         * @brief  Gets the component assigned to this RowItem. 
         * 
         * @return  The Component* assigned to this RowItem on creation.  This
         *          value may be null.
         */
        juce::Component* getComponent() const;
        
        /**
         * @brief  Gets the horizontal weight value assigned to this RowItem.
         * 
         * @return  The item's weight value. 
         */
        unsigned int getWeight() const;
        
        /**
         * @brief  Checks if this row item represents an empty space.
         * 
         * @return   Whether this row item has no component.
         */
        bool isEmpty() const;
        
        /**
         * @brief  Checks if this RowItem and another are equivalent. RowItems 
         *         are equivalent if they have the same weight and Component.
         *
         * @param rhs  The row item being compared to this one.
         *
         * @return     Whether this row item and rhs are equivalent.
         */
        bool operator==(const RowItem& rhs) const;

        /**
         * @brief  Checks if this RowItem and another are not equivalent.
         *
         * @param rhs  Another RowItem being compared to this one.
         *
         * @return     Whether this row item and rhs are not equivalent.
         */
        bool operator!=(const RowItem& rhs) const;

    private:
        /**
         * @brief  Points to a component in the layout, or nullptr to represent 
         *         an empty space in the layout.
         */
        juce::Component* component = nullptr;

        /**
         * Component width = total width * weight / xWeightSum[rowNumber]
         */
        unsigned int weight;
    };

    /**
     * @brief  Defines one row of components in a LayoutManager
     */
    class Row
    {
    public:
        /**
         * @brief  Creates a new layout Row, optionally with an initial weight
         *         and list of row items.
         *
         * @param weight    Sets this row's height relative to other rows in
         *                  the layout.
         * 
         * @param rowItems  The list of all RowItems arranged in this row.
         */
        Row(const unsigned int weight = 1, std::vector<RowItem> rowItems = {}) :
        weight(weight), rowItems(rowItems) { }
        
        /**
         * @brief  Adds a new RowItem to the end of this row.
         *  
         * @param rowItem  The RowItem to add to the row.
         */
        void addRowItem(const RowItem rowItem);
        
        /**
         * @brief  Gets the number of RowItems in the row.
         * 
         * @return  The number of components/empty spaces in the row layout.
         */
        unsigned int itemCount() const;
        
        /**
         * @brief  Checks if this row is empty.
         * 
         * @return  Whether this row contains no row items. 
         */
        bool isEmpty() const;
        
        /**
         * @brief  Gets a RowItem from the row.
         * 
         * @param index              The index of the row item within the row.
         * 
         * @return                   The row item at the given index.
         * 
         * @throw std::out_of_range  If the index is invalid. 
         */
        const RowItem& getRowItem(const unsigned int index) const;
        
        /**
         * @brief  Gets the weight assigned to this row.
         * 
         * @return  The weight value used to set the row's height.
         */
        unsigned int getWeight() const;
     
        /**
         * @brief  Checks if this Row and another Row are equivalent.  
         *
         * Rows are equivalent if they have the same vertical weight and each 
         * RowItem in this Row is equivalent to another row item at the same 
         * index in the other row.
         *
         * @param rhs  The other row being compared to this one.
         *
         * @return     Whether this row and rhs are equivalent.
         */
        bool operator==(const Row& rhs) const;

        /**
         * Checks if this Row and another Row are not equivalent.
         *
         * @param rhs  The other Row being compared with this row.
         *
         * @return     True iff this Row and rhs are not equivalent.
         */
        bool operator !=(const Row& rhs) const;   

    private:      
        /**
         * Row height = total height * weight / yWeightSum
         */
        unsigned int weight;

        /**
         * Holds all ComponentLayouts in the row.
         */
        std::vector<RowItem> rowItems;
    };

    class Layout
    {
    public:
        
        /**
         * @param rows  The rows of components arranged in this Layout.
         */
        Layout(std::vector<Row> rows = std::vector<Row>()) : 
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
        void setYMarginWeights(const unsigned int weight);
        
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
         *  Sets the fraction of the total layout width to leave empty between
	 * all row items with non-zero weights.
         *   
         *  Setting the X-padding fraction resets the X-padding weight to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param fraction  The amount of horizontal space to leave between each
         *                  item in the row with nonzero weight, as a fraction
	 *                  of the total layout width.
         */
        void setXPaddingFraction(const float fraction);
               
        /**
         *  Sets the fraction of the total layout height to leave empty
	 * between all layout rows with non-zero weights.
         *   
         *  Setting the Y-padding fraction resets the Y-padding weight to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param fraction  The amount of vertical space to leave between each
         *                  nonzero row in the layout, as a fraction of the 
	 *                  total layout height.
         */
        void setYPaddingFraction(const float fraction);
                 
        /**
         *  Assigns a weight value to the amount of empty space left between
	 * each row item, ignoring row items with a zero weight value.
         *   
         *  Setting the X-padding weight resets the X-padding fraction to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param weight  The weight value to assign to each space between
         *                row items.  
         */
        void setXPaddingWeight(const unsigned int weight);
        
                         
        /**
         *  Assigns a weight value to the amount of empty space left between
	 * each row, ignoring rows with a zero weight value.
         *   
         *  Setting the Y-padding weight resets the Y-padding fraction to
         * zero. Padding can be defined by a weight or a fraction, but not both.
         * 
         * @param weight  The weight value to assign to each space between
         *                rows.  
         */
        void setYPaddingWeight(const unsigned int weight);
        
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
        unsigned int getYMarginWeight() const;
        
        /**
         * Gets the fraction of the width to allocate to each space between
         * RowItems.
         * 
         * @return  The width fraction to allocate to each padding section, or
         *          zero if horizontal padding is set with a weight value. 
         */
        float getXPaddingFraction() const;
        
        /**
         * Gets the weight value assigned to each horizontal space between
         * RowItems.
         * 
         * @return  The weight value to allocate to each horizontal padding 
         *          section.
         */
        unsigned int getXPaddingWeight() const;
        
        /**
         * Gets the fraction of the height to allocate to each space between
         * layout rows.
         * 
         * @return  The height fraction to allocate to each padding section, or
         *          zero if vertical padding is set with a weight value. 
         */
        float getYPaddingFraction() const;
                    
        /**
         * Gets the weight value assigned to each vertical space between
         * layout rows.
         * 
         * @return  The weight value to allocate to each padding section
         */
        unsigned int getYPaddingWeight() const;
        
        /**
         * Gets the number of rows in the layout.
         * 
         * @return the number of rows. 
         */
        unsigned int rowCount() const;
        
        /**
         * Checks if the layout contains any rows.
         * 
         * @return  True iff the layout contains zero rows.  Note that this will
         *          return false even if all rows are empty or have a weight of
         *          zero. 
         */
        bool isEmpty() const;
        
        /**
         * Gets a row in the layout.
         * 
         * @param index  The index of the row to get.
         * 
         * @return  The row at the given index.
         * 
         * @throw   std::out_of_range if index is not a valid row index.
         */
        const Row& getRow(const unsigned int index) const;
        
        /**
         * Inserts a new row into the layout at a specific index.
         * 
         * @param index  The index where the row will be inserted. Any rows
         *               already located at that index or greater will be pushed
         *               forward.  If the index value is invalid, the closest
         *               valid index will be used.
         * 
         * @param row    The new layout row to add.
         */
        void insertRow(const unsigned int index, const Row row);
        
        /**
         * Replaces a row in the layout.
         * 
         * @param index  The index of the row that will be replaced.
         * 
         * @param row    The new row to add to the layout.
         * 
         * @throw   std::out_of_range if index is not a valid row index.
         */
        void setRow(const unsigned int index, const Row row);
        
        /**
         * Adds a new row to the end of the layout.
         * 
         * @param row  The new layout row to add.
         */
        void addRow(const Row row);

        /**
         * Checks if this layout and another are equivalent.  Layouts are 
         * equivalent if they have the same margin and padding values, and for
         * each row index, their rows are equivalent.
         *
         * @param rhs  Another Layout to compare to this Layout.
         *
         * @return     True iff this Layout and rhs are equivalent.
         */
        bool operator==(const Layout& rhs) const;

        /**
         * Checks if this layout and another are not equivalent.
         *
         * @param rhs  Another Layout to compare to this Layout
         *
         * @return     True iff this Layout and rhs are not equivalent.
         */
        bool operator!=(const Layout& rhs) const;
    private:
        
        //Margin Sizes:
        //Horizontal margin cannot be defined by a weight value because each
        //row in the layout can have a different horizontal weight sum.
        float xMarginFraction = 0;
        
        unsigned int yMarginWeight = 0;
        float yMarginFraction = 0;
        
        //Padding Sizes:
        //defines the amount of horizontal space between row items
        unsigned int xPaddingWeight = 0;
        float xPaddingFraction = 0;
        
        //defines the amount of vertical space between rows
        unsigned int yPaddingWeight = 0;        
        float yPaddingFraction = 0;
        
        /**
         * Holds the layouts of all component rows.
         */
        std::vector<Row> rows;
    };

    /**
     * Gets the current component layout held by this LayoutManager.
     * 
     * @return  A copy of the layout saved with setLayout().
     */
    Layout getLayout() const;
    
    /**
     * Set a new Component layout, removing all old layout definitions.
     * 
     * @param rows           Defines the position and weight of all components. 
     *                      Null components may be used to define empty spaces. 
     * 
     * @param parentToInit   If a non-null component pointer is provided, This 
     *                      will add all components in the layout as children 
     *                      of parentToInit and make them visible.
     */
    void setLayout
    (const Layout& layout, juce::Component* parentToInit = nullptr);
    
    /**
     * Changes the current layout, and immediately applies the updated layout
     * to all components in the layout, optionally animating the transition.
     *
     * @param newLayout    The new layout to apply.
     * 
     * @param parent       The parent component of all layout components.
     *                     All components in the old layout will be removed
     *                     from this component's children.  All components in
     *                     the new layout will be added to this component as
     *                     children.  Layout components will be placed within
     *                     this component's bounds.
     * 
     * @param transition   Optional transition animation to apply when updating
     *                     the layout.
     * 
     * @param duration     If animating the transition, this defines the
     *                     animation duration in milliseconds.
     */
    void transitionLayout(
            const Layout& newLayout,
            juce::Component* parent,
            const TransitionAnimator::Transition transition
                = TransitionAnimator::none,
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
            const TransitionAnimator::Transition transition
                = TransitionAnimator::none,
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
    //Current layout definition
    Layout layout;

    //Holds the sum of component weights for each row
    juce::Array<unsigned int> xWeightSums;

    //holds the sum of component row weights.
    unsigned int yWeightSum = 0;
};
