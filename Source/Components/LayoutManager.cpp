#include "LayoutManager.h"

/*
 * Gets the current component layout held by this LayoutManager.
 */
LayoutManager::Layout LayoutManager::getLayout() const
{
    return layout;
}

/*
 * Set a new Component layout, removing all old layout definitions.
 */
void LayoutManager::setLayout
(const Layout& layout, Component* parentToInit)
{
    clearLayout();
    this->layout = layout;
    yWeightSum = layout.getYMarginWeight() * 2;
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& rowLayout = layout.getRow(rowNum);
        yWeightSum += rowLayout.getWeight();

        //add padding weights between rows with non-zero weight
        if (rowNum > 0 && yWeightSum > 0 &&  rowLayout.getWeight() > 0)
        {
            yWeightSum += layout.getYPaddingWeight();
        }

        xWeightSums.set(rowNum, 0);
        for (int cNum = 0; cNum < rowLayout.itemCount(); cNum++)
        {
            const RowItem& rowItem = rowLayout.getRowItem(cNum);
            xWeightSums.getReference(rowNum) += rowItem.getWeight();

            //add padding weights between row items with non-zero weight
            if (cNum > 0 && !rowItem.getWeight() == 0
                && xWeightSums[rowNum] > 0)
            {
                xWeightSums.getReference(rowNum) += layout.getXPaddingWeight();
            }
            if (parentToInit != nullptr)
            {
                parentToInit->addAndMakeVisible(rowItem.getComponent());
            }
        }
    }
}


/*
 * Changes the current layout, and immediately applies the updated layout
 * to all components in the layout, optionally animating the transition.
 */
void LayoutManager::transitionLayout(const LayoutManager::Layout& newLayout,
        Component* parent,
        const TransitionAnimator::Transition transition,
        const unsigned int duration)
{
    if(transition == TransitionAnimator::none || duration == 0)
    {
        clearLayout(true);
        setLayout(newLayout, parent);
        layoutComponents(parent->getLocalBounds());
        return;
    }
    //Transition out old layout items
    for (int rNum = 0; rNum < layout.rowCount(); rNum++)
    {
        const Row& row = layout.getRow(rNum);
        for (int i = 0; i < row.itemCount(); i++)
        {
            const RowItem& rowItem = row.getRowItem(i);
            if (!rowItem.isEmpty())
            {
                TransitionAnimator::transitionOut(rowItem.getComponent(),
                        transition, duration, true);
            }
        }
    }
    clearLayout(true);
    //Transition in new layout items
    setLayout(newLayout, parent);
    layoutComponents(parent->getLocalBounds(), transition, duration);
}

/*
 * Adds all components in the layout to a parent component, and makes them
 * all visible.
 */
void LayoutManager::addComponentsToParent(Component* parent)
{
    for (int rNum = 0; rNum < layout.rowCount(); rNum++)
    {
        const Row& row = layout.getRow(rNum);
        for (int i = 0; i < row.itemCount(); i++)
        {
            const RowItem& rowItem = row.getRowItem(i);
            if (!rowItem.isEmpty())
            {
                parent->addAndMakeVisible(rowItem.getComponent());
            }
        }
    }
}

/*
 * Arranges the components within a bounding rectangle.
 */
