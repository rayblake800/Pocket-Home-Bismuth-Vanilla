#include "GridLayoutManager.h"

GridLayoutManager::GridLayoutManager()
{
}

GridLayoutManager::~GridLayoutManager()
{
}

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
        std::vector<Row>::iterator rowIter = rows.begin()+rowIndex;
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
    std::vector<Row>::iterator rowIter = rows.begin()+rowIndex;
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
(std::vector<ComponentLayoutParams> components,
        Component* parentToInit)
{
    for (const ComponentLayoutParams& compLayout : components)
    {
        addComponent(compLayout.comp, compLayout.row,
                compLayout.horizWeight, parentToInit);
    }
}

/**
 * Arrange the components within a bounding rectangle
 */
void GridLayoutManager::layoutComponents(Rectangle<int> bounds, int xPadding,
        int yPadding)
{
    int fullWidth = bounds.getWidth() - xPadding;
    int fullHeight = bounds.getHeight() - yPadding;
    int xStart = bounds.getX() + xPadding;
    int yStart = bounds.getY() + yPadding;

    int yPos = yStart;
    //DBG(String(rows.size()) + String(":") + String(vertWeightSum));
    for(const Row& row : rows){
        int height = fullHeight * row.vertWeight / vertWeightSum - yPadding;
        int xPos = xStart;
        for (int columnInd = 0; columnInd < row.columns.size(); columnInd++)
        {
            WeightedCompPtr compPtr = row.columns[columnInd];
            int width = fullWidth * compPtr.weight / row.horizWeightSum
                    - xPadding;
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
