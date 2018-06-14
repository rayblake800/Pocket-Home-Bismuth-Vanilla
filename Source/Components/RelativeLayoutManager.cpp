#include "RelativeLayoutManager.h"

/*
 * Set a new Component layout, removing all old layout definitions.
 */
void RelativeLayoutManager::setLayout
(const Layout& layout, Component* parentToInit)
{
    clearLayout();
    this->layout = layout;
    yWeightSum = layout.getYMarginWeight() * 2;
    for (int rowNum = 0; rowNum < layout.getRows().size(); rowNum++)
    {
        const RowLayout& rowLayout = layout.getRows()[rowNum];
        yWeightSum += rowLayout.weight;
        
        //add padding weights between non-empty rows
        if(rowNum > 0 && layout.getYPaddingWeight() > 0 
           && rowLayout.rowItems.size() > 0
           && layout.getRows()[rowNum - 1].rowItems.size() > 0)
        {
            yWeightSum += layout.getYPaddingWeight();
        }
        
        xWeightSums[rowNum] = 0;
        for (int cNum = 0; cNum < rowLayout.rowItems.size(); cNum++)
        {
            const ComponentLayout& compLayout = rowLayout.rowItems[cNum];
            xWeightSums[rowNum] += compLayout.weight;
            
            //add padding weights between non-null components
            if(cNum > 0 && layout.getXPaddingWeight() > 0 
               && compLayout.component != nullptr
               && rowLayout.rowItems[cNum].component != nullptr)
            {
                xWeightSums[rowNum] += layout.getXPaddingWeight();
            }
            if (parentToInit != nullptr)
            {
                parentToInit->addAndMakeVisible(compLayout.component);
            }
        }
    }
}

/*
 * Adds all components in the layout to a parent component, and makes them
 * all visible.
 */
void RelativeLayoutManager::addComponentsToParent(Component* parent)
{
    for (RowLayout& row : layout.getRows())
    {
        for (ComponentLayout& compPtr : row.rowItems)
        {
            parent->addAndMakeVisible(compPtr.component);
        }
    }
}

/*
 * Arranges the components within a bounding rectangle.
 */
void RelativeLayoutManager::layoutComponents(Rectangle<int> bounds)
{
    int xMarginFraction = layout.getXMarginFraction() * bounds.getWidth();
    int yMarginFraction = layout.getYMarginFraction() * bounds.getHeight();
    int xPaddingFraction = layout.getXPaddingFraction() * bounds.getWidth();
    int yPaddingFraction = layout.getYPaddingFraction() * bounds.getHeight();

    int yPaddingCount = 0;
    for(int i = 1; i < layout.getRows().size(); i++)
    {
        if(!layout.getRows()[i].rowItems.empty()
                && !layout.getRows()[i].rowItems.empty())
        {
            yPaddingCount++;
        }
    }
    
    int yPixPerWeight = (bounds.getHeight() - yMarginFraction * 2 
            - yPaddingFraction * yPaddingCount) / yWeightSum ;
    int yPos = bounds.getY() 
            + yMarginFraction
            + layout.getYMarginWeight() * yPixPerWeight;
    int yPaddingSize = bounds.getHeight() * yPaddingFraction;
    if(yPaddingSize == 0)
    {
        yPaddingSize = layout.getYPaddingWeight() * yPixPerWeight;
    }
    for (int rowNum = 0; rowNum < layout.getRows().size(); rowNum++)
    {
        const RowLayout& row = layout.getRows()[rowNum];
        if(rowNum > 0 && !row.rowItems.empty()
                && !layout.getRows()[rowNum - 1].rowItems.empty())
        {
            yPos += yPaddingSize;
        }

        int height = row.weight * yPixPerWeight;
        const int& xWeightSum = xWeightSums[rowNum];
        int xPaddingCount = 0;
        for(int i = 1; i < row.rowItems.size(); i++)
        {
            if(row.rowItems[i - 1].component != nullptr
               && row.rowItems[i].component != nullptr)
            {
                xPaddingCount++;
            }
        }
        int xPixPerWeight = (bounds.getWidth() - xMarginFraction * 2
                - xPaddingFraction * xPaddingCount) / xWeightSum ;
        int xPos = bounds.getX() + xMarginFraction;
        int xPaddingSize = bounds.getWidth() * xPaddingFraction;
        if(xPaddingSize == 0)
        {
            xPaddingSize = layout.getXPaddingWeight() * xPixPerWeight;
        }
        for (int cNum = 0; cNum < row.rowItems.size(); cNum++)
        {
            const ComponentLayout& compLayout = row.rowItems[cNum];
            if(cNum > 0 && compLayout.component != nullptr
               && row.rowItems[cNum - 1].component != nullptr)
            {
                xPos += xPaddingSize;
            }
            int width = compLayout.weight * xPixPerWeight;
            if (compLayout.component != nullptr)
            {
                compLayout.component->setBounds(xPos, yPos, width, height);
            }
            xPos += width;
        }
        yPos += height;
    }
}

/*
 * Remove all saved component layout parameters
 */
