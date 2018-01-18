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
 * Updates the vertical weight value of a row. If the row doesn't exist,
 * nothing will happen.
 */
void GridLayoutManager::setRowWeight(int rowIndex, int newWeight)
{
    if (rowIndex >= 0 && rowIndex < rows.size())
    {
        vertWeightSum -= rows.getReference(rowIndex).vertWeight;
        rows.getReference(rowIndex).vertWeight = newWeight;
        vertWeightSum += newWeight;
    }
}

/**
 * Add a new component to a grid row.
 */
void GridLayoutManager::addComponent(Component * comp, int rowIndex,
        int horizWeight, Component* parentToInit)
{
    DBG(String("Adding ")+comp->getName()+String(" to row ")
            +String(rowIndex)+String(" with weight ")+String(horizWeight));
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
    if (parentToInit != nullptr)
    {
        DBG(String("adding ")+comp->getName()+String( "to parent" )
                +parentToInit->getName());
        parentToInit->addAndMakeVisible(comp);
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
    DBG(String(rows.size()) + String(":") + String(vertWeightSum));
    for (int rowInd = 0; rowInd < rows.size(); rowInd++)
    {
        Row row = rows[rowInd];
        DBG(String("row ")+String(rowInd)+String(" weight ")
                +String(row.vertWeight)+String("/")+String(vertWeightSum));
        int height = fullHeight * row.vertWeight / vertWeightSum - yPadding;
        
        DBG(String("\theight ")
                +String(height)+String("/")+String(bounds.getHeight()));
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