void LayoutManager::layoutComponents(const Rectangle<int>& bounds,
            const TransitionAnimator::Transition transition,
            const unsigned int duration)
{
    int xMarginSize = layout.getXMarginFraction() * bounds.getWidth();
    int yMarginSize = layout.getYMarginFraction() * bounds.getHeight();
    int yPaddingSize = layout.getYPaddingFraction() * bounds.getHeight();
    int yPaddingCount = 0;
    bool nonzeroRowFound = false;
    for (int i = 0; i < layout.rowCount(); i++)
    {
        if(layout.getRow(i).getWeight() > 0)
        {
            if(nonzeroRowFound)
            {
                yPaddingCount++;
            }
            else
            {
                nonzeroRowFound = true;
            }
        }
    }

    int weightedHeight = 0;
    if (yWeightSum > 0)
    {
        weightedHeight = bounds.getHeight() - yMarginSize * 2
                         - yPaddingSize * yPaddingCount;
    }
    int yStart = bounds.getY() + yMarginSize;
    if(yWeightSum > 0)
    {
        yStart += layout.getYMarginWeight() * weightedHeight / yWeightSum;
    }
    if (yPaddingSize == 0 && yWeightSum > 0)
    {
        yPaddingSize = layout.getYPaddingWeight() * weightedHeight / yWeightSum;
    }
    int yPos = yStart;
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        if (row.getWeight() > 0 && yPos != yStart)
        {
            yPos += yPaddingSize;
        }

        int height = 0;
        if(yWeightSum > 0)
        {
            height = row.getWeight() * weightedHeight / yWeightSum;
        }
        const int& xWeightSum = xWeightSums[rowNum];
        int xPaddingCount = 0;
        bool nonzeroItmFound = false;
        for (int i = 0; i < row.itemCount(); i++)
        {
            if (row.getRowItem(i).getWeight() > 0)
            {
                if(nonzeroItmFound)
                {
                    xPaddingCount++;
                }
                else
                {
                    nonzeroItmFound = true;
                }
            }
        }
        int xPaddingSize = layout.getXPaddingFraction() * bounds.getWidth();
        int weightedWidth = bounds.getWidth() - xMarginSize * 2
                             - xPaddingSize * xPaddingCount;
        if (xPaddingSize == 0 && xWeightSum > 0)
        {
            xPaddingSize = layout.getXPaddingWeight() * weightedWidth 
                    / xWeightSum;
        }
        int xStart = bounds.getX() + xMarginSize;
        int xPos = xStart;
        for (int cNum = 0; cNum < row.itemCount(); cNum++)
        {
            const RowItem& rowItem = row.getRowItem(cNum);
            if (rowItem.getWeight() > 0 && xPos != xStart)
            {
                xPos += xPaddingSize;
            }
            int width = 0;
            if(xWeightSum > 0)
            {
                width = rowItem.getWeight() * weightedWidth / xWeightSum;
            }
            if (!rowItem.isEmpty())
            {
                TransitionAnimator::transitionIn(rowItem.getComponent(),
                        transition,
                        Rectangle<int>(xPos, yPos, width, height),
                        duration);
            }
            xPos += width;
        }
        yPos += height;
    }
}

/*
 * Remove all saved component layout parameters
 */
void LayoutManager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (int rNum = 0; rNum < layout.rowCount(); rNum++)
        {
            const Row& row = layout.getRow(rNum);
            for (int i = 0; i < row.itemCount(); i++)
            {
                const RowItem& rowItem = row.getRowItem(i);
                if (!rowItem.isEmpty())
                {
                    Component * parent = rowItem.getComponent()
                            ->getParentComponent();
                    if (parent != nullptr)
                    {
                        parent->removeChildComponent
                                (rowItem.getComponent());
                    }
                }
            }
        }
    }
    layout = Layout();
    xWeightSums.clear();
    yWeightSum = 0;
}

//####################### LayoutManager::RowItem ###############################   

/*
 * Gets the component assigned to this RowItem. 
 */
Component* LayoutManager::RowItem::getComponent() const
{
    return component;
}

/*
 * Gets the horizontal weight value assigned to this RowItem.
 */
unsigned int LayoutManager::RowItem::getWeight()const
{
    return weight;
}

/*
 * Checks if this row item represents an empty space.
 */
bool LayoutManager::RowItem::isEmpty() const
{
    return component == nullptr;
}
 
/*
 * Checks if this RowItem and another are equivalent. RowItems are 
 * equivalent if they have the same weight and Component.
 */
