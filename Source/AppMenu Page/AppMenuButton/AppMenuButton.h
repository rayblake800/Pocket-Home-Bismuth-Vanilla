/* 
 * File:   AppMenuButton.h
 * Author: anthony
 * 
 * Created on December 17, 2017, 3:57 PM
 * Defines a custom button type for applications and application directories
 */

#pragma once
#include "../../../JuceLibraryCode/JuceHeader.h"

class IconThread;
class AppMenuButton : public TextButton, public ReferenceCountedObject {
public:
    friend class IconThread;
    typedef ReferenceCountedObjectPtr<AppMenuButton> Ptr;

    /**
     * Create a new AppMenu button
     * @param name button name value
     * @param index button position in its column
     * @param column button's column in the AppMenu
     */
    AppMenuButton(String name, int index, int column);

    /**
     * Set whether this button is currently selected.
     * @param select new selection state
     */
    void setSelected(bool select);

    /**
     * @return true if this button is for an application folder
     */
    virtual bool isFolder() const = 0;

    /**
     * @return the display name of the associated application
     */
    virtual String getAppName() const = 0;

    /**
     * @return application shell command or directory path.
     */
    virtual String getCommand() const = 0;

    /**
     * @return all application categories linked to this button.
     */
    virtual Array<String> getCategories() const = 0;

    /**
     * @return button position in its column
     */
    int getIndex() const;

    /**
     * @return button's column in the AppMenu
     */
    int getColumn() const;

    /**
     * @param index a new index in the column to assign to this button.
     */
    void setIndex(int index);

    /**
     * @param column a new column number to assign to this button.
     */
    void setColumn(int column);

    /**
     * @return the size of an AppMenuButton with the current window size
     * and config file ratios.
     */
    static Rectangle<int> getButtonSize();

protected:
    /**
     * Custom button painting method.
     */
    void paint(Graphics& g) override;
    
    /**
     * Re-calculates draw values whenever the button is resized
     */
    void resized() override;

    //Icon image to draw
    Image appIcon;

private:
    //bounds for drawing app/folder name
    Rectangle<float> textBox;
    //bounds for drawing button icon
    Rectangle<float> imageBox;
    //font for drawing app/folder name
    Font titleFont;
    //true iff this button is selected
    bool selected = false;
    //index in the column, starting at the top
    int index;
    //containing column number, counted left to right
    int column;
    //Button background fill color, if not selected.
    Colour fillColour;
    //Button background fill color, if selected.
    Colour selectedFillColour;
};
