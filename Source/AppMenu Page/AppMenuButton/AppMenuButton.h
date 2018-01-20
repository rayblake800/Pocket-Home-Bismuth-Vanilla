/* 
 * File:   AppMenuButton.h
 * Author: anthony
 * 
 * Created on December 17, 2017, 3:57 PM
 * Defines a custom button type for applications and application directories
 */

#pragma once
#include "../../Configuration/Configurables/ConfigurableComponent.h"
#include "../IconThread.h"
#include "../Popup Editor Components/PopupEditorComponent.h"

class AppMenuButton : public Button, public ReferenceCountedObject,
        public ConfigurableComponent {
public:
    typedef ReferenceCountedObjectPtr<AppMenuButton> Ptr;

    /**
     * Create a new AppMenu button
     * @param name button name value
     * @param index button position in its column
     * @param column button's column in the AppMenu
     * @param iconThread
     */
    AppMenuButton(String name, int index, int column,IconThread& iconThread);

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
     * Gets a PopupEditorComponent configured to edit this button
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    virtual PopupEditorComponent* getEditor() = 0;
    
    /**
     * @return the icon image used by this button.
     */
    const Image& getIcon();

    /**
     * @return the size of an AppMenuButton with the current window size
     * and config file ratios.
     */
    static Rectangle<int> getButtonSize();
protected:
    /**
     * Custom button painting method.
     */
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
    
    /**
     * Re-calculates draw values whenever the button is resized
     */
    void resized() override;
    
    
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);

    /**
     * Requests an icon from the icon thread.
     * @param icon an icon's full path, or the name of an icon file located
     * in common icon directories.
     */
    void loadIcon(String icon);
    //Icon image to draw
    Image appIcon;
    IconThread& iconThread;

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
    //Button text color
    Colour textColour;
    //Button background fill color, if not selected.
    Colour fillColour;
    //Button background fill color, if selected.
    Colour selectedFillColour;
};
