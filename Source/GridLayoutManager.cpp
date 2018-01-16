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
    rows.add(newRow);
}

/**
 * @return the number of rows in the layout.  This may include empty rows.
 */
int GridLayoutManager::getNumRows()
{
    return rows.size();
}

/**
 * Add a new component to a grid row.
 */
void GridLayoutManager::addComponent(Component * comp, int rowIndex,
        int horizWeight)
{
    WeightedCompPtr wCompPtr;
    wCompPtr.component = comp;
    wCompPtr.weight = horizWeight;
    if (rowIndex < 0)
    {
        rowIndex = 0;
    }
    while (rowIndex >= rows.size())
    {
        addRow(1);
    }
    Row& row = rows.getReference(rowIndex);
    row.columns.add(wCompPtr);
    row.horizWeightSum += horizWeight;
}

/**
 * Arrange the components within a bounding rectangle
 */
void GridLayoutManager::layoutComponents(Rectangle<int> bounds, int xPadding, int yPadding)
{
    int fullWidth = bounds.getWidth() - xPadding;
    int fullHeight = bounds.getHeight() - yPadding;
    int xStart = bounds.getX() + xPadding;
    int yStart = bounds.getY() + yPadding;

    int yPos = yStart;
    DBG(String(rows.size()) + String(":") + String(vertWeightSum));
    for (int rowInd = 0; rowInd < rows.size(); rowInd++)
    {
        Row row = rows[rowInd];
        int height = fullHeight * row.vertWeight / vertWeightSum - yPadding;
        int xPos = xStart;
        for (int columnInd = 0; columnInd < row.columns.size(); columnInd++)
        {
            WeightedCompPtr compPtr = row.columns[columnInd];
            int width = fullWidth * compPtr.weight / row.horizWeightSum
                    - xPadding;

            compPtr.component->setBounds(xPos, yPos, width, height);
            xPos += width + xPadding;
        }
        yPos += height + yPadding;
    }
}
