#include "GridLayoutManager.h"

GridLayoutManager::GridLayoutManager() { }

GridLayoutManager::~GridLayoutManager() { }

/**
 * add a new (empty) row, assigning it a weight value
 */
void GridLayoutManager::addRow(int vertWeight)
{
    Row newRow;
    newRow.vertWeight = vertWeight;
    vertWeightSum += vertWeight;
    rows.push_back(newRow);
}

/**
 * @return the number of rows in the layout.  This may include empty rows.
 */
int GridLayoutManager::getNumRows()
{
    return rows.size();
}

/**
 * Updates the vertical weight value of a row. If the row doesn't exist,
 * nothing will happen.
 */
void GridLayoutManager::setRowWeight(int rowIndex, int newWeight)
{
    if (rowIndex >= 0 && rowIndex < rows.size())
    {
        std::vector<Row>::iterator rowIter = rows.begin() + rowIndex;
        vertWeightSum -= rowIter->vertWeight;
        rowIter->vertWeight = newWeight;
        vertWeightSum += newWeight;
    }
}

/**
 * Add a new component to a grid row.
 */
void GridLayoutManager::addComponent(Component * addedComp, int rowIndex,
        int horizWeight, Component* parentToInit)
{
    WeightedCompPtr wCompPtr;
    wCompPtr.component = addedComp;
    wCompPtr.weight = horizWeight;
    if (rowIndex < 0)
    {
        rowIndex = 0;
    }
    while (rowIndex >= rows.size())
    {
        addRow(1);
    }
    std::vector<Row>::iterator rowIter = rows.begin() + rowIndex;
    rowIter->columns.push_back(wCompPtr);
    rowIter->horizWeightSum += horizWeight;
    if (addedComp != nullptr && parentToInit != nullptr)
    {
        parentToInit->addAndMakeVisible(addedComp);
    }
}

/**
 * Add a list of components to the layout manager
 */
void GridLayoutManager::addComponents
(const std::vector<RowLayoutParams>& rows, Component* parentToInit)
{
    for (int rowNum = 0; rowNum < rows.size(); rowNum++)
    {
        addRow(rows[rowNum].vertWeight);
        for (const ComponentLayoutParams& compLayout : rows[rowNum].compRow)
        {
            addComponent(compLayout.comp, rowNum,
                    compLayout.horizWeight, parentToInit);
        }
    }
}

/**
 * Adds all components in the layout to a parent component, and makes them
 * all visible.
 */
void GridLayoutManager::addComponentsToParent(Component* parent)
{
    for (const Row& row : rows)
    {
        for(const WeightedCompPtr& compPtr : row.columns)
        {
            parent->addAndMakeVisible(compPtr.component);
        }
    }
}

/**
 * Arrange the components within a bounding rectangle
 */
void GridLayoutManager::layoutComponents(Rectangle<int> bounds, int xPadding,
        int yPadding)
{
    int usableHeight = bounds.getHeight() - (rows.size() - 1) * yPadding;
    int xStart = bounds.getX();
    int yStart = bounds.getY();

    int yPos = yStart;
    //DBG(String(rows.size()) + String(":") + String(vertWeightSum));
    for (const Row& row : rows)
    {
        int usableWidth = bounds.getWidth() - (row.columns.size() - 1)
                * xPadding;
        int height = usableHeight * row.vertWeight / vertWeightSum;
        int xPos = xStart;
        for (int columnInd = 0; columnInd < row.columns.size(); columnInd++)
        {
            WeightedCompPtr compPtr = row.columns[columnInd];
            int width = usableWidth * compPtr.weight / row.horizWeightSum;
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
void GridLayoutManager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (const Row& row : rows)
        {
            for (const WeightedCompPtr& componentData : row.columns)
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
    rows.clear();
    vertWeightSum = 0;
}
