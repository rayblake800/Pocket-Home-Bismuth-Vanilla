#include "LayoutManager.h"

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

        //add padding weights between non-empty rows
        if (rowNum > 0 && !rowLayout.isEmpty()
            && layout.getRow(rowNum - 1).itemCount() > 0)
        {
            yWeightSum += layout.getYPaddingWeight();
        }

        xWeightSums.set(rowNum, 0);
        for (int cNum = 0; cNum < rowLayout.itemCount(); cNum++)
        {
            const RowItem& rowItem = rowLayout.getRowItem(cNum);
            xWeightSums.getReference(rowNum) += rowItem.getWeight();

            //add padding weights between non-null components
            if (cNum > 0 && layout.getXPaddingWeight() > 0
                && !rowItem.isEmpty()
                && !rowLayout.getRowItem(cNum).isEmpty())
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
void LayoutManager::layoutComponents(const Rectangle<int>& bounds)
{
    int xMarginFraction = layout.getXMarginFraction() * bounds.getWidth();
    int yMarginFraction = layout.getYMarginFraction() * bounds.getHeight();
    int xPaddingFraction = layout.getXPaddingFraction() * bounds.getWidth();
    int yPaddingFraction = layout.getYPaddingFraction() * bounds.getHeight();
    int yPaddingCount = 0;
    for (int i = 1; i < layout.rowCount(); i++)
    {
        if (!layout.getRow(i - 1).isEmpty()
            && !layout.getRow(i).isEmpty())
        {
            yPaddingCount++;
        }
    }

    int weightedHeight = 0;
    if (yWeightSum > 0)
    {
        weightedHeight = bounds.getHeight() - yMarginFraction * 2
                         - yPaddingFraction * yPaddingCount;
    }
    int yPos = bounds.getY() + yMarginFraction;
    if(yWeightSum > 0)
    {
        yPos += layout.getYMarginWeight() * weightedHeight / yWeightSum;
    }
    int yPaddingSize = bounds.getHeight() * yPaddingFraction;
    if (yPaddingSize == 0 && yWeightSum > 0)
    {
        yPaddingSize = layout.getYPaddingWeight() * weightedHeight / yWeightSum;
    }
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        if (rowNum > 0 && !row.isEmpty()
            && !layout.getRow(rowNum - 1).isEmpty())
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
        for (int i = 1; i < row.itemCount(); i++)
        {
            if (!row.getRowItem(i).isEmpty()
                && !row.getRowItem(i - 1).isEmpty())
            {
                xPaddingCount++;
            }
        }
        int weightedWidth = bounds.getWidth() - xMarginFraction * 2
                             - xPaddingFraction * xPaddingCount;
        int xPos = bounds.getX() + xMarginFraction;
        int xPaddingSize = bounds.getWidth() * xPaddingFraction;
        if (xPaddingSize == 0 && xWeightSum > 0)
        {
            xPaddingSize = layout.getXPaddingWeight() * weightedWidth 
                    / xWeightSum;
        }
        for (int cNum = 0; cNum < row.itemCount(); cNum++)
        {
            const RowItem& rowItem = row.getRowItem(cNum);
            if (cNum > 0 && !rowItem.isEmpty()
                && !row.getRowItem(cNum - 1).isEmpty())
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
                rowItem.getComponent()->setBounds(xPos, yPos, width, height);
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

//######################## LayoutManager::Layout ###############################

/*
 *  Sets the size of the top and bottom margins of the layout using a
 * weight value.
 */
void LayoutManager::Layout::setYMarginWeights(const unsigned int weight)
{
    yMarginWeight = weight;
    yMarginFraction = 0;
}

/*
 *  Sets the size of the left and right margins of the layout as a
 * fraction of the total layout width.
 */
void LayoutManager::Layout::setXMarginFraction(const float fraction)
{
    xMarginFraction = fraction;
}

/*
 *  Sets the size of the top and bottom margins of the layout as a
 * fraction of the total layout height.
 */
void LayoutManager::Layout::setYMarginFraction(const float fraction)
{
    yMarginFraction = fraction;
    yMarginWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-null components
 * in each row, as a fraction of the total layout width.
 */
void LayoutManager::Layout::setXPaddingFraction(const float fraction)
{
    xPaddingFraction = fraction;
    xPaddingWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-empty rows in the
 * layout, as a fraction of the total layout height.
 */
void LayoutManager::Layout::setYPaddingFraction(const float fraction)
{
    yPaddingFraction = fraction;
    yPaddingWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-null components
 * in each row using a weight value. 
 */
void LayoutManager::Layout::setXPaddingWeight(const unsigned int weight)
{
    xPaddingWeight = weight;
    xPaddingFraction = 0;
}

/*
 *  Sets the amount of space to leave between all non-empty rows in the
 * layout using a weight value.  
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
 * non-null components.
 */
float LayoutManager::Layout::getXPaddingFraction() const
{
    return xPaddingFraction;
}

/*
 * Gets the weight value assigned to each horizontal space between
 * non-null layout components.
 */
unsigned int LayoutManager::Layout::getXPaddingWeight() const
{
    return xPaddingWeight;
}

/*
 * Gets the fraction of the height to allocate to each space between
 * non-empty rows.
 */
float LayoutManager::Layout::getYPaddingFraction() const
{
    return yPaddingFraction;
}

/*
 * Gets the weight value assigned to each vertical space between
 * non-empty layout rows.
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

#if JUCE_DEBUG

/**
 * Print out the layout to the console for debugging
 */
void LayoutManager::printLayout()
{
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        DBG("LayoutManager::" << __func__ << ":");
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
