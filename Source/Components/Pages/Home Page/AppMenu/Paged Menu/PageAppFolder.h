/**
 * @file PageAppFolder.h
 * 
 * @brief PageAppFolder organizes one folder of an application menu as 
 * a grid of menu icons, divided into horizontal scrolling pages.
 */

#pragma once
#include "AppMenuFolder.h"

class PageAppFolder : public AppMenuFolder
{
public:
    PageAppFolder(
            AppMenuItem::Ptr folderItem,
            MouseListener* btnListener,
            std::map<juce::String, AppMenuButton::Ptr>& buttonNameMap);

    virtual ~PageAppFolder() { }

    /**
     * Create an AppMenuButton component for an AppMenuItem.
     * 
     * @param menuItem
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem::Ptr menuItem) override;

    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * 
     * @return a Layout containing all items in the button array.
     */
    virtual LayoutManager::Layout buildFolderLayout
    (juce::Array<AppMenuButton::Ptr>& buttons);

    /**
     * @return the number of pages this folder needs to display all menu
     *          buttons.
     */
    int getNumFolderPages() const;

    /**
     * @return the index of the page that's currently visible, or 0 if 
     *          there is no current page. 
     */
    int getCurrentFolderPage() const;

    /**
     * Set which folder page should currently be visible.
     * 
     * @param pageNum  This must be a valid page index, or the selection will
     *                  not change.
     * 
     * @return true iff the page selection changed.
     */
    bool setCurrentFolderPage(int pageNum);

    /**
     * @return the folder page index containing the selected folder 
     *          button, or -1 if there is no selection.
     */
    int getSelectionPage() const;

    /**
     * @return the index of the selected button within its folder page,
     *          or -1 if there is no selection.
     */
    int getSelectedIndexInFolderPage() const;

    /**
     * @return the column index of the selected button within its
     *          folder page, or -1 if there is no selection.
     */
    int getSelectionColumn() const;


    /**
     * @return the row index of the selected button within its
     *          folder page, or -1 if there is no selection.
     */
    int getSelectionRow() const;

    /**
     * Finds what index value a button would have at a particular
     * position within the folder.
     * 
     * @param page    The folder page's index.
     * 
     * @param column  The button's column index in the page.
     * 
     * @param row     The button's row index in the page.
     * 
     * @return   the button's index within the entire folder, or -1 for invalid
     *            positions.
     */
    int positionIndex(int page, int column, int row) const;

    /**
     * Set the button selection based on its position in the
     * folder.  The selection will not change if there isn't
     * a button located at the given position.  If necessary,
     * the current folder page will change to the one containing
     * the new selection.
     *
     * @param page    The index of the button's folder page.
     * 
     * @param column  The new selected button's column number within
     *                 its folder page.
     * 
     * @param row     The new selected button's row number within
     *                 its folder page.
     * 
     * @return true if the selection changed, false otherwise.
     */
    bool setSelectedPosition(int page, int column, int row);

    /**
     * Sets the margin width relative to the parent component width, rather than
     * relative to folder width.  The actual margin will be calculated using
     * this value whenever the folder's size changes.
     * 
     * @param margin  If this value is negative, folder margins will not scale
     *                 based on the size of the parent component.
     */
    void setParentRelativeMargin(float margin);

private:

    /**
     * Resizes padding, then calls AppFolder::resized().
     */
    void resized() override;

    int currentPage = 0;

    float parentRelativeMargin = -1;

    //############################  PageMenuButton  ###########################
    /**
     * Custom menu button for paged folders.
     */
    class PageMenuButton : public AppMenuButton
    {
    public:
        /**
         * @param menuItem    This sets the button's menu data.
         * 
         * @param name        Sets the internal component name.
         */
        PageMenuButton(
                AppMenuItem::Ptr menuItem,
                juce::String name);

        virtual ~PageMenuButton() { }
    private:

        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;

        /**
         * Set the background and border to only draw for selected buttons.
         */
        void selectionStateChanged() override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageMenuButton)
    };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageAppFolder)
};