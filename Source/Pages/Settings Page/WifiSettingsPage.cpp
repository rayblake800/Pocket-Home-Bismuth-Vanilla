#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "WifiSettingsPage.h"

static const Array<String> WifiSettingsPage::wifiImageFiles = 
{
	"wifiStrength0.svg", 
	"wifiStrength1.svg",
    "wifiStrength2.svg",
    "wifiStrength3.svg"
};

WifiSettingsPage::WifiSettingsPage()
{
    WifiStatus& wifiStatus = PocketHomeApplication::getInstance()
            ->getWifiStatus();
    wifiStatus.addListener(this);
    if (wifiStatus.isConnected())
    {
        handleWifiConnected();
    }
}

WifiSettingsPage::~WifiSettingsPage() { }

/**
*Returns the list of all access points that should be listed.
*/
Array<WifiAccessPoint> WifiSettingsPage::loadConnectionList()
{
	return PocketHomeApplication::getInstance()
            ->getWifiStatus().nearbyAccessPoints();
}
    
    
/**
* Attempts to open a connection, if possible.
*/
void WifiSettingsPage::connect(const WifiAccessPoint& connection)
{
	WifiStatus& wifiStatus
            = PocketHomeApplication::getInstance()->getWifiStatus();
    if (wifiStatus.isConnected())
	{
		PocketHomeApplication::getInstance()->getWifiStatus().setDisconnected();
	}
	ListItemComponents* apComponents = getAPComponents(connection);
	if(apComponents != nullptr)
	{
		apComponents->errorLabel.setVisible(false);
        if (connection.requiresAuth)
        {
            const auto& psk = apComponents->passwordEditor.getTextValue().toString();
            status.setConnectedAccessPoint(connection, psk);
        }
        else
        {
            status.setConnectedAccessPoint(connection);
        }
    }
}

/**
* Attempts to close a connection, if possible.
*/
void WifiSettingsPage::disconnect(const WifiAccessPoint& connection)
{
	if(isConnected(connection))
	{
		PocketHomeApplication::getInstance()->getWifiStatus().setDisconnected();
	}
}

/**
* @return true iff the system is connected to WifiAccessPoint connection.
*/
bool WifiSettingsPage::isConnected(const WifiAccessPoint& connection){
	WifiStatus& wifiStatus
            = PocketHomeApplication::getInstance()->getWifiStatus();
	try
	{
		WifiAccessPoint connectedAP = status.connectedAccessPoint();
		return connectedAP.hash == connection.hash;
	}
	catch (WifiStatus::MissingAccessPointException e)
    {
		return false;
    }
}

/**
* This is called whenever a button other than the navigation buttons
* is clicked.
*/
void WifiSettingsPage::connectionButtonClicked(Button* button)
{
	ListItemComponents* selectedAPComps = getSelectedAPComponents();
	if(selectedAPComps != nullptr && 
	&selectedAPComps->connectionButton == button)
	{
		WifiAccessPoint* selectedAP = getSelectedConnection();
		if(isConnected(*selectedAP))
		{
			disconnect(*selectedAP);
		}
		else
		{
			connect(*selectedAP);
		}
	}
}

/**
* Get the layout for a single connection list item.
*/
virtual std::vector<GridLayoutManager::RowLayoutParams>
WifiSettingsPage::getConnectionLayout(WifiAccessPoint* connection)
{
	ListItemComponents* apComponents = getAPComponents(connection);
	if(apComponents == nullptr){
		apComponents = new ListItemComponents(connection);
		listItemComponents.add(apComponents);
		apComponents->connectionButton.addListener(this);
	}
	else{
		apComponents->wifiIcon.setImage(getWifiAssetName());
	}
	return {
		{3,{
			{&apComponents->connectionLabel,7},
			{connection.requiresAuth ?
				&apComponents->lockIcon : nullptr,1},
			{&apComponents->wifiIcon,1}}}
	};
}

/**
* Get the layout for a connection list item in expanded detail mode.
* This should include whatever controls are needed to manage the
* connection.
*/
std::vector<GridLayoutManager::RowLayoutParams>
WifiSettingsPage::getConnectionDetailLayout(WifiAccessPoint* connection)
{
	std::vector<GridLayoutManager::RowLayoutParams> layout = 
		getConnectionLayout(connection);
	ListItemComponents* apComponents = getAPComponents(connection);
	layout.push_back({2,{{&apComponents->passwordEditor,1}}});
	layout.push_back({2,{{&apComponents->connectionButton,1}}});
	layout.push_back({2,{{&apComponents->errorLabel,1}}});
	return layout;
	
}
    
