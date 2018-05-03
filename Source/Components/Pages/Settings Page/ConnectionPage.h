#pragma once
#include "ScalingLabel.h"
#include "SwitchComponent.h"
#include "ConfigurableImageButton.h"
#include "RelativeLayoutManager.h"
#include "PageComponent.h"
#include "PageStackComponent.h"

/**
 * @file ConnectionPage.h
 * 
 * @brief  Defines an abstract base for pages that select network connections.
 * 
 * Connection page is the abstract base for PageComponent classes that
 * show a list of connections and allow the user to connect or disconnect
 * from connections in the list.
 * 
 * ConnectionPoint objects represent a potential connection point.  These
 * objects must have an isNull() method that returns true iff the object is
 * invalid.  ConnectionPoint objects created with the default constructor should
 * always be null.  
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
    ConnectionPoint getSelectedConnection();


    /**
     * Sets the selected connection.  When a connection is selected, this page 
     * will show details and controls for that connection, and the other 
     * connections on the list will be hidden.  When set to the null
     * connectionPoint, the full ConnectionPoint list will be shown again.
     * 
     * @param connectionPt  This must be null, or equal to one of the 
     *                      connections in the connection list.
     */
    void setSelectedConnection(ConnectionPoint connectionPt);


    /**
     * Remove all items from the connection list, and clear the page layout.
     */
    void clearConnectionList();

    /**
     * Reloads the list of connections and updates the page.
     */
    void updateConnectionList();

    /**
     * Reloads the page layout.  This method is responsible for updating the 
     * page when scrolling through the connection list or opening/closing 
     * connection details.
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
     * Checks if the system is currently connected to a specific connection
     * point.
     * 
     * @param connectionPt  The connection point that may be connected
     * 
     * @return true iff a connection exists that is using this ConnectionPoint.
     */
    virtual bool isConnected(ConnectionPoint connectionPt) = 0;

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
    virtual Button* getConnectionButton(ConnectionPoint connectionPt) = 0;

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
    virtual RelativeLayoutManager::Layout
    getConnectionControlsLayout(ConnectionPoint connectionPt) = 0;

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
        ConnectionPoint getConnectionPt();

        /**
         * Switches to connection point control mode, filling the entire
         * connection page.
         * 
         * @param detailLayout  Contains the layout generated by
         *                      getConnectionControlsLayout().
         */
        void setControlLayout(RelativeLayoutManager::Layout detailLayout);

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
        RelativeLayoutManager::Layout getListItemLayout();

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
        RelativeLayoutManager listItemLayout;
        ConnectionPoint connectionPt = nullptr;
        static const constexpr int borderWidth = 4;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionListItem)
    };
    //scroll through the list of connections
    ConfigurableImageButton prevPageBtn;
    ConfigurableImageButton nextPageBtn;
    static const constexpr int connectionsPerPage = 4;
    int connectionIndex = 0;
    
    OwnedArray<ConnectionListItem> connectionItems;
    Array<ConnectionPoint> connectionPts;
    ConnectionPoint selectedConnectionPt;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionPage)
};

#include "ConnectionPage.cpp"
