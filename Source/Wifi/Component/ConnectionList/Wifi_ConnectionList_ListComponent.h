#pragma once
/**
 * @file Wifi_ConnectionList_ListComponent.h
 * 
 * @brief Shows information on all visible Wifi access points, and provides 
 *        controls for connecting to or disconnecting from those access points.
 */

#include "FocusingPagedList.h"
#include "Wifi_AP_StrengthListener.h"
#include "Wifi_APList_Listener.h"
#include "Wifi_Connection_Record_Listener.h"
#include "Wifi_ConnectionList_ControlComponent.h"

namespace Wifi { namespace ConnectionList { class ListComponent; } }

class Wifi::ConnectionList::ListComponent : public FocusingPagedList,
        public Wifi::APList::Listener,
        public Wifi::AP::StrengthListener,
        public Wifi::Connection::Record::Listener
{
public:
    /**
     * @brief  Loads the initial access point list on construction.
     */
    ListComponent();

    virtual ~ListComponent() { }
    
private:
    /**
     * @brief  Sets the number of items in the list to match the number of 
     *         visible Wifi access points.
     * 
     * @return  The number of WifiAccessPoint objects available. 
     */
    virtual unsigned int getListSize() const final override;

    /**
     * @brief  Creates or updates an unselected list item.
     * 
     * @param listItem  A list item Button to update. This parameter may be 
     *                  null, in which case a new ConnectionList::ListButton
     *                  should be created.
     * 
     * @param index     The index where the list item will be used. This index
     *                  is also the index of the AccessPoint object the list
     *                  item will represent.
     * 
     * @return          The updated list Component. 
     */
    virtual juce::Button* updateUnselectedListItem(juce::Button* listItem,
            const unsigned int index) final override;

    /**
     * @brief  Creates or updates a component so it can be used as the selected
     *         list item.
     * 
     * @param listItem  A list item Button to update. This parameter may be 
     *                  null, in which case a new ConnectionList::ListButton
     *                  should be created.
     * 
     * @return          The updated selected list Component. 
     */
    virtual juce::Button* updateSelectedListItem(juce::Button* listItem) 
        final override;

private:
    /**
     * @brief  Reloads the list of wifi access points within range of the wifi 
     *         device, and updates the access point list.
     */
    void loadAccessPoints();

    /**
     * @brief  Refreshes all visible list items without changing their order in
     *         the list.
     *
     * @param animate  Whether changes to the list layout should be animated.
     */
    void updateListItems(const bool animate = false);

    /**
     * @brief  Removes any lost access points and sorts the access point list.
     */
    void updateList();

    /**
     * @brief  Updates the list normally if no list item is selected. If a list
     *         item is selected, it will instead update list components, and
     *         wait to update list order and remove missing items until the
     *         full list is shown again.
     */
    void scheduleListUpdate();
    
    /**
     * @brief  If necessary, updates the list order and removes lost access
     *         points when the selection is removed to reveal the full list.
     */
    virtual void selectionChanged() final override;

    /**
     * @brief  Schedules a list update when access point strength changes.
     *
     * @param updatedAP  The AccessPoint with a new signal strength value.
     */
    virtual void signalStrengthUpdate(const AccessPoint updatedAP) 
        final override;

    /**
     * @brief  Adds a newly discovered access point to the list, and schedules a
     *         list update.
     *
     * @param newAP  The new Wifi::AccessPoint object.
     */
    virtual void accessPointAdded(const AccessPoint newAP) override;

    /**
     * @brief  Replaces the removed access point with a null access point and 
     *         schedules a list update whenever an access point is lost.
     *
     * @param removedAP  The access point that is no longer visible.
     */
    virtual void accessPointRemoved(const AccessPoint removedAP) override;

    /**
     * @brief  Updates access point connection controls when a connection starts
     *         to activate.
     *
     * @param connectingAP  The Wifi access point the system is trying to 
     *                      connect to.
     */
    virtual void startedConnecting(const AccessPoint connectingAP) override;

    /**
     * @brief  Updates access point connection controls when connection 
     *         authentication fails.
     *
     * @param connectingAP  The Wifi access point that failed to connect.
     */
    virtual void connectionAuthFailed(const AccessPoint connectingAP) override;

    /**
     * @brief  Schedules a list update whenever an access point connects.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const AccessPoint connectedAP) override; 

    /**
     * @brief  Schedules a list update whenever an access point disconnects.
     *
     * @param disconnectedAP  The access point that was being used by the closed
     *                        connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) override; 
    
    /* All visible access points. */
    juce::Array<AccessPoint> visibleAPs;

    /* Tracks whether the access point list needs to be sorted: */
    bool fullUpdateNeeded = false;

    /* Selected connection control component to reuse: */
    ControlComponent controlComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ListComponent)
};