void WifiSettingsPage::setCurrentlyConnecting(bool currentlyConnecting)
{
	ListItemComponents* apComponents = getSelectedAPComponents();
	if(apComponents != nullptr)
	{
		WifiAccessPoint* wifiAP = getSelectedConnection();
		bool wifiEnabled = PocketHomeApplication::getInstance()
            ->getWifiStatus().isEnabled();
		apComponents->spinner.setVisible(currentlyConnecting);
		apComponents->connectionButton.setEnabled(wifiEnabled && !currentlyConnecting);
		apComponents->passwordEditor.setEnabled(wifiAP->requiresAuth && wifiEnabled
            && !currentlyConnecting);
		apComponents->passwordEditor.setVisible(wifiAP->requiresAuth);
	}
}

void handleWifiEnabled() override;
    
    void handleWifiDisabled() override;
    
    void handleWifiConnected() override;
    
    void handleWifiDisconnected() override;
    
    void handleWifiBusy() override;
    
    void handleWifiFailedConnect() override;

void textEditorReturnKeyPressed(TextEditor &)
{
}

static String WifiSettingsPage::getWifiAssetName(const WifiAccessPoint& accessPoint)
{
    float sigStrength = std::max(0., std::fmin(100, accessPoint.signalStrength));
    int maxIndex = wifiImageFiles.size() - 1;
    return wifiImageFiles[round((maxIndex * (sigStrength) / 100.0f))];
}
     

WifiSettingsPage::ListItemComponents::ListItemComponents(const WifiAccessPoint& accessPoint):
accesPoint(accessPoint),
connectionLabel("connectionLabel",accessPoint.ssid),
wifiIcon(getWifiAssetName(accessPoint));
lockIcon("lock.svg"),
connectionButton("connectionButton", isConnected(accessPoint)?"Disconnect","Connect")
{
}
		
   
WifiSettingsPage::ListItemComponents* WifiSettingsPage::getAPComponents(const WifiAccessPoint& accessPoint)
{
	for(ListItemComponents* listComp : listItemComponents)
	{
		if(listComp->accessPoint.hash == accessPoint.hash)
		{
			return listComp;
		}
	}
	return nullptr;
}

WifiSettingsPage::ListItemComponents* WifiSettingsPage::getSelectedAPComponents()
{
	return getAPComponents(*getSelectedConnection());
}
    

void WifiSettingsPage::handleWifiDisabled()
{
    DBG("WifiSettingsPage::wifiDisabled");
    //close wifi settings pages if wifi is disabled
    if (mainPageStack().getCurrentPage() == this)
    {
        mainPageStack().popPage(PageStackComponent::kTransitionTranslateHorizontal);
    }
    else
    {
        mainPageStack().removePage(this);
    }
}

void WifiSettingsPage::handleWifiConnected()
{
    DBG("WifiSettingsPage::wifiConnected");
    try
    {
        WifiAccessPoint ap = PocketHomeApplication::getInstance()
                ->getWifiStatus().connectedAccessPoint();

        if (mainPageStack().getCurrentPage() == this)
        {
            if (accessPointPage == nullptr)
            {
                accessPointPage = new WifiAPPage(ap);
            }
            else
            {
                accessPointPage->setAccessPoint(ap);
            }
            mainPageStack().swapPage(accessPointPage,
                    PageStackComponent::kTransitionNone);
        }
        else
        {
            mainPageStack().removePage(this);
        }
    }
    catch (WifiStatus::MissingAccessPointException e)
    {
        DBG("WifiSettingsPage::wifiConnected: no access point found!");
    }

}

void WifiSettingsPage::handleWifiDisconnected()
{
    DBG("WifiSettingsPage::wifiDisconnected");
    updateAccessPoints();
    if (mainPageStack().getCurrentPage() == accessPointPage)
    {

        mainPageStack().insertPage(this, mainPageStack().getDepth() - 1);
    }
}

void WifiSettingsPage::handleWifiFailedConnect()
{
    DBG("WifiSettingsPage::wifiFailedConnect");
    if (mainPageStack().getCurrentPage() == accessPointPage)
    {

        mainPageStack().insertPage(this, mainPageStack().getDepth() - 1);
    }
}

void WifiSettingsPage::updateAccessPoints()
{
    accessPointItems.clear();
    updateLayout({});
    accessPoints = PocketHomeApplication::getInstance()
            ->getWifiStatus().nearbyAccessPoints();
    for (WifiAccessPoint ap : accessPoints)
    {

        DBG(__func__ << ": added " << ap.ssid << ", "
                << ap.signalStrength << ", " << ap.requiresAuth);
        WifiAPListItem* item = new WifiAPListItem(ap);
        item->addListener(this);
        accessPointItems.add(item);
    }
    layoutAccessPoints();
}

