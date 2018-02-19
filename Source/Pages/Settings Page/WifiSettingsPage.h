/**
 * @file WifiSettingsPage.h
 * 
 * TODO: documentation and cleanup
 */

#pragma once
#include "../../Wifi/WifiStatus.h"
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../PageComponent.h"
#include "../../PageStackComponent.h"
#include "WifiAPPage.h"

class WifiSettingsPage : public PageComponent, public WifiStatus::Listener,
private ComponentListener {
public:
    WifiSettingsPage();
    ~WifiSettingsPage();

    void updateAccessPoints();
    void layoutAccessPoints();

private:
    void handleWifiDisabled() override;
    void handleWifiConnected() override;
    void handleWifiDisconnected() override;
    void handleWifiFailedConnect() override;

    void pageButtonClicked(Button*) override;
    void visibilityChanged() override;

    PageStackComponent& mainPageStack();

    DrawableImageButton nextPageBtn;
    DrawableImageButton prevPageBtn;
    SwitchComponent switchComponent;


    ScopedPointer<WifiAPPage> accessPointPage;

    class WifiAPListItem : public Button {
    public:
        WifiAPListItem(WifiAccessPoint ap);
        virtual ~WifiAPListItem();
        const WifiAccessPoint& getAccessPoint();
    private:
        void paintButton(Graphics &g, bool isMouseOverButton,
                bool isButtonDown) override;
        void resized() override;

        int wifiSignalStrengthToIdx(int strength);

        WifiAccessPoint accessPoint;
        Rectangle<float> wifiIconBounds;
        Rectangle<float> lockIconBounds;
        Rectangle<float> textBounds;
        Font textFont;

        static Array<Image> wifiImages;
        static Image lockImage;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiAPListItem)
    };
    static const constexpr int apItemsPerPage = 4;
    int apIndex = 0;
    OwnedArray<WifiAPListItem> accessPointItems;
    Array<WifiAccessPoint> accessPoints;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsPage)
};
