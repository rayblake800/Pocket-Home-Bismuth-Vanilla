#include "JuceHeader.h"
#include "Utils.h"
#include "ConnectionPage.h"

template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionPage() :
PageComponent("ConnectionPage",{}, true),
prevPageBtn(ComponentConfigFile::pageUpKey),
nextPageBtn(ComponentConfigFile::pageDownKey)
{
    prevPageBtn.addListener(this);
    nextPageBtn.addListener(this);
    addAndMakeVisible(prevPageBtn);
    addAndMakeVisible(nextPageBtn);
}


/*
 * Returns the current selected connection point.
 */
template<class ConnectionPoint>
ConnectionPoint ConnectionPage<ConnectionPoint>::getSelectedConnection()
{
    return selectedConnectionPt;
};

/*
 * Sets the selected connection.  When a connection is selected, this page 
 * will show details and controls for that connection, and the other 
 * connections on the list will be hidden.  When set to the null
 * connectionPoint, the full ConnectionPoint list will be shown again.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::setSelectedConnection
(ConnectionPoint connectionPt)
{
    bool selectionChanged = false;
    DBG("Selecting " << connectionPt.toString());
    if (connectionPt.isNull()
        || !connectionPts.contains(connectionPt))
    {
        if (!selectedConnectionPt.isNull())
        {
            selectionChanged = true;
            selectedConnectionPt = ConnectionPoint();
        }
    }
    else if (connectionPt != selectedConnectionPt)
    {
        setWantsKeyboardFocus(true);
        selectionChanged = true;
        selectedConnectionPt = connectionPt;
    }
    if (selectionChanged)
    {
        layoutConnectionPage();
    }
};

/*
 * Remove all items from the connection list, and clear the page layout.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::clearConnectionList()
{
    connectionIndex = 0;
    selectedConnectionPt = ConnectionPoint();
    for (ConnectionListItem* listItem : connectionItems)
    {
        removeChildComponent(listItem);
    }
    connectionItems.clear();
    connectionPts.clear();
    updateLayout({});
}

/*
 * Reloads the list of connections and updates the page.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::updateConnectionList()
{
    clearConnectionList();
    connectionPts = loadConnectionPoints();
    DBG("creating connection list with " << connectionPts.size() <<
            "connection points");
    for (const ConnectionPoint& connection : connectionPts)
    {
        DBG("Adding connection button for " << addressID(&connection));
        jassert(!connection.isNull());
        Button * connectionButton = getConnectionButton(connection);
        connectionButton->addListener(this);
        ConnectionListItem* listItem = new ConnectionListItem(connection,
                connectionButton);
        if (isConnected(connection))
        {
            connectionItems.insert(0, listItem);
        }
        else
        {
            connectionItems.add(listItem);
        }
    }
    layoutConnectionPage();
}

/*
 * Reloads the page layout. This method is responsible for updating the page 
 * when scrolling through the connection list or opening/closing connection 
 * details.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::layoutConnectionPage()
{
    RelativeLayoutManager::Layout layout;
    bool showList = selectedConnectionPt.isNull();
    prevPageBtn.setVisible(connectionIndex > 0 && showList);
    nextPageBtn.setVisible(connectionItems.size() > connectionIndex
            + connectionsPerPage && showList);
    int rowWeight = 2;
    if (!showList)
    {
        rowWeight *= connectionsPerPage;
    }
    for (int i = connectionIndex;
         i < connectionIndex + connectionsPerPage; i++)
    {
        ConnectionPoint connectionPt
                = (i < connectionItems.size() && i >= 0) ?
                connectionItems[i]->getConnectionPt() : ConnectionPoint();
        if (showList || connectionPt == selectedConnectionPt)
        {
            ConnectionListItem* listItem = (i < connectionItems.size()) ?
                    connectionItems[i] : nullptr;
            if (listItem != nullptr)
            {
                if (connectionPt == selectedConnectionPt
                    && !connectionPt.isNull())
                {
                    listItem->setControlLayout
                            (getConnectionControlsLayout(connectionPt));
                    updateConnectionControls();
                }
                else
                {
                    listItem->setListItemLayout();
                }
            }
            layout.push_back({rowWeight,
                {
                    {listItem, 1}
                }});
        }
    }
    updateLayout(layout);
}

/*
 * Updates the connection point list when the page is first added to the page 
 * stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageAddedToStack()
{
    updateConnectionList();
};

/*
 * Updates the connection point list when the page is revealed on the page 
 * stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageRevealedOnStack()
{
    updateConnectionList();
};


/*
 * Handles connection list scrolling and connection point selection.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageButtonClicked(Button* button)
{
    if (button == &prevPageBtn)
    {
        if (connectionIndex > 0)
        {
            connectionIndex -= connectionsPerPage;
            if (connectionIndex < 0)
            {
                connectionIndex = 0;
            }
            DBG("ConnectionPage::" << __func__ << ": Connection index set to "
                    << connectionIndex << " of " << connectionPts.size());
            layoutConnectionPage();
        }
    }
    else if (button == &nextPageBtn)
    {
        if ((connectionIndex + connectionsPerPage) < connectionPts.size())
        {
            connectionIndex += connectionsPerPage;
            DBG("ConnectionPage::" << __func__ << ": Connection index set to "
                    << connectionIndex << " of " << connectionPts.size());
            layoutConnectionPage();
        }
    }
    else
    {
        for (ConnectionListItem* listItem : connectionItems)
        {
            if (listItem->ownsButton(button))
            {
                if (selectedConnectionPt != listItem->getConnectionPt())
                {
                    setSelectedConnection(listItem->getConnectionPt());
                }
                else
                {
                    setSelectedConnection(ConnectionPoint());
                }
                return;
            }
        }
        connectionButtonClicked(button);
    }
};

/*
 * When connection controls are open, override the back button to close
 * connection controls instead of closing the page.
 */
