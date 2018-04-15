#include "JuceHeader.h"
#include "Utils.h"
#include "ConnectionPage.h"

template<class ConnectionPtr>
ConnectionPage<ConnectionPtr>::ConnectionPage() :
PageComponent("ConnectionPage",{}, true),
prevPageBtn(ComponentConfigFile::pageUpKey),
nextPageBtn(ComponentConfigFile::pageDownKey),
selectedConnection(nullptr)
{
    prevPageBtn.addListener(this);
    nextPageBtn.addListener(this);
    addAndMakeVisible(prevPageBtn);
    addAndMakeVisible(nextPageBtn);
}


/**
 * @return the selected connection's reference
 */
template<class ConnectionPtr>
ConnectionPtr ConnectionPage<ConnectionPtr>::getSelectedConnection()
{
    return selectedConnection;
};

/**
 * Sets ConnectionPtr's connection as the selected connection.  When a
 * connection is selected, this page will show details and controls for that
 * connection, and the other connections on the list will be hidden.  When
 * set to nullptr, the full ConnectionPoint list will be shown 
 * again.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::setSelectedConnection
(ConnectionPtr connection)
{
    bool selectionChanged = false;
    if (connection == nullptr
        || !connections.contains(connection))
    {
        if (selectedConnection != nullptr)
        {
            selectionChanged = true;
            selectedConnection = nullptr;
        }
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
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::clearConnectionList()
{
    connectionIndex = 0;
    selectedConnection = nullptr;
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
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::updateConnectionList()
{
    clearConnectionList();
    connections = loadConnectionList();
    for (ConnectionPtr connection : connections)
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
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::layoutConnectionPage()
{
    RelativeLayoutManager::Layout layout;
    bool showList = (selectedConnection == nullptr);
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
        ConnectionPtr connection 
                = (i < connectionItems.size() && i >= 0) ?
                connectionItems[i]->getConnection() : nullptr;
        if (showList || connection == selectedConnection)
        {
            ConnectionListItem* listItem = (i < connectionItems.size()) ?
                    connectionItems[i] : nullptr;
            if (listItem != nullptr)
            {
                if (connection == selectedConnection
                    && connection != nullptr)
                {
                    listItem->setControlLayout
                            (getConnectionControlsLayout(connection));
                    updateConnectionControls(selectedConnection);
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
    updateLayout(layout);
}

/**
 * Update the connection list when the page is first added to the
 * page stack.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::pageAddedToStack()
{
    updateConnectionList();
};

/**
 * Update the connection list when the page is revealed on the page stack.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::pageRevealedOnStack()
{
    updateConnectionList();
};


/**
 * Handles connection list scrolling and connection selection.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::pageButtonClicked(Button* button)
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
                    << connectionIndex << " of " << connections.size());
            layoutConnectionPage();
        }
    }
    else if (button == &nextPageBtn)
    {
        if ((connectionIndex + connectionsPerPage) < connections.size())
        {

            connectionIndex += connectionsPerPage;

            DBG("ConnectionPage::" << __func__ << ": Connection index set to "
                    << connectionIndex << " of " << connections.size());
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
                    setSelectedConnection(nullptr);
                }
                return;
            }
        }
        connectionButtonClicked(button);
    }
};

/**
 * When connection controls are open, override the back button to close
 * connection controls instead of closing the page.
 * 
 * @return true if connection controls were open when the back button was
 * clicked.
 */
template<class ConnectionPtr>
bool ConnectionPage<ConnectionPtr>::overrideBackButton()
{
    if (selectedConnection == nullptr)
    {
        return false;
    }
    DBG("ConnectionPage::" << __func__ << ": deselecting connection "
            << selectedConnection->toString());
    setSelectedConnection(nullptr);
    return true;
}

/**
 * Close connection controls when esc is pressed.
 */
template<class ConnectionPtr>
bool ConnectionPage<ConnectionPtr>::keyPressed(const KeyPress& key)
{
    if (key == KeyPress::escapeKey)
    {
        setSelectedConnection(nullptr);
    }
};

/**
 * Update list navigation buttons to fit the page.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::pageResized()
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

/**
 * Create a new ConnectionListItem representing a ConnectionPtr
 */
template<class ConnectionPtr>
ConnectionPage<ConnectionPtr>::ConnectionListItem::ConnectionListItem
(ConnectionPtr connection, Button* connectionButton) :
connection(connection),
connectionButton(connectionButton)
{
    setBasicLayout();
};

template<class ConnectionPtr>
ConnectionPage<ConnectionPtr>::ConnectionListItem::~ConnectionListItem() { }

/**
 * @return the connection represented by this component.
 */
template<class ConnectionPtr>
ConnectionPtr ConnectionPage<ConnectionPtr>::ConnectionListItem::
getConnection()
{
    return connection;
};

/**
 * Switch to connection point control mode.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::ConnectionListItem::setControlLayout
(RelativeLayoutManager::Layout detailLayout)
{
    detailLayout.insert(detailLayout.begin(), getBasicLayout()[0]);
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(detailLayout, this);
};

/**
 * Load the basic layout, which only shows the connectionButton.
 * This loads the initial layout, and restores it when closing
 * connection controls.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::ConnectionListItem::setBasicLayout()
{
    listItemLayout.clearLayout(true);
    listItemLayout.setLayout(getBasicLayout(), this);
};

/**
 * @return true iff Button* button is this list item's connectionButton.
 */
template<class ConnectionPtr>
bool ConnectionPage<ConnectionPtr>::ConnectionListItem::ownsButton
(Button* button)
{
    return button == connectionButton;
};

/**
 * @return the component's layout when not showing connection controls.
 */
template<class ConnectionPtr>
RelativeLayoutManager::Layout 
ConnectionPage<ConnectionPtr>::ConnectionListItem::getBasicLayout()
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
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::ConnectionListItem::
paint(Graphics &g)
{
    g.setColour(findColour(Label::ColourIds::textColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}

/**
 * Scale the layout to fit the new bounds.
 */
template<class ConnectionPtr>
void ConnectionPage<ConnectionPtr>::ConnectionListItem::
resized()
{
    Rectangle<int> bounds = getLocalBounds().reduced(borderWidth,
            borderWidth);
    listItemLayout.layoutComponents(bounds,
            bounds.getWidth() / 20, bounds.getHeight() / 20);
}

