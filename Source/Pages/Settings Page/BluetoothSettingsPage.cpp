#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "BluetoothSettingsPage.h"


BluetoothSettingsPage::BluetoothSettingsPage() :
PageComponent("BluetoothSettingsPage",{}, true),
checkIcon("confirm.svg"),
btIcon("bluetoothIcon.svg"),
deviceListPage(1, 4),
connectionLabel("Connected", "Connection Label"),
connectionButton("Connection Button", "Connect")
{
    addAndMakeVisible(pageStack);

    // create device list "page"
    BluetoothStatus& status = PocketHomeApplication::getInstance()
            ->getBluetoothStatus();
    for (auto btDevice : status.devices)
    {
        auto item = new BluetoothDeviceListItem(btDevice);
        item->addListener(this);
        deviceListItems.add(item);
        deviceListPage.addItem(item);
    }
    connectionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(connectionLabel);

    connectionButton.addListener(this);
    addAndMakeVisible(connectionButton);
    //pageStack.pushPage(&deviceListPage, PageStackComponent::kTransitionNone);
}

BluetoothSettingsPage::~BluetoothSettingsPage() { }

void BluetoothSettingsPage::pageResized()
{
    auto bounds = getLocalBounds();
    auto pageBounds = Rectangle<int>(120, 0, bounds.getWidth() - 120, bounds.getHeight());

    pageStack.setBounds(pageBounds);
    connectionLabel.setBounds(10, 70, pageBounds.getWidth() - 20, 60);
    connectionButton.setBounds(90, 160, pageBounds.getWidth() - 180, 50);
    btIcon.setBounds(-10, 0, 80, 80);
}

void BluetoothSettingsPage::pageButtonClicked(Button *button)
{
    if (button == &connectionButton && selectedDevice)
    {
        selectedDevice->connected = !selectedDevice->connected;
        pageStack.popPage(PageStackComponent::kTransitionTranslateHorizontal);
    }


    auto btButton = dynamic_cast<BluetoothDeviceListItem *> (button);
    if (btButton)
    {
        selectedDevice = btButton->device;
        connectionButton.setButtonText(selectedDevice->connected
                ? "Disconnect" : "Connect");
        connectionLabel.setText(selectedDevice->name + "\n"
                + selectedDevice->macAddress,
                juce::NotificationType::dontSendNotification);
    }
}


BluetoothSettingsPage::BluetoothDeviceListItem::BluetoothDeviceListItem
(BluetoothStatus::BluetoothDevice *device)
: Button(device->name), device(device){ }

void BluetoothSettingsPage::BluetoothDeviceListItem::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = getLocalBounds();
    auto inset = bounds.reduced(6, 4);
    auto w = bounds.getWidth(), h = bounds.getHeight();
    auto iconBounds = Rectangle<float>(w - h, h / 5.0, h * 0.6, h * 0.6);

    auto listOutline = Path();
    listOutline.addRoundedRectangle(inset.toFloat(), 10.0f);
    g.setColour(findColour(ListBox::ColourIds::backgroundColourId));
    g.fillPath(listOutline);

//    if (device->connected)
//    {
//        icons->checkIcon->setSize(h, h);
//        icons->checkIcon->drawWithin(g, iconBounds, RectanglePlacement::fillDestination, 1.0f);
//    }

    //  icons->arrowIcon->setSize(h, h);
    //  icons->arrowIcon->drawWithin(g, Rectangle<float>(w - (h/8), contentHeight + 8, contentHeight, contentHeight),
    //                               RectanglePlacement::fillDestination, 1.0f);

    g.setFont(h * 0.5);
    g.setColour(findColour(ListBox::ColourIds::textColourId));
    g.drawText(getName(), inset.reduced(h * 0.2, 0), Justification::centredLeft, true);
}