bool LayoutManager::RowItem::operator==(const LayoutManager::RowItem& rhs) const
{
    return component == rhs.component && weight == rhs.weight;
}

/*
 * Checks if this RowItem and another are not equivalent.
 */
bool LayoutManager::RowItem::operator!=(const LayoutManager::RowItem& rhs) const
{
    return !(*this == rhs);
}

//######################### LayoutManager::Row #################################

/*
 * Adds a new RowItem to the end of this row.
 */
void LayoutManager::Row::addRowItem(const LayoutManager::RowItem rowItem)
{
    rowItems.push_back(rowItem);
}

/*
 * Gets the number of rowItems in the row.
 */
unsigned int LayoutManager::Row::itemCount() const
{
    return rowItems.size();
}

/*
 * Checks if this row is empty.
 */
bool LayoutManager::Row::isEmpty() const
{
    return rowItems.empty();
}

/*
 * Gets a RowItem from the row.
 */
const LayoutManager::RowItem& LayoutManager::Row::getRowItem
(const unsigned int index) const
{
    return rowItems.at(index);
}

/*
 * Gets the weight assigned to this row.
 */
unsigned int LayoutManager::Row::getWeight() const
{
    return weight;
}
/*
 * Checks if this Row and another Row are equivalent.  Rows are
 * equivalent if they have the same vertical weight and each RowItem
 * in this Row is equivalent to another row item at the same index
 * in the other row.
 */
bool LayoutManager::Row::operator==(const LayoutManager::Row& rhs) const
{
    if(weight != rhs.weight 
            || rowItems.size() != rhs.rowItems.size())
    {
        return false;
    }
    for(int i = 0; i < rowItems.size(); i++)
    {
        if(rowItems[i] != rhs.rowItems[i])
        {
            return false;
        }
    }
    return true;
}

/*
 * Checks if this Row and another Row are not equivalent.
 */
bool LayoutManager::Row::operator !=(const LayoutManager::Row& rhs) const
{
    return !(*this == rhs);
}

//######################## LayoutManager::Layout ###############################

/*
 * Sets the size of the top and bottom margins of the layout using a
 * weight value.
 */
void LayoutManager::Layout::setYMarginWeights(const unsigned int weight)
{
    yMarginWeight = weight;
    yMarginFraction = 0;
}

/*
 * Sets the size of the left and right margins of the layout as a
 * fraction of the total layout width.
 */
void LayoutManager::Layout::setXMarginFraction(const float fraction)
{
    xMarginFraction = fraction;
}

/*
 * Sets the size of the top and bottom margins of the layout as a
 * fraction of the total layout height.
 */
void LayoutManager::Layout::setYMarginFraction(const float fraction)
{
    yMarginFraction = fraction;
    yMarginWeight = 0;
}

/*
 * Sets the fraction of the total layout width to leave empty between
 * all row items with non-zero weights.
 */
void LayoutManager::Layout::setXPaddingFraction(const float fraction)
{
    xPaddingFraction = fraction;
    xPaddingWeight = 0;
}

/*
 *  Sets the fraction of the total layout height to leave empty
 * between all layout rows with non-zero weights.
 */
void LayoutManager::Layout::setYPaddingFraction(const float fraction)
{
    yPaddingFraction = fraction;
    yPaddingWeight = 0;
}

/*
 *  Assigns a weight value to the amount of empty space left between
 * each row item, ignoring row items with a zero weight value.
 */
void LayoutManager::Layout::setXPaddingWeight(const unsigned int weight)
{
    xPaddingWeight = weight;
    xPaddingFraction = 0;
}

/*
 *  Assigns a weight value to the amount of empty space left between
 * each row, ignoring rows with a zero weight value.
 */
void LayoutManager::Layout::setYPaddingWeight(const unsigned int weight)
{
    yPaddingWeight = weight;
    yPaddingFraction = 0;
}

/*
 * Gets the fraction of the width to allocate to each horizontal margin.
 */
