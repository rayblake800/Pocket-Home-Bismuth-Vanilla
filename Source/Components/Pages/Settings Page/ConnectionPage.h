#pragma once
#include "ScalingLabel.h"
#include "SwitchComponent.h"
#include "NavButton.h"
#include "LayoutManager.h"
#include "PageComponent.h"
#include "PageStackComponent.h"

/**
 * @file ConnectionPage.h
 * 
 * @brief  Defines an abstract base for pages that select network connections.
 * 
 * Connection page is the abstract base for PageComponent classes that
 * show a list of ConnectionPointa and allow the user to connect or disconnect
 * from ConnectionPoints in the list.
 * 
 * ConnectionPoint objects represent a potential network connection point, such
 * as a wifi access point or a bluetooth device. These objects must have an 
 * isNull() method that returns true iff the object is invalid.  ConnectionPoint
 * objects created with the default constructor should always be null.  
 * 
 * In debug builds, ConnectionPoint should also have a toString() method that 
 * returns some sort of identifying string for debug output.
 */

template<class ConnectionPoint>
class ConnectionPage : public PageComponent {
public:

    ConnectionPage();

    virtual ~ConnectionPage() { }

protected:
    /**
     * Returns the current selected connection point.
     * 
     * @return the selected connection point.
     */
    const ConnectionPoint& getSelectedConnection();


    /**
     * Sets the selected connection.  When a connection is selected, this page 
     * will show details and controls for that connection, and the other 
     * connections on the list will be hidden.  When set to the null
     * connectionPoint, the full ConnectionPoint list will be shown again.
     * 
     * @param connectionPt  This must be null, or equal to one of the 
     *                      connections in the connection list.
     */
    void setSelectedConnection(const ConnectionPoint& connectionPt);

    /**
     * Add a new connection point to the page, and update page layout.
     * 
     * @param addedCP  A newly detected connection point to add to the list.
     */
    void addConnectionPoint(const ConnectionPoint& addedCP);
    
    /**
     * Remove a connectionPoint from the page, and update page layout.
     * 
     * @param removedCP  A connection point to remove from the list.
     */
    void removeConnectionPoint(const ConnectionPoint& removedCP);
    
    /**
     * Updates a connection point's list component.
     * 
     * @param updatedCP  A connection point that's changed in some way that
     *                   requires its list component to be re-created.
     */
    void updateConnectionPoint(const ConnectionPoint& updatedCP);
    
    /**
     * Remove all items from the connection list, and clear the page layout.
     */
    void clearConnectionList();

    /**
     * Reloads the list of connections and updates the page.
     */
    void updateConnectionList();

    /**
     * Sorts the list of connection points, checks the index, and reloads the
     * page layout if necessary.  This method is used to update the page
     * when changing the selected index, adding and removing connection points,
     * and opening and closing connection point details.
     */
    void layoutConnectionPage();
    
    /**
     * This method will run every time the page component bounds change.
     * Inheriting classes that need to act whenever page bounds change should
     * override this method instead of pageResized().
     */
    virtual void connectionPageResized() { }

private:
    /**
     * Returns the list of all connection point objects that should be listed.
     * 
     * @return  the complete list of connection points.
     */
    virtual Array<ConnectionPoint> loadConnectionPoints() = 0;
    
    /**
     * This method will be used to sort the list of connection points.
     * 
     * @param first    Some connection point in the list.
     * 
     * @param second   Some other connection point in the list.
     * 
     * @return  a negative number if first should come before second, zero if 
     *          the two connection points are equal, or a positive number if
     *          second should come before first.
     */
    virtual int compareConnectionPoints
    (const ConnectionPoint& first, const ConnectionPoint& second) = 0;

    /**
     * Construct a button component to represent a network connection point.
     * The generic ConnectionPage class will handle listening to button events,
     * and deallocating the button when it is no longer needed.
     * 
     * This button will be displayed in a list with other connection buttons, 
     * and when clicked, it will toggle the visibility of the connection 
     * controls.  
     * 
     * @param connectionPt  One of the connection points listed on this page.
     * 
     * @return  a dynamically allocated Button component that displays basic 
     *          connection information.  
     */
    virtual Button* getConnectionButton
    (const ConnectionPoint& connectionPt) = 0;

    /**
     * This is called whenever any button other than the navigation buttons
     * is clicked.  Inheriting classes should override this method if they
     * need to handle button events.
     * 
     * @param button  A button that was clicked.
     */
    virtual void connectionButtonClicked(Button* button) = 0;

    /**
     * Get a set of layout parameters defining controls for a connection
     * list item.  Since connection controls are only shown for one connection
     * at a time, the ConnectionPage should reuse a a single set of control
     * components for every call to this method.
     * 
     * @param connectionPt   The connection point that the control layout will
     *                       affect.
     */
    virtual LayoutManager::Layout
    getConnectionControlsLayout(const ConnectionPoint& connectionPt) = 0;

