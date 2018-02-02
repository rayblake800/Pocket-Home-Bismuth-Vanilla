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
     * @return the number of rows in the layout.  This may include empty rows.
     */
    int getNumRows();
    
    /**
     * Updates the vertical weight value of a row. If the row doesn't exist,
     * nothing will happen.
     * @param rowIndex the row to update.
     * @param newWeight the new weight value to set.
     */
    void setRowWeight(int rowIndex,int newWeight);
    
    /**
     * Add a new component to a grid row.
     * @param comp any UI component
     * @param row index of the component's row. The component will be added
     * to the end of the row.  If the row index does not exist yet, rows with
     * a vertical weight of 1 will be added until it does exist. If the index is
     * less than zero, it is rounded up to zero.
     * @param horizWeight determines the relative width of the component.
     * @param parentToInit if this optional argument is provided, this will add
     * comp as a child of parentToInit and make it visible.
     */
    void addComponent(Component * comp,int row,int horizWeight,
        Component* parentToInit=nullptr);
    
    //bulk component setup:
    struct ComponentLayoutParams{
        ComponentLayoutParams(Component* comp,int row, int horizWeight):
        comp(comp),
        row(row),
        horizWeight(horizWeight){};
        
        Component* comp;
        int row;
        int horizWeight;
    };
    
    /**
     * Add a list of components to the layout manager
     * @param components a list of components, with their weights and 
     * row indexes
     * @param parentToInit if this optional argument is provided, this will add
     * all components as children of parentToInit and make them visible.
     */
    void addComponents(std::vector<ComponentLayoutParams> components,
        Component* parentToInit=nullptr);

    /**
     * Arrange the components within a bounding rectangle
     * @param bounds the rectangle containing the components
     * @param xPadding space in pixels to leave between components in a row
     * and between components and the left and right edges of the bounds
     * @param yPadding space in pixels to leave between rows and between
     * rows and the top and bottom edges of the bounds.
     */
    void layoutComponents(Rectangle<int> bounds,int xPadding,int yPadding);
    
    /**
     * Remove all saved component layout parameters
     * @param removeComponentsFromParent if true, all components will also
     * be removed from their parent component.
     */
    void clearLayout(bool removeComponentsFromParent=false);
private:
    struct WeightedCompPtr
    {
        Component* component;
        int weight=0;
    };

    struct Row
    {
        std::vector<WeightedCompPtr> columns;
        int horizWeightSum=0;
        int vertWeight=0;
    };
    int vertWeightSum=0;
    std::vector<Row> rows;
};