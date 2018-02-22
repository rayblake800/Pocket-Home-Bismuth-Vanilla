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


ConnectionPoint* ConnectionPage::getSelectedConnection()
{
	return selectedConnection;
}


void ConnectionPage::clearConnectionList()
{
	selectedConnection=nullptr;	
    removeAllChildren();
    connectionItems.clear();
    connections.clear();
    updateLayout({});
}

void ConnectionPage::updateConnectionList()
{
	clearConnectionList()
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
        if (showList || connectionItems[i]->getConnection()
            == *selectedConnection)
        {
			ConnectionListItem* listItem = (i < connectionItems.size()) ?
				connectionItems[i] : nullptr;
            if(listItem != nullptr)
            {
				listItem->setLayout((listItem == selectedConnection) ?
					getConnectionDetailLayout(listItem) : getConnectionLayout(listItem));
			}
            layout.push_back({rowWeight,
                {{listItem, 1}
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


void ConnectionPage::pageAddedToStack()
{
	updateConnectionList();
}
	
void ConnectionPage::pageRemovedFromStack()
{
	clearConnectionList();
}
	
void ConnectionPage::pageRevealedOnStack()
{
	updateConnectionList();
}
	
void ConnectionPage::pageCoveredOnStack()
{
	clearConnectionList();
}
	

void ConnectionPage::pageButtonClicked(Button*)
{
    if (button == &prevPageBtn)
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
		for(ConnectionListItem* listItem : connectionItems)
		{
			if(listItem->ownsButton(button))
			{
				if (*selectedConnection != listItem->getConnection)
				{
					setSelectedConnection(*listItem->getConnection);
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
}

bool ConnectionPage::keyPressed(const KeyPress& key) { 
		if (key == KeyPress::escapeKey)
		{
			setSelectedConnection(nullptr);
		}
}

ConnectionPage::ConnectionListItem::ConnectionListItem
(ConnectionPoint connection) : connection(connection) 
{ 
		listItemLayout.addComponents(
		{
			{3,
				{
					{&listButton,1}}
			}
		},this);
}

ConnectionPage::ConnectionListItem::~ConnectionListItem() { }

const ConnectionPage::ConnectionListItem::ConnectionPoint& getConnection()
{
    return connection;
}


void ConnectionPage::ConnectionListItem::layoutListItemComponents
(std::vector<GridLayoutManager::RowLayoutParams> layout)
{
	listButton.setLayout(layout);
}
        
void ConnectionPage::ConnectionListItem::setDetailedLayout
(std::vector<GridLayoutManager::RowLayoutParams> detailLayout)
{
	detailLayout.insert(0,
	{3,
		{
			{&listButton,1}}
	});
	listItemLayout.clearLayout(true);
	listItemLayout.addComponents(detailLayout,this);
}
			
void ConnectionPage::ConnectionListItem::setBasicLayout()
{
	listItemLayout.clearLayout(true);
}

void ConnectionPage::ConnectionListItem::setLayout
(std::vector<GridLayoutManager::RowLayoutParams> layout)
{
    listItemLayout.clearLayout();
    int buttonWeight = 2;
    if(!layout.empty()){
		std::vector<GridLayoutManager::RowLayoutParams> buttonLayout;
		buttonLayout.push_back(layout[0]);
		buttonWeight = layout[0].vertWeight;
		layout.remove(0);
		listButton.setLayout(buttonLayout);
	}
	layout.insert(0,{buttonWeight,{{&listButton,1}}});
    listItemLayout.addComponents(layout, this);
    resized();
}

bool ConnectionPage::ConnectionListItem::ownsButton(Button* button){
	return button == &listButton;
}

void ConnectionPage::ConnectionListItem::paint(Graphics &g)
{
	g.setColour(findColour(ListBox::ColourIds::backgroundColourId));
    isButtonDown ? setAlpha(0.5f) : setAlpha(1.0f);
    g.drawRoundedRectangle(getX(), getY(),
            getWidth(),getHeight(), 1, borderWidth);
}

void ConnectionPage::ConnectionListItem::resized() 
{
	Rectangle<int> bounds = getLocalBounds().reduced(borderWidth,
	borderWidth);
	listItemLayout.layoutComponents(bounds);
}
