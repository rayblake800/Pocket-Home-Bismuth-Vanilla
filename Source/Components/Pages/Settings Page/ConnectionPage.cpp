#include "JuceHeader.h"
#include "Utils.h"
#include "ConnectionPage.h"

template<class ConnectionPoint>
ConnectionPage<ConnectionPoint>::ConnectionPage() :
PageComponent("ConnectionPage"),
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
const ConnectionPoint& ConnectionPage<ConnectionPoint>::getSelectedConnection()
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
(const ConnectionPoint& connectionPt)
{
    if (connectionPt.isNull())
    {
        if (!selectedConnectionPt.isNull())
        {
            DBG("ConnectionPage::" << __func__ << ": De-selecting "
                    << selectedConnectionPt.toString());
            selectedConnectionPt = ConnectionPoint();
        }
        else
        {
            if (!connectionPt.isNull())
            {
                DBG("ConnectionPage::" << __func__ << ": Connection point "
                        << connectionPt.toString() << " not found!");
            }
            return;
        }
    }
    else if (connectionPointIndex(connectionPt) == -1)
    {
        DBG("ConnectionPage::" << __func__ << ": Connection point "
                << connectionPt.toString() << " not found!");
        return;
    }
    else
    {
        DBG("Selecting " << connectionPt.toString());
        setWantsKeyboardFocus(true);
        selectedConnectionPt = connectionPt;
    }
    layoutConnectionPage();
};

/*
 * Add a new connection point to the page, and update page layout.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::addConnectionPoint
(const ConnectionPoint& addedCP)
{
    if (!addedCP.isNull() && connectionPointIndex(addedCP) == -1)
    {
        Button * connectionButton = getConnectionButton(addedCP);
        connectionButton->addListener(this);
        connectionItems.add(new ConnectionListItem(addedCP,
                connectionButton));
        layoutConnectionPage();
    }
    else
    {
        DBG("ConnectionPage::" << __func__
                << ": Tried to add invalid connection point!");
    }
}

/*
 * Remove a connectionPoint from the page, and update page layout.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::removeConnectionPoint
(const ConnectionPoint& removedCP)
{
    if (selectedConnectionPt == removedCP)
    {
        DBG("ConnectionPage::" << __func__ << ": De-selecting "
                << selectedConnectionPt.toString() << " before it is removed.");
        setSelectedConnection(ConnectionPoint());
    }
    int itemIndex = connectionPointIndex(removedCP);
    if (itemIndex >= 0)
    {
        setLayout({});
        connectionItems.remove(itemIndex);
        layoutConnectionPage();
    }
    else
    {
        DBG("ConnectionPage::" << __func__ << ": couldn't find connectionPoint "
                << removedCP.toString() << " in the page list.");
    }

}

/*
 * Updates a connection point's list component.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::updateConnectionPoint
(const ConnectionPoint& updatedCP)
{

    int itemIndex = connectionPointIndex(updatedCP);
    if (itemIndex >= 0)
    {
        setLayout({});
        connectionItems.remove(itemIndex);
        Button * connectionButton = getConnectionButton(updatedCP);
        connectionButton->addListener(this);
        connectionItems.add(new ConnectionListItem(updatedCP,
                connectionButton));

        //Selected connection must be checked and possibly updated because two
        //connectionPoint objects can be equal despite having different
        //properties.
        if (selectedConnectionPt == updatedCP)
        {
            setSelectedConnection(updatedCP);
        }
        else
        {
            layoutConnectionPage();
        }
    }
    else
    {
        DBG("ConnectionPage::" << __func__ << ": couldn't find connectionPoint "
                << updatedCP.toString() << " in the page list.");
    }

}

/*
 * Remove all items from the connection list, and clear the page layout.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::clearConnectionList()
{
    connectionIndex = 0;
    selectedConnectionPt = ConnectionPoint();
    setLayout({});
    connectionItems.clear();
}

/*
 * Reloads the list of connections and updates the page.
 */
template<class ConnectionPoint>
void ConnectionPage<ConnectionPoint>::updateConnectionList()
{
    clearConnectionList();
    Array<ConnectionPoint> connectionPts = loadConnectionPoints();
    for (const ConnectionPoint& connection : connectionPts)
    {
        jassert(!connection.isNull());
        Button * connectionButton = getConnectionButton(connection);
        connectionButton->addListener(this);
        ConnectionListItem* listItem = new ConnectionListItem(connection,
                connectionButton);
        connectionItems.add(listItem);
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
    LayoutManager::Layout layout;
    bool showList = selectedConnectionPt.isNull();
    prevPageBtn.setVisible(connectionIndex > 0 && showList);
    nextPageBtn.setVisible(connectionItems.size() > connectionIndex
            + connectionsPerPage && showList);
    connectionItems.sort(*this);
    DBG("Setting layout for " << connectionItems.size() << " connections");
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
            using Row = LayoutManager::Row;
            using RowItem = LayoutManager::RowItem;
            layout.addRow(Row(20,{RowItem(listItem, 6)}));
        }
    }
    layout.setYPaddingWeight(3);
    layout.setXMarginFraction(0.1);
    ComponentConfigFile config;
    ComponentConfigFile::ComponentSettings prevSettings 
            = config.getComponentSettings(prevPageBtn.getComponentKey());
    ComponentConfigFile::ComponentSettings nextSettings 
            = config.getComponentSettings(nextPageBtn.getComponentKey());
    layout.setYMarginFraction(0.01 + (float) std::max(
         prevSettings.getHeightFraction() + prevSettings.getYFraction(),
         1 - nextSettings.getYFraction()));
    setLayout(layout);
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
                    << connectionIndex << " of " << connectionItems.size());
            layoutConnectionPage();
        }
    }
    else if (button == &nextPageBtn)
    {
        if ((connectionIndex + connectionsPerPage) < connectionItems.size())
        {
            connectionIndex += connectionsPerPage;
            DBG("ConnectionPage::" << __func__ << ": Connection index set to "
                    << connectionIndex << " of " << connectionItems.size());
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
    }
    connectionPageResized();
}

/*
 * Allows ConnectionPage to sort connection list item components.
 */
template<class ConnectionPoint>
int ConnectionPage<ConnectionPoint>::compareElements
(ConnectionListItem* first, ConnectionListItem* second)
{
    return compareConnectionPoints(first->getConnectionPt(),
            second->getConnectionPt());
}

/*
 * Finds the index of a ConnectionPoint in the connection button list.
 */
template<class ConnectionPoint>
int ConnectionPage<ConnectionPoint>::connectionPointIndex
(const ConnectionPoint& toFind)
{
    if (toFind.isNull())
    {
        return -1;
    }
    for (int i = 0; i < connectionItems.size(); i++)
    {
        if (toFind == connectionItems[i]->getConnectionPt())
        {
            return i;
        }
    }
    return -1;
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
const ConnectionPoint& ConnectionPage<ConnectionPoint>::ConnectionListItem::
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
(LayoutManager::Layout detailLayout)
{
    detailLayout.insertRow(getListItemLayout().getRow(0), 0);
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
LayoutManager::Layout
ConnectionPage<ConnectionPoint>::ConnectionListItem::getListItemLayout()
{
    return LayoutManager::Layout({
        LayoutManager::Row(20,
        {
            LayoutManager::RowItem(connectionButton, 20)
        })
    });
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
    listItemLayout.layoutComponents(getLocalBounds().reduced(borderWidth,
            borderWidth));
}

