/**
 * @file GridLayoutManager
 * @author Anthony Brown
 * 
 * GridLayoutManager arranges components in an arbitrary bounding rectangle.
 * Components are arranged into rows and resized to fit the available space,
 * with weight values determining their sizes in proportion to each other.
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class GridLayoutManager{
public:
    GridLayoutManager();
    virtual ~GridLayoutManager();

    /**
     * add a new (empty) row, assigning it a weight value
     * @param vertWeight determines the relative height of the row
     */
    void addRow(int vertWeight);
    
    /**
     * Add a new component to a grid row.
     * @param comp any UI component
     * @param row index of the component's row. The component will be added
     * to the end of the row.  If the row index does not exist yet, rows with
     * a vertical weight of 1 will be added until it does exist. If the index is
     * less than zero, it is rounded up to zero.
     * @param horizWeight determines the relative width of the component.
     */
    void addComponent(Component * comp,int row,int horizWeight);

    /**
     * Arrange the components within a bounding rectangle
     * @param bounds the rectangle containing the components
     * @param xPadding space in pixels to leave between components in a row
     * and between components and the left and right edges of the bounds
     * @param yPadding space in pixels to leave between rows and between
     * rows and the top and bottom edges of the bounds.
     */
    void layoutComponents(Rectangle<int> bounds,int xPadding,int yPadding);
private:
    struct WeightedCompPtr
    {
        Component* component;
        int weight=0;
    };

    struct Row
    {
        Array<WeightedCompPtr> columns;
        int horizWeightSum=0;
        int vertWeight=0;
    };
    int vertWeightSum=0;
    Array<Row> rows;
};