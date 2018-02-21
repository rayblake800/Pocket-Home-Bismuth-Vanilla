/*
  ==============================================================================

    ConnectionPage.cpp
    Created: 20 Feb 2018 1:36:21pm
    Author:  anthony

  ==============================================================================
 */

#include "ConnectionPage.h"

ConnectionPage::ConnectionPage() :
PageComponent("ConnectionPage",{}, true) { }

ConnectionPage::~ConnectionPage() { }

void ConnectionPage::setSelectedConnection(ConnectionPoint* connection)
{
    setWantsKeyboardFocus(true);
    if (connection == nullptr)
    {
        selectedConnection = nullptr;
    }
    else
    {
        selectedConnection = new ConnectionPoint(connection);
    }
    layoutConnectionPage();
}

void ConnectionPage::updateConnectionList()
{
    removeAllChildren();
    connectionItems.clear();
    connections = loadConnectionList();
    for (const ConnectionPoint& connection : connections)
    {
        ConnectionPoint* connectionCopy = new ConnectionListItem(connections);
        if (isConnected(connection))
        {
            connectionItems.insert(0, connectionCopy);
        }
        else
        {
            connectionItems.add(connectionCopy);
        }
    }
    layoutConnectionPage();
}

void ConnectionPage::layoutConnectionPage()
{
    std::vector<GridLayoutManager::RowLayoutParams> layout;
    bool showList = (selectedConnection == nullptr);
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
        if (showList || connectionItems[i]->getConnection
            == *selectedConnection)
        {
            layout.push_back({rowWeight,
                {
                    {(i < connectionItems.size()) ?
                        connectionItems[i] : nullptr, 1}
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

void ConnectionPage::pageButtonClicked(Button*)
{
    ConnectionListItem* connButton = dynamic_cast<ConnectionListItem *> (button);
    if (connButton != nullptr)
    {
        if (*selectedConnection != connButton->getConnection)
        {
            setSelectedConnection(*connButton->getConnection);
        }
        else
        {
            setSelectedConnection(nullptr);
        }
    }
    else if (button == &prevPageBtn)
    {
        if (apIndex > 0)
        {
            apIndex -= apItemsPerPage;
            if (apIndex < 0)
            {
                apIndex = 0;
            }
            layoutAccessPoints();
        }
    }
    else if (button == &nextPageBtn)
    {
        if ((apIndex + apItemsPerPage) < accessPointItems.size())
        {

            apIndex += apItemsPerPage;
            layoutAccessPoints();
        }
    }
    else
    {
        connectionButtonClicked(button);
    }
}

bool ConnectionPage::keyPressed(const KeyPress& key) { 
    
}

ConnectionPage::ConnectionListItem::ConnectionListItem
(ConnectionPoint connection) : connection(connection) { }

ConnectionPage::ConnectionListItem::~ConnectionListItem() { }

const ConnectionPage::ConnectionListItem::ConnectionPoint& getConnection()
{
    return connection
}

void ConnectionPage::ConnectionListItem::setLayout
(std::vector<GridLayoutManager::RowLayoutParams> layout)
{
    buttonLayout.clearLayout();
    buttonLayout.addComponents(layout, this)
}

void ConnectionPage::ConnectionListItem::paintButton(Graphics &g, bool isMouseOverButton,
        bool isButtonDown) override;

void ConnectionPage::ConnectionListItem::resized() { }
