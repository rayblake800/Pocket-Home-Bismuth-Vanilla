/* 
 * File:   AppMenu.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 * A menu for launching applications, populated with .Desktop file info
 */

#ifndef APPMENU_H
#define APPMENU_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "AppMenuButton.h"
#include "DesktopEntries.h"

class AppMenu : public Component,
public Button::Listener{
public:
    AppMenu(const var &configJson);
    virtual ~AppMenu();
    
    /**
     * Open an application category folder, creating AppMenuButtons for all
     * associated desktop applications.
     * @param categoryName the category to open
     */
    void openFolder(String categoryName);
    
    /**
     * close the topmost open folder, removing all contained buttons
     */
    void closeFolder();
    
    //handle AppMenuButton clicks
    void buttonClicked (Button* buttonClicked) override;
    
    /**
     * Select the next appMenuButton in the active button column.
     */
    void selectNext();
    
    /**
     * Select the previous appMenuButton in the active button column.
     */
    void selectPrevious();
    
    /**
     * Trigger a click for the selected button.
     */
    void clickSelected();
    
    /**
     * @return the index of the active button column.
     */
    int activeColumn();
private:
    DesktopEntries desktopEntries;
    void selectIndex(int index);
    void addButton(AppMenuButton* appButton);
    void scrollTo(Rectangle<int> dest);
    std::map<String,AppMenuButton*> nameMap;

    //all buttons in each column
    std::vector<std::vector<AppMenuButton*>> buttonColumns;
    //current button selection(if any) for each open column
    std::vector<AppMenuButton*> selected;
    
    //appMenuButton dimensions, loaded in constructor
    int buttonWidth;
    int buttonHeight;
};

#endif /* APPMENU_H */

