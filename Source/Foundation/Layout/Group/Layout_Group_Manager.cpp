#include "Layout_Group_Manager.h"
#include "Layout_Transition_Animator.h"

namespace GroupLayout = Layout::Group;

/*
 * Gets the current component layout held by this LayoutManager.
 */
GroupLayout::RelativeLayout GroupLayout::Manager::getLayout() const
{
    return layout;
}

/*
 * Set a new Component layout, removing all old layout definitions.
 */
void GroupLayout::Manager::setLayout
(const RelativeLayout& layout, juce::Component* parentToInit)
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
            if (cNum > 0 && rowItem.getWeight() != 0
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
void GroupLayout::Manager::transitionLayout(
        const RelativeLayout& newLayout,
        juce::Component* parent,
        const Transition::Type transition,
        const unsigned int duration)
{
    if(transition == Transition::Type::none || duration == 0)
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
                Transition::Animator::transitionOut(rowItem.getComponent(),
                        transition, duration, true);
            }
        }
    }
    clearLayout(true);
    // Transition in new layout items:
    setLayout(newLayout, parent);
    layoutComponents(parent->getLocalBounds(), transition, duration);
}

/*
 * Adds all components in the layout to a parent component, and makes them
 * all visible.
 */
void GroupLayout::Manager::addComponentsToParent(juce::Component* parent)
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
void GroupLayout::Manager::layoutComponents(const juce::Rectangle<int>& bounds,
            const Transition::Type transition,
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
            height = std::max<int>(0,
                    row.getWeight() * weightedHeight / yWeightSum);
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
                width = std::max<int>(0,
                        rowItem.getWeight() * weightedWidth / xWeightSum);
            }
            if (!rowItem.isEmpty())
            {
                Transition::Animator::transitionIn(rowItem.getComponent(),
                        transition,
                        juce::Rectangle<int>(xPos, yPos, width, height),
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
void GroupLayout::Manager::clearLayout(bool removeComponentsFromParent)
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
                    juce::Component * parent = rowItem.getComponent()
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
    layout = RelativeLayout();
    xWeightSums.clear();
    yWeightSum = 0;
}

#if JUCE_DEBUG

/**
 * Print out the layout to the console for debugging
 */
void GroupLayout::Manager::printLayout()
{
    using juce::String;
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