void WifiSettingsPage::layoutAccessPoints()
{
    std::vector<GridLayoutManager::RowLayoutParams> layout;
    layout.push_back({1,
        {
            {(apIndex > 0) ? &prevPageBtn : nullptr, 1}
        }});
    for (int i = apIndex; i < apIndex + apItemsPerPage; i++)
    {

        layout.push_back({2,
            {
                {(i < accessPointItems.size()) ?
                    accessPointItems[i] : nullptr, 1}
            }});
    }
    layout.push_back({1,
        {
            {(accessPointItems.size() > apIndex + apItemsPerPage) ?
                &nextPageBtn : nullptr, 1}
        }});

    updateLayout(layout);
}

void WifiSettingsPage::pageButtonClicked(Button *button)
{
    WifiAPListItem* apButton = dynamic_cast<WifiAPListItem *> (button);
    if (apButton != nullptr)
    {
        if (accessPointPage == nullptr)
        {
            accessPointPage = new WifiAPPage(apButton->getAccessPoint());
        }
        else
        {
            accessPointPage->setAccessPoint(apButton->getAccessPoint());
        }
        mainPageStack().pushPage(accessPointPage,
                PageStackComponent::kTransitionTranslateHorizontal);
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
}
//TODO: replace with custom page visibility callback that triggers only when
//the page is added to the stack
void WifiSettingsPage::visibilityChanged()
{
    if (mainPageStack().getCurrentPage() == this)
    {
        WifiStatus& status = PocketHomeApplication::getInstance()
                ->getWifiStatus();
        if (status.isConnected())
        {

            handleWifiConnected();
        }
    }
}

PageStackComponent& WifiSettingsPage::mainPageStack()
{

    return PocketHomeApplication::getInstance()->getMainStack();
}


Array<Image> WifiSettingsPage::WifiAPListItem::wifiImages;
Image WifiSettingsPage::WifiAPListItem::lockImage = Image::null;

WifiSettingsPage::WifiAPListItem::WifiAPListItem(WifiAccessPoint ap) :
Button(ap.ssid), accessPoint(ap)
{
    if (lockImage == Image::null)
    {
        lockImage = createImageFromFile(assetFile("lock.png"));
    }
    if (wifiImages.isEmpty())
    {
        Array<String> wifiAssets = {"wifiStrength0.svg", "wifiStrength1.svg",
                                    "wifiStrength2.svg", "wifiStrength3.svg"};
        for (const String& assetStr : wifiAssets)
        {

            wifiImages.add(createImageFromFile(assetFile(assetStr)));
        }
    }
}

WifiSettingsPage::WifiAPListItem::~WifiAPListItem() { }

const WifiAccessPoint& WifiSettingsPage::WifiAPListItem::getAccessPoint()
{

    return accessPoint;
}

void WifiSettingsPage::WifiAPListItem::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = getLocalBounds();
    auto inset = bounds.reduced(6, 4);
    auto borderThick = 4.0;

    g.setColour(findColour(ListBox::ColourIds::backgroundColourId));
    isButtonDown ? setAlpha(0.5f) : setAlpha(1.0f);
    g.drawRoundedRectangle(bounds.getX() + borderThick,
            bounds.getY() + borderThick,
            bounds.getWidth() - 4 * borderThick,
            bounds.getHeight() - 2 * borderThick,
            1, borderThick);

    const Image& wifiImage = wifiImages
            [wifiSignalStrengthToIdx(accessPoint.signalStrength)];
    g.drawImage(wifiImage, wifiIconBounds,
            RectanglePlacement::fillDestination, 1.0f);
    if (accessPoint.requiresAuth)
    {

        g.drawImage(lockImage, lockIconBounds,
                RectanglePlacement::fillDestination, 1.0f);
    }

    g.setFont(textFont);
    g.setColour(findColour(ListBox::ColourIds::textColourId));
    g.drawText(getName(), textBounds, Justification::centredLeft);
}

void WifiSettingsPage::WifiAPListItem::resized()
{

    Rectangle<float> bounds = getLocalBounds().toFloat();
    wifiIconBounds.setX(bounds.getWidth() - bounds.getHeight());
    wifiIconBounds.setY(bounds.getHeight() / 5.0);
    wifiIconBounds.setWidth(bounds.getHeight() / 2);
    wifiIconBounds.setHeight(bounds.getHeight() / 2);
    lockIconBounds = wifiIconBounds.translated(-bounds.getHeight() * 0.75, 0);
    textBounds = bounds.reduced(6 + bounds.getHeight()*0.3, 4);

    int textHeight = PocketHomeApplication::getInstance()->getComponentConfig()
            .getFontHeight(textBounds.toNearestInt(), getName());
    textFont.setHeight(textHeight);
}

int WifiSettingsPage::WifiAPListItem::wifiSignalStrengthToIdx(int strength)
{
    // 0 to 100
    float sigStrength = std::max(0., std::fmin(100, strength));
    int iconBins = wifiImages.size() - 1;
    return round((iconBins * (sigStrength) / 100.0f));
}
