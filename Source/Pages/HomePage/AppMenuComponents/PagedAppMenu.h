/**
 * @file PagedAppMenu.h
 * 
 * TODO finish documentation
 */

#pragma once
#include "../../../Configuration/Configurables/ConfigurableImageButton.h"
#include "AppMenuComponent.h"

class PagedAppMenu : public AppMenuComponent, public Button::Listener {
public:
    /**
     * @param loadingSpinner
     */
    PagedAppMenu(OverlaySpinner& loadingSpinner);
    virtual ~PagedAppMenu();

    /**
     * Use key presses for menu navigation, setting specific controls based on 
     * AppMenu type. Other classes may call this to pass on or simulate
     * key events.
     * @param key
     * @return true if the key press was used.
     */
    bool keyPressed(const KeyPress& key) override;

protected:
    /**
     * Updates the folder component layout, optionally animating the transition.
     * @param animateTransition if true, animate component changes rather than
     * immediately updating folder bounds.
     */
    virtual void layoutFolders(bool animateTransition) override;

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    AppFolder* createFolderObject(AppMenuItem::Ptr folderItem) override;

private:

    /**
     * Updates the layout if row/column size changes, otherwise handle
     * changes like any other ConfigurableComponent.
     * @param config the configFile containing the updated data value
     * @param key the key of property that has changed
     */
    void loadConfigProperties(ConfigFile* config, String key);


    /**
     * Handles navigation button controls
     * @param 
     */
    void buttonClicked(Button*) override;

    //app grid dimensions, saved from config
    int maxRows = 0;
    int maxColumns = 0;

    //navigation buttons

    ConfigurableImageButton pageLeft;
    ConfigurableImageButton pageRight;


    //############################ PageAppFolder ###############################

    /**
     *
     */
    class PageAppFolder : public AppFolder {
    public:
        PageAppFolder(AppMenuItem::Ptr folderItem, MouseListener* btnListener,
                std::map<String, AppMenuButton::Ptr>& buttonNameMap,
                IconThread& iconThread);
        virtual ~PageAppFolder();

        /**
         * Sets the button grid row and column sizes, updating button layout
         * if the values change
         */
        void updateGridSize(int maxRows, int maxColumns);

        /**
         * Create an AppMenuButton component for an AppMenuItem.
         * @param menuItem
         */
        virtual AppMenuButton::Ptr createMenuButton
        (AppMenuItem::Ptr menuItem) override;

        /**
         * Given a list of folder buttons, return an appropriate layout
         * for positioning them in the folder component.
         * 
         * @param buttons
         * @return a Layout containing all items in the button array.
         */
        virtual GridLayoutManager::Layout buildFolderLayout
        (Array<AppMenuButton::Ptr>& buttons);
        
        /**
         * @return the number of pages this folder needs to display all menu
         * buttons.
         */
        int getNumFolderPages();
        
        /**
         * @return the index of the page that's currently visible, or -1 if 
         * there is no current page. 
         */
        int getCurrentPage();
        
        /**
         * @return the index of the selected menu button within the current
         * folder page, or -1 if no buttons are selected on the current folder
         * page.
         */
        int getSelectionPageIndex();
        
        /**
         * Set which folder page should currently be visible.
         * @param pageNum must be a valid page index, or the selection will
         * not change.
         * @return true if the page selection changed.
         */
        bool setCurrentPage(int pageNum);
        
        /**
         * Select a menu button using its index within the current folder page.
         * The selection will not change if the index is invalid or there
         * @param pageIndex
         * @return 
         */
        bool setSelectionPageIndex(int pageIndex);
    private:

        /**
         * Resizes navigation buttons, then calls AppFolder::resized().
         */
        void resized() override;
        int maxRowNum = 1;
        int maxColumnNum = 1;
        int buttonsPerPage = 1;
    };

    //############################  PageMenuButton  ############################

    /**
     *
     */
    class PageMenuButton : public AppMenuButton {
    public:
        /**
         * 
         * @param menuItem
         * @param iconThread
         * @param name
         */
        PageMenuButton(AppMenuItem::Ptr menuItem, IconThread& iconThread,
                String name = String::empty);
        virtual ~PageMenuButton();
    private:

        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;

        /**
         * Set the background and border to only draw for selected buttons.
         */
        void selectionStateChanged() override;
    };
};