    /**
     * Update the connection control components for a given connection point.
     * This should be called whenever the selected connection changes state or
     * is replaced.
     */
    virtual void updateConnectionControls() = 0;
    
    /**
     * Updates the connection point list when the page is first added to the
     * page stack.
     */
    virtual void pageAddedToStack() override;

    /**
     * Updates the connection point list when the page is revealed on the page 
     * stack.
     */
    virtual void pageRevealedOnStack() override;

    /**
     * Handles connection list scrolling and connection selection.  Other button
     * events will be passed to the connectionButtonClicked() method.
     * 
     * @button  A button that was clicked by the user.
     */
    void pageButtonClicked(Button* button) override;
    
        
    /**
     * When connection controls are open, override the back button to close
     * connection controls instead of closing the page.
     * 
     * @return true if connection controls were open when the back button was
     *         clicked.
     */
    bool overrideBackButton() override;

    /**
     * Close connection controls when the escape key is pressed.
     * 
     * @param key  represents a key pressed by the user.
     * 
     * @return  true iff the key pressed was the escape key.
     */
    virtual bool keyPressed(const KeyPress& key) override;
    
    /**
     * Update list navigation buttons to fit the page.
     */
    void pageResized() final override;
     
    /**
     * Finds the index of a ConnectionPoint in the connection button list.
     * 
     * @param toFind  The connection point to find within the list.
     * 
     * @return  The index of toFind's ConnectionListItem in the ConnectionItem
     *          array, or -1 if no matching list item is found.
     */
    int connectionPointIndex(const ConnectionPoint& toFind);
    
    /**
     * Holds the connection button and connection controls for a single
     * connection point. Clicking the connection button toggles visibility
     * of connection controls.
     */
    class ConnectionListItem : public Component {
    public:

        /**
         * Creates a new ConnectionListItem representing a ConnectionPoint.
         * 
         * @param connectionPt      The represented connection point.
         * 
         * @param connectionButton  A button component generated by the
         *                          ConnectionPage::getConnectionButton method 
         *                          using connectionPt.
         */
        ConnectionListItem
        (ConnectionPoint connectionPt, Button* connectionButton);

        virtual ~ConnectionListItem() { }

        /**
         * Gets this button's connection point.
         * 
         * @return the connection point represented by this component.
         */
        const ConnectionPoint& getConnectionPt();

        /**
         * Switches to connection point control mode, filling the entire
         * connection page.
         * 
         * @param detailLayout  Contains the layout generated by
         *                      getConnectionControlsLayout().
         */
        void setControlLayout(LayoutManager::Layout detailLayout);

        /**
         * Load the basic button layout used when showing the entire list.
         * This loads the initial layout, and restores it when closing
         * connection controls.
         */
        void setListItemLayout();

        /**
         * Checks if a button is the one attached to this list item.
         * 
         * @param button  A button component to check.
         * 
         * @return true iff the button is this list item's connectionButton.
         */
        bool ownsButton(Button* button);
        
    private:
        /**
         * Returns the layout of an unselected list item.
         * 
         * @return the component's layout when not showing connection controls.
         */
        LayoutManager::Layout getListItemLayout();

        /**
         * Draws an outline around the entire component, with a width of 
         * ConnectionPage<ConnectionPoint>::ConnectionListItem::borderWidth
         * and color set by the ListBox backgroundColourId.
         * 
         * TODO: define a unique ColourId for the outline.
         * 
         * @param g  Graphics context to use for drawing operations.
         */
        void paint(Graphics &g) override;

        /**
         * Scale the layout to fit the new bounds.
         */
        void resized() override;
        
        //Clicked to select this ConnectionPoint
        ScopedPointer<Button> connectionButton;
        LayoutManager listItemLayout;
        ConnectionPoint connectionPt;
        static const constexpr int borderWidth = 4;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionListItem)
    };
   
public:
    /**
     * Allows ConnectionPage to sort connection list item components.
     * 
     * @param first   A list component.
     * 
     * @param second  Another list component.
     * 
     * @return  a negative number if first should come before second, zero if 
     *          the two components are equal, or a positive number if second 
     *          should come before first.
     */
    int compareElements(ConnectionListItem* first, ConnectionListItem* second);
    
private:
    //scroll through the list of connections
    NavButton prevPageBtn;
    NavButton nextPageBtn;
    static const constexpr int connectionsPerPage = 4;
    int connectionIndex = 0;
    
    OwnedArray<ConnectionListItem> connectionItems;
    ConnectionPoint selectedConnectionPt;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionPage)
};

#include "ConnectionPage.cpp"
