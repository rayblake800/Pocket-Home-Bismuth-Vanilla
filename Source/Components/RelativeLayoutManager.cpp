#include "RelativeLayoutManager.h"

/**
 * Set a new Component layout, removing all old layout definitions.
 */
void RelativeLayoutManager::setLayout
(const Layout& layout, Component* parentToInit)
{
    clearLayout();
    this->layout = layout;
    for (int rowNum = 0; rowNum < layout.size(); rowNum++)
    {
        vertWeightSum += layout[rowNum].vertWeight;
        horizWeightSums.set(rowNum, 0);
        for (const ComponentLayout& compLayout : layout[rowNum].components)
        {
            horizWeightSums.getReference(rowNum) += compLayout.horizWeight;
            if (parentToInit != nullptr)
            {
                parentToInit->addAndMakeVisible(compLayout.component);
            }
        }
    }
}

/**
 * Adds all components in the layout to a parent component, and makes them
 * all visible.
 */
void RelativeLayoutManager::addComponentsToParent(Component* parent)
{
    for (RowLayout& row : layout)
    {
        for (ComponentLayout& compPtr : row.components)
        {
            parent->addAndMakeVisible(compPtr.component);
        }
    }
}

/**
 * Arranges the components within a bounding rectangle.
 */
void RelativeLayoutManager::layoutComponents(
        Rectangle<int> bounds,
        int xPadding,
        int yPadding)
{
    int usableHeight = bounds.getHeight() - (layout.size() - 1) * yPadding;
    int xStart = bounds.getX();
    int yStart = bounds.getY();

    int yPos = yStart;
    for (int rowNum = 0; rowNum < layout.size(); rowNum++)
    {
        RowLayout& row = layout[rowNum];
        int usableWidth = bounds.getWidth() - (row.components.size() - 1)
                * xPadding;
        int height = 0;
        if (vertWeightSum > 0)
        {
            height = usableHeight * row.vertWeight / vertWeightSum;
        }
        int xPos = xStart;
        for (int columnNum = 0; columnNum < row.components.size(); columnNum++)
        {
            const ComponentLayout& compPtr = row.components[columnNum];
            int width = 0;
            if (horizWeightSums[rowNum] > 0)
            {
                width = usableWidth * compPtr.horizWeight
                        / horizWeightSums[rowNum];
            }
            if (compPtr.component != nullptr)
            {
                compPtr.component->setBounds(xPos, yPos, width, height);
            }
            xPos += width + xPadding;
        }
        yPos += height + yPadding;
    }
}

/**
 * Remove all saved component layout parameters
 */
void RelativeLayoutManager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (RowLayout& row : layout)
        {
            for (ComponentLayout& componentData : row.components)
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
    layout.clear();
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
        for (ComponentLayout& comp : row.components)
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
            rowStr += String(comp.horizWeight);
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