float LayoutManager::Layout::getXMarginFraction() const
{
    return xMarginFraction;
}

/*
 * Gets the fraction of the height to allocate to each vertical margin.
 */
float LayoutManager::Layout::getYMarginFraction() const
{
    return yMarginFraction;
}

/*
 * Gets the weight value assigned to each vertical margin.
 */
unsigned int LayoutManager::Layout::getYMarginWeight() const
{
    return yMarginWeight;
}

/*
 * Gets the fraction of the width to allocate to each space between
 * RowItems.
 */
float LayoutManager::Layout::getXPaddingFraction() const
{
    return xPaddingFraction;
}

/*
 * Gets the weight value assigned to each horizontal space between
 * layout rows.
 */
unsigned int LayoutManager::Layout::getXPaddingWeight() const
{
    return xPaddingWeight;
}

/*
 * Gets the fraction of the height to allocate to each space between
 * layout rows.
 */
float LayoutManager::Layout::getYPaddingFraction() const
{
    return yPaddingFraction;
}

/*
 * Gets the weight value assigned to each vertical space between
 * layout rows.
 */
unsigned int LayoutManager::Layout::getYPaddingWeight() const
{
    return yPaddingWeight;
}

/*
 * Gets the number of rows in the layout.
 */
unsigned int LayoutManager::Layout::rowCount() const
{
    return rows.size();
}

/*
 * Gets a row in the layout.
 */
const LayoutManager::Row& LayoutManager::Layout::getRow
(const unsigned int index) const
{
    return rows.at(index);
}

/*
 * Inserts a new row into the layout at a specific index.
 */
void LayoutManager::Layout::insertRow
(const LayoutManager::Row row, const unsigned int index)
{
    int iterIdx = 0;
    auto rowIter = rows.begin();
    while (rowIter != rows.end() && iterIdx < index)
    {
        iterIdx++;
        rowIter++;
    }
    rows.insert(rowIter, row);
}

/*
 * Adds a new row to the end of the layout.
 */
void LayoutManager::Layout::addRow
(const LayoutManager::Row row)
{
    rows.push_back(row);
}

/*
 * Checks if this layout and another are equivalent.  Layouts are 
 * equivalent if they have the same margin and padding values, and for
 * each row index, their rows are equivalent.
 */
bool LayoutManager::Layout::operator==(const LayoutManager::Layout& rhs) const
{
    if(yPaddingFraction != rhs.yPaddingFraction
            || yPaddingWeight   != rhs.yPaddingWeight
            || xPaddingFraction != rhs.xPaddingFraction
            || xPaddingWeight   != rhs.xPaddingWeight
            || yMarginFraction  != rhs.yMarginFraction
            || yMarginWeight    != rhs.yMarginWeight
            || xMarginFraction  != rhs.xMarginFraction
            || rows.size()      != rhs.rows.size())
    {
        return false;
    }
    for(int i = 0; i < rows.size(); i++)
    {
        if(rows[i] != rhs.rows[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * Checks if this layout and another are not equivalent.
 */
bool LayoutManager::Layout::operator!=(const LayoutManager::Layout& rhs) const
{
    return !(*this == rhs);
}

#if JUCE_DEBUG

/**
 * Print out the layout to the console for debugging
 */
void LayoutManager::printLayout()
{
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        DBG(String("Row weight:") + String(row.getWeight()) + String("/")
                + String(yWeightSum));
        String rowStr = "\t";
        for (int i = 0; i < row.itemCount(); i++)
        {
            const RowItem& rowItem = row.getRowItem(i);
            if (rowItem.isEmpty())
            {
                rowStr += "null";
            }
            else
            {
                rowStr += rowItem.getComponent()->getName();
            }
            rowStr += "(";
            rowStr += String(rowItem.getWeight());
            rowStr += "/";
            rowStr += String(xWeightSums[rowNum]);
            rowStr += ") ";
        }
        DBG(rowStr);
    }
    DBG("");
}

#endif
