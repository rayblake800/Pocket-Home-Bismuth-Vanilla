#include "Page_WifiConnection.h"

/*
 * Initializes the page layout.
 */
Page::WifiConnection::WifiConnection()
{
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(1, { RowItem(&connectionList) })
    });
    setLayout(layout);
    setBackButton(BackButtonType::left);
}

/*
 * Makes the page's back button deselect the active list item instead of
 * closing the page when an item in the connection list is selected.
 */
bool Page::WifiConnection::overrideBackButton()
{
    if(connectionList.getSelectedIndex() != -1)
    {
        connectionList.deselect();
        return true;
    }
    return false;
}