void RelativeLayoutManager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (RowLayout& row : layout.getRows())
        {
            for (ComponentLayout& componentData : row.rowItems)
            {
                if (componentData.component != nullptr)
                {
                    Component * parent = componentData.component
                            ->getParentComponent();
                    if (parent != nullptr)
                    {
                        parent->removeChildComponent(componentData.component);
                    }
                }
            }
        }
    }
    layout = Layout();
    xWeightSums.clear();
    yWeightSum = 0;
}

/*
 *  Sets the size of the top and bottom margins of the layout using a
 * weight value.
 */
void RelativeLayoutManager::Layout::setYMarginWeights(const int weight)
{
    yMarginWeight = weight;
    yMarginFraction = 0;
}

/*
 *  Sets the size of the left and right margins of the layout as a
 * fraction of the total layout width.
 */
void RelativeLayoutManager::Layout::setXMarginFraction(const float fraction)
{
    xMarginFraction = fraction;
}

/*
 *  Sets the size of the top and bottom margins of the layout as a
 * fraction of the total layout height.
 */
void RelativeLayoutManager::Layout::setYMarginFraction(const float fraction)
{
    yMarginFraction = fraction;
    yMarginWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-null components
 * in each row, as a fraction of the total layout width.
 */
void RelativeLayoutManager::Layout::setXPaddingFraction(const float fraction)
{
    xPaddingFraction = fraction;
    xPaddingWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-empty rows in the
 * layout, as a fraction of the total layout height.
 */
void RelativeLayoutManager::Layout::setYPaddingFraction(const float fraction)
{
    yPaddingFraction = fraction;
    yPaddingWeight = 0;
}

/*
 *  Sets the amount of space to leave between all non-null components
 * in each row using a weight value. 
 */
void RelativeLayoutManager::Layout::setXPaddingWeight(const int weight)
{
    xPaddingWeight = weight;
    xPaddingFraction = 0;
}

/*
 *  Sets the amount of space to leave between all non-empty rows in the
 * layout using a weight value.  
 */
void RelativeLayoutManager::Layout::setYPaddingWeight(int weight)
{
    yPaddingWeight = weight;
    yPaddingFraction = 0;
}

/*
 * Gets the fraction of the width to allocate to each horizontal margin.
 */
float RelativeLayoutManager::Layout::getXMarginFraction() const
{
    return xMarginFraction;
}

/*
 * Gets the fraction of the height to allocate to each vertical margin.
 */
float RelativeLayoutManager::Layout::getYMarginFraction() const
{
    return yMarginFraction;
}

/*
 * Gets the weight value assigned to each vertical margin.
 */
int RelativeLayoutManager::Layout::getYMarginWeight() const
{
    return yMarginWeight;
}

/*
 * Gets the fraction of the width to allocate to each space between
 * non-null components.
 */
float RelativeLayoutManager::Layout::getXPaddingFraction() const
{
    return xPaddingFraction;
}

/*
 * Gets the weight value assigned to each horizontal space between
 * non-null layout components.
 */
int RelativeLayoutManager::Layout::getXPaddingWeight() const
{
    return xPaddingWeight;
}

/*
 * Gets the fraction of the height to allocate to each space between
 * non-empty rows.
 */
float RelativeLayoutManager::Layout::getYPaddingFraction() const
{
    return yPaddingFraction;
}

/*
 * Gets the weight value assigned to each vertical space between
 * non-empty layout rows.
 */
int RelativeLayoutManager::Layout::getYPaddingWeight() const
{
    return yPaddingWeight;
}

/*
 * Gets the list of all rows in the layout.
 */
const std::vector<RelativeLayoutManager::RowLayout>& 
RelativeLayoutManager::Layout::getRows() const
{
    return rows;
}
        
/*
 * Inserts a new row into the layout at a specific index.
 */
void RelativeLayoutManager::Layout::insertRow
(const RelativeLayoutManager::RowLayout row, int index)
{
    int iterIdx = 0;
    auto rowIter = rows.begin();
    while(rowIter != rows.end() && iterIdx < index)
    {
        iterIdx++;
        rowIter++;
    }
    rows.insert(rowIter, row);
}

/*
 * Adds a new row to the end of the layout.
 */
void RelativeLayoutManager::Layout::addRow
(const RelativeLayoutManager::RowLayout row)
{
    rows.push_back(row);
}

#if JUCE_DEBUG

/**
 * Print out the layout to the console for debugging
 */
void RelativeLayoutManager::printLayout()
{
    int rowNum = 0;
    for (RowLayout& row : layout.getRows())
    {
        DBG("RelativeLayoutManager::" << __func__ << ":");
        DBG(String("Row weight:") + String(row.vertWeight) + String("/")
                + String(yWeightSum));
        String rowStr = "\t";
        for (ComponentLayout& comp : row.rowItems)
        {
            if (comp.component == nullptr)
            {
                rowStr += "null";
            }
            else
            {
                rowStr += comp.component->getName();
            }
            rowStr += "(";
            rowStr += String(comp.weight);
            rowStr += "/";
            rowStr += String(xWeightSums[rowNum]);
            rowStr += ") ";
        }
        DBG(rowStr);
        rowNum++;
    }
    DBG("");
}

#endif
