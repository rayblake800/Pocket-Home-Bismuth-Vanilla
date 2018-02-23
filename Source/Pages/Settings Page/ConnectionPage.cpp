#include "../../Utils.h"
#include "../../../JuceLibraryCode/JuceHeader.h"
#include "ConnectionPage.h"

template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionPage() :
PageComponent("ConnectionPage",{}, true),
nextPageBtn("pageDownIcon.svg"),
prevPageBtn("pageUpIcon.svg"),
selectedConnection(ConnectionPoint::null)
{
    nextPageBtn.addListener(this);
    prevPageBtn.addListener(this);
}

template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::~ConnectionPage() { }

/**
 * @return the selected connection's reference
 */
template<class ConnectionPoint>
const ConnectionPoint& ConnectionPage<ConnectionPoint>::
getSelectedConnection()
{
    return selectedConnection;
};

/**
 * Sets ConnectionPoint connection as the selected connection.  When a
 * connection is selected, this page will show details and controls for that
 * connection, and the other connections on the list will be hidden.  When
 * set to ConnectionPoint::null, the full ConnectionPoint list will be shown 
 * again.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::setSelectedConnection
(const ConnectionPoint& connection)
{
    bool selectionChanged = false;
    if (connection == ConnectionPoint::null
        || !connections.contains(connection))
    {
        if (selectedConnection != ConnectionPoint::null)
        {
            selectionChanged = true;
        }
        selectedConnection = ConnectionPoint::null;
    }
    else if (connection != selectedConnection)
    {
        setWantsKeyboardFocus(true);
        selectionChanged = true;
        selectedConnection = connection;
    }
    if (selectionChanged)
    {
        layoutConnectionPage();
    }
};

/**
 * Remove all items from the connection list, and clear the page layout.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::clearConnectionList()
{
    connectionIndex = 0;
    selectedConnection = ConnectionPoint::null;
    for (ConnectionListItem* listItem : connectionItems)
    {
        removeChildComponent(listItem);
    }
    connectionItems.clear();
    connections.clear();
    updateLayout({});
}

/**
 * Reloads the list of connections and updates the page.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::updateConnectionList()
{
    clearConnectionList();
    connections = loadConnectionList();
    for (const ConnectionPoint& connection : connections)
    {
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

/**
 * Reloads the page layout.  This method is responsible for updating the 
 * page when scrolling through the connection list or opening/closing 
 * connection details.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::layoutConnectionPage()
{
    GridLayoutManager::Layout layout;
    bool showList = (selectedConnection == ConnectionPoint::null);
    layout.push_back({1,
        {
            {(connectionIndex > 0 && showList) ? &prevPageBtn : nullptr, 1}
        }});
    int rowWeight = 2;
    if (!showList)
    {
        rowWeight *= connectionsPerPage;
    }
    for (int i = connectionIndex;
         i < connectionIndex + connectionsPerPage; i++)
    {
        ConnectionPoint connection = (i < connectionItems.size() && i >= 0) ?
                connectionItems[i]->getConnection() : ConnectionPoint::null;
        if (showList || connection == selectedConnection)
        {
            ConnectionListItem* listItem = (i < connectionItems.size()) ?
                    connectionItems[i] : nullptr;
            if (listItem != nullptr)
            {
                if (connection == selectedConnection)
                {
                    listItem->setControlLayout
                            (getConnectionControlsLayout(connections[i]));
                }
                else
                {
                    listItem->setBasicLayout();
                }
            }
            layout.push_back({rowWeight,
                {
                    {listItem, 1}
                }});
        }
    }
    layout.push_back({1,
        {
            {(connectionItems.size() > connectionIndex + connectionsPerPage
              && showList) ? &nextPageBtn : nullptr, 1}
        }});
    updateLayout(layout);
}

/**
 * Update the connection list when the page is first added to the
 * page stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageAddedToStack()
{
    updateConnectionList();
};

/**
 * Clear the connection list when the page is removed from the page stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageRemovedFromStack()
{
    clearConnectionList();
};

/**
 * Update the connection list when the page is revealed on the page stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageRevealedOnStack()
{
    updateConnectionList();
};

/**
 * Clear the connection list when the page is no longer on top of the page
 * stack.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::pageCoveredOnStack()
{
    clearConnectionList();
};

/**
 * Handles connection list scrolling and connection selection.
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
            layoutConnectionPage();
        }
    }
    else if (button == &nextPageBtn)
    {
        if ((connectionIndex + connectionsPerPage) < connections.size())
        {

            connectionIndex += connectionsPerPage;
            layoutConnectionPage();
        }
    }
    else
    {
        for (ConnectionListItem* listItem : connectionItems)
        {
            if (listItem->ownsButton(button))
            {
                if (selectedConnection != listItem->getConnection())
                {
                    setSelectedConnection(listItem->getConnection());
                }
                else
                {
                    setSelectedConnection(ConnectionPoint::null);
                }
                return;
            }
        }
        connectionButtonClicked(button);
    }
};

/**
 * Close connection controls when esc is pressed.
 */
template<class ConnectionPoint>
bool ConnectionPage<ConnectionPoint>::keyPressed(const KeyPress& key)
{
    if (key == KeyPress::escapeKey)
    {
        setSelectedConnection(ConnectionPoint::null);
    }
};

/**
 * Create a new ConnectionListItem representing a ConnectionPoint
 */
template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionListItem::
ConnectionListItem
(const ConnectionPoint& connection, Button* connectionButton) :
connection(connection),
connectionButton(connectionButton)
{
    setBasicLayout();
};

template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionListItem::
~ConnectionListItem() { };

/**
 * @return the connection represented by this component.
 */
template<class ConnectionPoint>
const ConnectionPoint& ConnectionPage<ConnectionPoint>::ConnectionListItem::
getConnection()
{
    return connection;
};

/**
 * Switch to connection point control mode.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::
setControlLayout
(GridLayoutManager::Layout detailLayout)
{
    detailLayout.insert(detailLayout.begin(), getBasicLayout()[0]);
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(detailLayout, this);
    controlLayout = true;
};

/**
 * Load the basic layout, which only shows the connectionButton.
 * This loads the initial layout, and restores it when closing
 * connection controls.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::
setBasicLayout()
{
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(getBasicLayout(), this);
    controlLayout = false;
};

/**
 * @return true iff Button* button is this list item's connectionButton.
 */
template<class ConnectionPoint>
bool ConnectionPage<ConnectionPoint>::ConnectionListItem::
ownsButton(Button* button)
{
    return button == connectionButton;
};

/**
 * @return the component's layout when not showing connection controls.
 */
template<class ConnectionPoint>
GridLayoutManager::Layout ConnectionPage<ConnectionPoint>::ConnectionListItem::
getBasicLayout()
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
    static Image minimize = createImageFromFile(assetFile("pageUpIcon.svg"));
    g.setColour(findColour(ListBox::ColourIds::backgroundColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
    if (controlLayout)
    {
        int height = connectionButton->getHeight()/2;
        g.drawImage(minimize,
                borderWidth*2, borderWidth*2, height, height,
                0, 0, minimize.getWidth(), minimize.getHeight());
    }
}

/**
 * Scale the layout to fit the new bounds.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::ConnectionListItem::
resized()
{
    Rectangle<int> bounds = getLocalBounds().reduced(borderWidth,
            borderWidth);
    listItemLayout.layoutComponents(bounds,
            bounds.getWidth() / 20, bounds.getHeight() / 20);
}

