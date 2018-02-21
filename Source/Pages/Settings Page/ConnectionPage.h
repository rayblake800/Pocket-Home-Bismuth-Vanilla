/**
 * @file ConnectionPage.h
 * 
 * Connection page is the abstract base for PageComponent classes that
 * show a list of connections and allow the user to connect or disconnect
 * from connections in the list.
 */

#pragma once
#include "../../Basic Components/ScalingLabel.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../GridLayoutManager.h"
#include "../../PageComponent.h"
#include "../../PageStackComponent.h"

template<class ConnectionPoint>
class ConnectionPage : public PageComponent {
public:
    ConnectionPage();
    virtual ~ConnectionPage();
protected:
    /**
     *Returns the list of all connection objects that should be listed.
     */
    virtual Array<ConnectionPoint> loadConnectionList() = 0;

    /**
     * Attempts to open a connection, if possible.
     */
    virtual void connect(const ConnectionPoint& connection) = 0;

    /**
     * Attempts to close a connection, if possible.
     */
    virtual void disconnect(const ConnectionPoint& connection) = 0;

    /**
     * @return true iff the system is connected to ConnectionPoint connection.
     */
    virtual bool isConnected(const ConnectionPoint& connection) = 0;

    /**
     * This is called whenever a button other than the navigation buttons
     * is clicked.
     */
    virtual void connectionButtonClicked(Button* button) = 0;

    /**
     * Get the layout for a single connection list item.
     */
    virtual std::vector<GridLayoutManager::RowLayoutParams>
    getConnectionLayout(ConnectionPoint* connection) = 0;

    /**
     * Get the layout for a connection list item in expanded detail mode.
     * This should include whatever controls are needed to manage the
     * connection.
     */
    virtual std::vector<GridLayoutManager::RowLayoutParams>
    getConnectionDetailLayout(ConnectionPoint* connection) = 0;

    /**
     * Sets ConnectionPoint connection as the selected connection.  When a
     * connection is selected, this page will show details and controls for that
     * connection, and the other connections on the list will be hidden.  When
     * set to nullptr, the full ConnectionPoint list will be shown again.
     */
    virtual void setSelectedConnection(ConnectionPoint* connection);

private:
    /**
     * Reloads the list of connections and updates the page.
     */
    void updateConnectionList();

    /**
     * Reloads the page layout.  This method is what updates the page when
     * scrolling through the connection list or opening/closing connection
     * details.
     */
    void layoutConnectionPage();

    /**
     * Handles connection list scrolling and connection selection.
     */
    void pageButtonClicked(Button*) override;

    virtual bool keyPressed(const KeyPress& key) override;

    class ConnectionListItem : public Button {
    public:
        ConnectionListItem(ConnectionPoint connection);
        virtual ~ConnectionListItem();
        const ConnectionPoint& getConnection();
        void setLayout(std::vector<GridLayoutManager::RowLayoutParams> layout);
    private:
        void paintButton(Graphics &g, bool isMouseOverButton,
                bool isButtonDown) override;
        void resized() override;

        GridLayoutManager buttonLayout;
        ConnectionPoint connection;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionListItem)
    };

    DrawableImageButton nextPageBtn;
    DrawableImageButton prevPageBtn;
    static const constexpr int connectionsPerPage = 4;
    int connectionIndex = 0;
    OwnedArray<ConnectionListItem> connectionItems;
    Array<ConnectionPoint> connections;
    ScopedPointer<ConnectionPoint> selectedConnection = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionPage)
};