template<class ConnectionPoint>
bool ConnectionPage<ConnectionPoint>::overrideBackButton()
{
    if (selectedConnectionPt.isNull())
    {
        return false;
    }
    DBG("ConnectionPage::" << __func__ << ": deselecting connection "
            << selectedConnectionPt.toString());
    setSelectedConnection(ConnectionPoint());
    return true;
}

/*
 * Close connection controls when the escape key is pressed.
 */
template<class ConnectionPoint>
bool ConnectionPage<ConnectionPoint>::keyPressed(const KeyPress& key)
{
    if (key == KeyPress::escapeKey)
    {
        setSelectedConnection(ConnectionPoint());
        return true;
    }
    return false;
};

/*
 * Update list navigation buttons to fit the page.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageResized()
{
    if (!getBounds().isEmpty())
    {
        prevPageBtn.applyConfigBounds();
        nextPageBtn.applyConfigBounds();
        setMarginFraction((float) (prevPageBtn.getBottom() + getWidth() / 50)
                / (float) getHeight());
        layoutComponents();
    }
    connectionPageResized();
}

/*
 * Create a new ConnectionListItem representing a ConnectionPoint
 */
template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionListItem::ConnectionListItem
(ConnectionPoint connectionPt, Button* connectionButton) :
connectionPt(connectionPt),
connectionButton(connectionButton)
{
    setListItemLayout();
}

/*
 * Gets this button's connection point.
 */
template<class ConnectionPoint>
ConnectionPoint ConnectionPage<ConnectionPoint>::ConnectionListItem::
getConnectionPt()
{
    return connectionPt;
}

/*
 * Switches to connection point control mode, filling the entire
 * connection page.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::setControlLayout
(RelativeLayoutManager::Layout detailLayout)
{
    detailLayout.insert(detailLayout.begin(), getListItemLayout()[0]);
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(detailLayout, this);
}

/*
 * Load the basic button layout used when showing the entire list.
 * This loads the initial layout, and restores it when closing
 * connection controls.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::setListItemLayout()
{
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(getListItemLayout(), this);
};

/*
 * Checks if a button is the one attached to this list item.
 */
template<class ConnectionPoint>
bool ConnectionPage<ConnectionPoint>::ConnectionListItem::ownsButton
(Button* button)
{
    return button == connectionButton;
};

/*
 * Returns the layout of an unselected list item.
 */
template<class ConnectionPoint>
RelativeLayoutManager::Layout 
ConnectionPage<ConnectionPoint>::ConnectionListItem::getListItemLayout()
{
    return {
        { 3,
            {
                {connectionButton, 1}
            }}};
}

/**
 * Draws an outline around the entire component, with a width of 
 * ConnectionPage<ConnectionPoint>::ConnectionListItem::borderWidth
 * and color set by the ListBox backgroundColourId
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::
paint(Graphics &g)
{
    g.setColour(findColour(Label::ColourIds::textColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}

/**
 * Scale the layout to fit the new bounds.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::resized()
{
    Rectangle<int> bounds = getLocalBounds().reduced(borderWidth,
            borderWidth);
    listItemLayout.layoutComponents(bounds,
            bounds.getWidth() / 20, bounds.getHeight() / 20);
}

