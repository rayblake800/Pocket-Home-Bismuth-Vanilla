#include "RelativeLayoutManager.h"

/*
 * Set a new Component layout, removing all old layout definitions.
 */
void RelativeLayoutManager::setLayout
(const Layout& layout, Component* parentToInit)
{
    clearLayout();
    this->layout = layout;
    for (int rowNum = 0; rowNum < layout.rows. size(); rowNum++)
    {
        const RowLayout& rowLayout = layout.rows[rowNum];
        yWeightSum += rowLayout.rowWeight;
        if (rowLayout.yPaddingWeight >= 0)
        {
            yWeightSum += rowLayout.yPaddingWeight;
        }
        else
        {
            yWeightSum += layout.yPaddingWeight;
        }
        xWeightSums[rowNum] = 0;
        for (const ComponentLayout& compLayout : rowLayout.rowItems)
        {
            if (compLayout.xPaddingWeight >= 0)
            {
                xWeightSums[rowNum] += compLayout.xPaddingWeight;
            }
            else if (rowLayout.xPaddingWeight >= 0)
            {
                xWeightSums[rowNum] += rowLayout.xPaddingWeight;
            }
            else
            {
                xWeightSums[rowNum] += layout.xPaddingWeight;
            }
            xWeightSums[rowNum] += compLayout.componentWeight;
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
    for (RowLayout& row : layout.rows)
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
    int xMargins = layout.xMarginFraction * bounds.getWidth();
    int yMargins = layout.yMarginFraction * bounds.getHeight();
    int topPaddingWeight = layout.rows.begin()->yPaddingWeight;
    int bottomPaddingWeight = layout.rows.back()->yPaddingWeight;

    //Layout margins and edge row padding should overlap.
    int yPixPerWeight = 0;
    if (yWeightSum > 0 && yWeightSum
        > ((topPaddingWeight + bottomPaddingWeight) / 2))
    {
        yPixPerWeight = (bounds.getHeight() - yMargins)
                / (yWeightSum - ((topPaddingWeight + bottomPaddingWeight) / 2));

        if ((yPixPerWeight * (topPaddingWeight + bottomPaddingWeight) / 2)
            > yMargins)
        {
            yPixPerWeight = bounds.getHeight() / yWeightSum;
        }
    }

    int yPos = bounds.getY()
            + (yMargins - topPaddingWeight * yPixPerWeight) / 2;

    for (int rowNum = 0; rowNum < layout.rows.size(); rowNum++)
    {
        const RowLayout& row = layout.rows[rowNum];
        int yPadding = row.yPaddingWeight * yPixPerWeight;
        int height = row.rowWeight * yPixPerWeight;
        yPos += yPadding / 2;

        int leftPaddingWeight = row.rowItems.begin()->xPaddingWeight;
        int rightPaddingWeight = row.rowItems.back()->xPaddingWeight;
        const int& xWeightSum = xWeightSums[rowNum];

        int xPixPerWeight = 0;
        if (xWeightSum > 0 && xWeightSum
            > ((leftPaddingWeight + rightPaddingWeight) / 2))
        {
            xPixPerWeight = (bounds.getWidth() - xMargins) / (xWeightSum 
                    - ((leftPaddingWeight + rightPaddingWeight) / 2));
        }
        if ((xPixPerWeight * (leftPaddingWeight + rightPaddingWeight) / 2)
            > xMargins)
        {
            xPixPerWeight = bounds.getWidth() / xWeightSum;
        }
        
        int xPos = bounds.getX() 
            + (xMargins - leftPaddingWeight * xPixPerWeight) / 2;
        for (const ComponentLayout& compLayout : row.rowItems)
        {
            int xPadding = compLayout.xPaddingWeight * xPixPerWeight;
            int width = compLayout.componentWeight * xPixPerWeight;
            xPos += xPadding / 2;
            if (compLayout.component != nullptr)
            {
                compLayout.component->setBounds(xPos, yPos, width, height);
            }
            xPos += width + xPadding / 2;
        }
        yPos += height + yPadding / 2;
    }
}

/*
 * Remove all saved component layout parameters
 */
void RelativeLayoutManager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (RowLayout& row : layout)
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
    layout = {};
    xWeightSums.clear();
    yWeightSum = 0;
}

#if JUCE_DEBUG

/**
 * Print out the layout to the console for debugging
 */
void RelativeLayoutManager::printLayout()
{
    int rowNum = 0;
    for (RowLayout& row : layout)
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
            rowStr += String(comp.componentWeight);
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
