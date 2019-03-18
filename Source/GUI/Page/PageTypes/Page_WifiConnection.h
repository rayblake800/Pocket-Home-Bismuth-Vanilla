#pragma once
/**
 * @file  Page_WifiConnection.h
 *
 * @brief  The application page used to control the active Wifi connection.
 */

#include "Page_Component.h"
#include "Settings_WifiList_ListComponent.h"

namespace Page { class WifiConnection; }

/**
 * @brief  A page component that contains the Wifi connection list.
 */
class Page::WifiConnection final : public Component
{
public:
    /**
     * @brief  Initializes the page layout.
     */
    WifiConnection();

    virtual ~WifiConnection() { }

private:

    /**
     * @brief  Makes the page's back button deselect the active list item
     *         instead of closing the page when an item in the connection list
     *         is selected.
     *
     * @return   Whether the default back button action was replaced.
     */
    virtual bool overrideBackButton() override;

    /* The list component used to view and control possible connections: */
    Settings::WifiList::ListComponent connectionList;
};
