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
    int xMargins = layout.xMarginFraction * bounds.getWidth();
    int yMargins = layout.yMarginFraction * bounds.getHeight();
    int topPaddingWeight = layout.rows.begin()->yPaddingWeight;
    int bottomPaddingWeight = layout.rows.back()->yPaddingWeight;

    //Layout margins and edge row padding should overlap.
    int yPixPerWeight = 0;
    if(vertWeightSum > 0 && vertWeightSum 
            > ((topPadding + bottomPaddingWeight) / 2))
    {
        yPixPerWeight = (bounds.getHeight() - xMargins) / (vertWeightSum 
	        - ((topPaddingWeight + bottomPaddingWeight) / 2);
	
	if((yPixelsPerWeight * (topPaddingWeight + bottomPaddingWeight) / 2)
	        > xMargins)
	{
	    yPixelsPerWeight = bounds.getHeight() / vertWeightSum;
	}
    }

    int yPos = bounds.getY() 
            + (xMargins - topPaddingWeight * yPixelsPerWeight) / 2;
    
    for (int rowNum = 0; rowNum < layout.rows.size(); rowNum++)
    {
        const RowLayout& row = layout.rows[rowNum];
        int yPadding = 0;
        int height = 0;

        if (vertWeightSum == 0)
        {
            yPadding = row.yPaddingWeight * yPixelsPerWeight;
            int height = row.rowWeight * yPixelsPerWeight;
            yPos += yPadding/2;
        }

        int leftPaddingWeight = row.rowItems.begin()->xPaddingWeight;
        int rightPaddingWeight = row.rowItems.back()->yPaddingWeight;

        int xPixelsPerWeight = -(bounds.getWidth() - 2 * xMargin)
                / (leftPaddingWeight + rightPaddingWeight 
		- horizWeightSums[rowNum]);
    
        int adjustedLeftMargin = std::max(0,
            xMargin - leftPaddingWeight * xPixelsPerWeight);

        int xPos = bounds.getX() + adjustedLeftMargin;
        for (const ComponentLayout& compLayout : row.rowItems)
        {
            int xPadding = 0;
            int width = 0;
            if (horizWeightSums[rowNum] > 0)
            {
                xPadding = compLayout.xPaddingWeight * xPixelsPerWidth;
                width = compLayout.componentWeight * xPixelsPerWidth;
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
