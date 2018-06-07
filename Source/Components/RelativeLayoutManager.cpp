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
        vertWeightSum += rowLayout.rowWeight;
        if (rowLayout.yPaddingWeight >= 0)
        {
            vertWeightSum += rowLayout.yPaddingWeight;
        }
        else
        {
            vertWeightSum += layout.yPaddingWeight;
        }
        horizWeightSums[rowNum] = 0;
        for (const ComponentLayout& compLayout : rowLayout.rowItems)
        {
            if (compLayout.xPaddingWeight >= 0)
            {
                horizWeightSums[rowNum] += compLayout.xPaddingWeight;
            }
            else if (rowLayout.xPaddingWeight >= 0)
            {
                horizWeightSums[rowNum] += rowLayout.xPaddingWeight;
            }
            else
            {
                horizWeightSums[rowNum] += layout.xPaddingWeight;
            }
            horizWeightSums[rowNum] += compLayout.componentWeight;
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
    int xMargin = layout.xMarginFraction * bounds.getWidth() / 2;
    int yMargin = layout.yMarginFraction * bounds.getHeight() / 2;
    int topPaddingWeight = layout.rows.begin()->yPaddingWeight;
    int bottomPaddingWeight = layout.rows.back()->yPaddingWeight;

    //Layout margins and edge row padding should overlap.

    //usableHeight = height - 2*yMargin + topPadding + bottomPadding
    //topPadding   = usableHeight * topPaddingWeight / vertWeightSum
    //bottomPadding= usableHeight * bottomPaddingWeight / vertWeightSum

    //x1 = usableHeight
    //x2 = topPadding
    //x3 = bottomPadding

    //a = height
    //b = yMargin
    //c = top padding weight
    //d = bottom padding weight
    //e = vertWeightSum

    // x1 = a - 2b + x2 + x3
    // x2 = x1*c/e
    // x3 = x1*d/e

    // x1 = -e(a-2b)/(c+d-e)
    // x2 = -c(a-2b)/(c+d-e)
    // x3 = -d(a-2b)/(c+d-e)

    int yWeightValue = -(bounds.getHeight() - 2 * yMargin)
            / (topPaddingWeight + bottomPaddingWeight - vertWeightSum);
    
    int usableHeight = vertWeightSum * yWeightValue;
    int adjustedTopMargin = yMargin - topPaddingWeight * yWeightValue;
    int adjustedBottomMargin =  yMargin - bottomPaddingWeight * yWeightValue;

    int yPos = bounds.getY() + adjustedTopMargin;
    for (int rowNum = 0; rowNum < layout.rows.size(); rowNum++)
    {
        const RowLayout& row = layout.rows[rowNum];
        int yPadding = 0;
        int height = 0;

        if (vertWeightSum == 0)
        {
            yPadding = row.yPaddingWeight * usableHeight
                    / vertWeightSum / 2;
            int height = row.rowWeight * usableHeight / vertWeightSum;
            yPos += yPadding;
        }

        int xPos = bounds.getX();
        for (const ComponentLayout& compLayout : row.rowItems)
        {
            int xPadding = 0;
            int width = 0;
            if (horizWeightSums[rowNum] > 0)
            {
                xPadding = compLayout.xPaddingWeight * bounds.getWidth()
                        / horizWeightSums[rowNum] / 2;
                width = bounds.getWidth() * compLayout.componentWeight
                        / horizWeightSums[rowNum];
            }
            xPos += xPadding;
            if (compLayout.component != nullptr)
            {
                compLayout.component->setBounds(xPos, yPos, width, height);
            }
            xPos += width + xPadding;
        }
        yPos += height + yPadding;
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
    horizWeightSums.clear();
    vertWeightSum = 0;
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
                + String(vertWeightSum));
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
            rowStr += String(horizWeightSums[rowNum]);
            rowStr += ") ";
        }
        DBG(rowStr);
        rowNum++;
    }
    DBG("");
}

#endif
