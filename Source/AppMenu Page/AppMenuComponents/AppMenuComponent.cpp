#include <set>
#include "../../PocketHomeApplication.h"
#include "../Popup Editor Components/NewConfigAppEditor.h"
#include "../Popup Editor Components/NewDesktopAppEditor.h"
#include "../Popup Editor Components/NewFolderEditor.h"
#include "../AppMenuButton/AppMenuItemFactory.h"
#include "../AppMenuPage.h"
#include "AppMenuComponent.h"

AppMenuComponent::AppMenuComponent
(String componentKey, AppConfigFile& appConfig) :
ConfigurableComponent(componentKey),
loadingAsync(false),
appConfig(appConfig),
showPopupCallback([this](AppMenuPopupEditor* newEditor)
{

    buttonEditor = newEditor;
    addAndMakeVisible(newEditor);
})
{
    applyConfigBounds();
    setWantsKeyboardFocus(false);
    loadingSpinner.setAlwaysOnTop(true);
}

AppMenuComponent::~AppMenuComponent()
{
    stopWaitingForLoading();
    while (!buttonColumns.empty())closeFolder();
}

/**
 * Loads all app menu buttons
 */
void AppMenuComponent::loadButtons()
{
    DBG("loading AppMenuButtons");
    int selection = selected.empty() ? 0 : selected[0]->getRowIndex();
    while (!buttonColumns.empty())
    {
        closeFolder();
    }
    buttonNameMap.clear();
    buttonColumns.clear();
    selected.clear();
    selected.push_back(nullptr);
    buttonColumns.emplace(buttonColumns.begin());

    //read in main page apps from config
    Array<AppConfigFile::AppItem> favorites = appConfig.getFavorites();
    for (const AppConfigFile::AppItem& favorite : favorites)
    {
        DBG(String("AppMenu:Found app in config:") + favorite.name);
        addButton(createMenuButton(AppMenuItemFactory::create
                (favorite, appConfig),
                activeColumn(), buttonColumns[activeColumn()].size()));
    }

    //add category buttons
    Array<AppConfigFile::AppFolder> categories = appConfig.getFolders();
    for (const AppConfigFile::AppFolder& category : categories)
    {
        addButton(createMenuButton(AppMenuItemFactory::create
                (category, appConfig),
                activeColumn(), buttonColumns[activeColumn()].size()));
    }
    DBG(String("added ") + String(buttonColumns[activeColumn()].size())
            + " buttons");
    if (selection >= buttonColumns[0].size())
    {
        selection = buttonColumns[0].size() - 1;
    }
    selected[0] = buttonColumns[0][selection];
    buttonColumns[0][selection]->setToggleState(true, NotificationType::dontSendNotification);
    scrollToSelected(false);
    if (!loadingAsync)
    {
        loadingAsync = true;
        showLoadingSpinner();
        desktopEntries.loadEntries([this](String loadingMsg)
        {
            loadingSpinner.setLoadingText(loadingMsg);
        },
        [this]()
        {
            loadingAsync = false;
            loadingSpinner.setLoadingText("");
            hideLoadingSpinner();
        });
    }
}

/**
 * If this function is called, when this component would create a pop-up
 * editor component, instead of adding it to itself, it will pass the
 * editor to this callback function.
 */
void AppMenuComponent::setPopupCallback
(std::function<void(AppMenuPopupEditor*) > callback)
{
    showPopupCallback = callback;
}

/**
 * Select a new AppMenuButton relative to the current selected button.
 */
void AppMenuComponent::changeSelection(int indexOffset)
{
    if (getSelectedButton() == nullptr)
    {
        selectIndex(0);
    } else
    {
        selectIndex(getSelectedButton()->getRowIndex() + indexOffset);
    }

}

/**
 * Trigger a click for the selected button.
 */
void AppMenuComponent::clickSelected()
{
    if (selected[activeColumn()] != nullptr && !isLoading())
    {
        onButtonClick(selected[activeColumn()]);
    }
}

/**
 * Open the pop-up menu used for editing this object.
 * @param selectionMenu if true, the pop-up menu contains only options for 
 * editing the selected menu button.
 */
void AppMenuComponent::openPopupMenu(bool selectionMenu)
{
    PopupMenu appMenu;
    AppMenuButton* selectedButton = selectionMenu ?
            getSelectedButton() : nullptr;

    AppMenuItem * selectedMenuItem = selectedButton == nullptr ?
            nullptr : selectedButton->getMenuItem();

    /* Menu Options:
     * 1. Edit button
     * 2. Delete button (and button source)
     * 3. Create new pinned favorites item
     * 4. Create new desktop entry
     * 5. Create new folder
     * 6. Add application to favorites
     * 7. Move button up
     * 8. Move button down
     */


    if (selectedButton != nullptr)
    {
        appMenu.addItem(1, "Edit");
        appMenu.addItem(2, "Delete");
        if (selectedMenuItem->isFolder())
        {
            appMenu.addItem(4, "New application link");
        } else if (selectedButton->getColumnIndex() != 0)
        {
            appMenu.addItem(6, "Pin to favorites");
        }
        if (selectedMenuItem->canChangeIndex(-1))
        {
            appMenu.addItem(7, "Move forward");
        }
        if (selectedMenuItem->canChangeIndex(1))
        {
            appMenu.addItem(8, "Move back");
        }
    } else
    {
        appMenu.addItem(3, "New favorite application link");
        appMenu.addItem(4, "New application link");
        appMenu.addItem(5, "New folder");
    }

    const int selection = appMenu.show();
    std::function<void() > confirmNew = [this]()
    {
        loadButtons();
    };
    switch (selection)
    {
        case 1://User selects "Edit"
            showPopupCallback(getEditorForSelected());
            break;
        case 2://User selects "Delete"
            selectedButton->confirmRemoveButtonSource([this]()
            {
                loadButtons();
            });
            break;
        case 3://User selects "New favorite application"
            showPopupCallback(new NewConfigAppEditor(appConfig, iconThread,
                    confirmNew));
            break;
        case 4://User selects "New application link"
        {
            AppMenuPopupEditor* newAppEditor = new NewDesktopAppEditor
                    (iconThread, confirmNew);
            if (selectedButton != nullptr)
            {
                newAppEditor->setCategories(selectedButton->getMenuItem()->getCategories());
            }
            showPopupCallback(newAppEditor);
            break;
        }
        case 5://User selects "New folder"
        {
            AppMenuPopupEditor* newFolderEditor = new NewFolderEditor
                    (appConfig, iconThread, confirmNew);
            showPopupCallback(newFolderEditor);
            break;
        }
        case 6://User selects "Pin to favorites"
        {
            AppConfigFile::AppItem newFavorite;
            newFavorite.name = selectedMenuItem->getAppName();
            newFavorite.icon = selectedMenuItem->getIconName();
            newFavorite.shell = selectedMenuItem->getCommand();
            newFavorite.launchInTerminal = false;
            appConfig.addFavoriteApp(newFavorite,
                    appConfig.getFavorites().size());
            confirmNew();
            break;
        }
        case 7://User selects "Move forward"
            swapButtons(selectedButton,
                    buttonColumns[selectedButton->getColumnIndex()]
                    [selectedButton->getRowIndex() - 1]);
            break;
        case 8://User selects "Move back"
            swapButtons(selectedButton,
                    buttonColumns[selectedButton->getColumnIndex()]
                    [selectedButton->getRowIndex() + 1]);
    }
}

/**
 * @return the index of the active button column.
 */
int AppMenuComponent::activeColumn()
{
    return selected.size() - 1;
}

/**
 * Open an application category folder, creating AppMenuButtons for all
 * associated desktop applications.
 */
void AppMenuComponent::openFolder(Array<String> categoryNames)
{
    int folderIndex = selected[activeColumn()]->getRowIndex() -
            appConfig.getFavorites().size();
    AppConfigFile::AppFolder selectedFolder =
            appConfig.getFolders()[folderIndex];
    std::set<DesktopEntry> folderItems =
            desktopEntries.getCategoryListEntries(categoryNames);
    if (folderItems.empty())
    {
        return;
    }
    selected.push_back(nullptr);
    buttonColumns.push_back(std::vector<AppMenuButton::Ptr>());
    DBG(String("found ") + String(folderItems.size()) + " items in folder");

    for (DesktopEntry desktopEntry : folderItems)
    {
        if (!desktopEntry.getValue(DesktopEntry::hidden)
                && !desktopEntry.getValue(DesktopEntry::hidden))
        {
            String name = desktopEntry.getValue(DesktopEntry::name);
            AppMenuButton::Ptr addedButton;
            if (buttonNameMap[name].get() != nullptr &&
                    buttonNameMap[name]->getParentComponent() == nullptr)
            {
                addedButton = buttonNameMap[name];
                addedButton->setRowIndex(buttonColumns[activeColumn()].size());
                addedButton->setColumnIndex(activeColumn());
            } else
            {
                addedButton = createMenuButton(AppMenuItemFactory::create
                        (desktopEntry), activeColumn(),
                        buttonColumns[activeColumn()].size());
            }
            addButton(addedButton);
        }
    }
    selected[activeColumn()] = buttonColumns[activeColumn()][0];
    buttonColumns[activeColumn()][0]->setToggleState(true, NotificationType::dontSendNotification);
    scrollToSelected();
}

/**
 * close the topmost open folder, removing all contained buttons
 */
void AppMenuComponent::closeFolder()
{
    if (isLoading())
    {
        return;
    }
    for (int i = buttonColumns[activeColumn()].size() - 1; i >= 0; i--)
    {
        AppMenuButton* toRemove = buttonColumns[activeColumn()][i];
        toRemove->setVisible(false);
        toRemove->setToggleState(false, NotificationType::dontSendNotification);
        Component * parent = toRemove->getParentComponent();
        if (parent != nullptr)
        {
            parent->removeChildComponent(toRemove);
        }
        buttonColumns[activeColumn()].pop_back();
    }
    selected.pop_back();
    buttonColumns.pop_back();
    scrollToSelected();
}

/**
 * @return true if currently loading information or a new child process.
 */
bool AppMenuComponent::isLoading()
{
    return loadingSpinner.isShowing();
}

/**
 * Makes the menu stop waiting to load something, re-enabling
 * user input.
 */
void AppMenuComponent::stopWaitingForLoading()
{

    hideLoadingSpinner();
    appLauncher.stopTimer();
}

/**
 * Gets the selected button in the active button column.
 */
AppMenuButton* AppMenuComponent::getSelectedButton()
{
    try
    {
        return selected.at(activeColumn());
    } catch (std::out_of_range e)
    {
        return nullptr;
    }
}

/**
 * Resize all child components.
 */
void AppMenuComponent::resized()
{
    loadingSpinner.setBounds(getWindowSize());
    if (loadingAsync)
    {
        showLoadingSpinner();
    }
}

/**
 * Display the spinner that indicates application loading. This will
 * also disable input.
 */
void AppMenuComponent::showLoadingSpinner()
{
    Component * parentPage = getParentComponent();
    if (parentPage != nullptr)
    {

        parentPage->addAndMakeVisible(loadingSpinner);
    }
}

/**
 * Hide the loading spinner, re-enabling user input.
 */
void AppMenuComponent::hideLoadingSpinner()
{
    if (!loadingAsync)
    {
        Component * parentPage = getParentComponent();
        if (parentPage != nullptr)
        {
            parentPage->removeChildComponent(&loadingSpinner);
        }
    }
}

/**
 * Sets what should happen when a button is left clicked.
 * This opens selected buttons, and selects unselected buttons
 * @param button
 */
void AppMenuComponent::onButtonClick(AppMenuButton* button)
{
    if (button->getColumnIndex() < activeColumn())
    {
        while (button->getColumnIndex() < activeColumn())
        {
            closeFolder();
        }
        selectIndex(button->getRowIndex());
        return;
    }
    if (selected[activeColumn()] == button)
    {
        AppMenuItem * selectedMenuItem = button->getMenuItem();
        if (selectedMenuItem->isFolder())
        {
            openFolder(selectedMenuItem->getCategories());
        } else
        {
            showLoadingSpinner();
            appLauncher.startOrFocusApp(selectedMenuItem->getAppName(),
                    selectedMenuItem->getCommand());
        }
    } else
    {
        selectIndex(button->getRowIndex());
    }
}

/**
 * Returns a pop-up editor component for updating the selected button.
 */
AppMenuPopupEditor* AppMenuComponent::getEditorForSelected()
{
    if (!isLoading() && getSelectedButton() != nullptr)
    {
        AppMenuItem * selectedItem = getSelectedButton()->getMenuItem();
        return getSelectedButton()->getEditor(
                [this, selectedItem](AppMenuPopupEditor * editor)
                {
                    if (editor->getCategories() != selectedItem->getCategories())
                    {
                        loadButtons();
                    }
                });
    }
    return nullptr;
}

/**
 * Add a new application button to the active menu column.
 */
void AppMenuComponent::addButton(AppMenuButton::Ptr appButton)
{
    AppMenuItem * newMenuItem = appButton->getMenuItem();
    String name = newMenuItem->getAppName();
    if (buttonNameMap[name] == nullptr)
    {
        buttonNameMap[name] = appButton;
    }
    int columnIndex = activeColumn();
    int rowIndex = buttonColumns[columnIndex].size();
    appButton->setColumnIndex(columnIndex);
    appButton->setRowIndex(rowIndex);
    this->buttonColumns[columnIndex].push_back(appButton);
    appButton->addMouseListener(this, true);
    addButtonComponent(appButton);
}

/**
 * Change which button is selected in the active menu column.
 */
void AppMenuComponent::selectIndex(int index)
{
    if (!isLoading())
    {
        std::vector<AppMenuButton::Ptr>& column =
                buttonColumns.at(activeColumn());
        AppMenuButton* selectedButton = getSelectedButton();
        if (index >= 0 && index < column.size()
                && column[index].get() != selectedButton)
        {
            if (selectedButton != nullptr)
            {
                selectedButton->setToggleState(false,
                        NotificationType::dontSendNotification);
                selectedButton->repaint();
            }
            selectedButton = column[index];
            selectedButton->setToggleState(true,
                    NotificationType::dontSendNotification);
            selectedButton->repaint();

            selected[activeColumn()] = selectedButton;
            scrollToSelected();
        }
    }
}

/**
 * Swaps the positions of two different buttons in the menu. This updates
 * their column and index values, swaps their bounds, and changes their
 * placements in the buttonColumns array.
 */
void AppMenuComponent::swapButtons(AppMenuButton* button1, AppMenuButton* button2)
{
    DBG(String("Swapping ") + button1->getMenuItem()->getAppName()
            + String(" and ") + button2->getMenuItem()->getAppName());
    int column = button1->getColumnIndex();
    int index = button1->getRowIndex();
    if (column != button2->getColumnIndex())
    {
        DBG("AppMenuComponent::swapButtons: swapping buttons between columns isn't allowed.");
        return;
    }
    if (button1->getMenuItem()->canChangeIndex(button2->getRowIndex() - index))
    {
        Rectangle<int> bounds = button1->getBounds();

        button1->setColumnIndex(button2->getColumnIndex());
        button1->moveDataIndex(button2->getRowIndex() - index);
        button1->setRowIndex(button2->getRowIndex());
        buttonColumns[button2->getColumnIndex()][button2->getRowIndex()] = button1;
        button1->setBounds(button2->getBounds());

        button2->setColumnIndex(column);
        button2->setRowIndex(index);
        buttonColumns[column][index] = button2;
        button2->setBounds(bounds);
    } else
    {
        DBG(String("Cant move index ") + String(index) + String(" by ") + String(button2->getRowIndex() - index));
    }
}

/**
 * handle all AppMenuButton clicks
 */
void AppMenuComponent::mouseDown(const MouseEvent &event)
{
    //ignore clicks when loading or when the editor is open
    if (!isLoading() &&
            (buttonEditor == nullptr || !buttonEditor->isVisible()))
    {

        AppMenuButton* appClicked = dynamic_cast<AppMenuButton*>
                (event.originalComponent);

        //handle right clicks/ctrl-clicks
        if (event.mods.isPopupMenu() || event.mods.isCtrlDown())
        {
            if (appClicked == nullptr ||
                    appClicked == getSelectedButton())
            {
                openPopupMenu(appClicked == getSelectedButton());
            } else
            {
                onButtonClick(appClicked);
            }

        }//handle regular clicks
        else if (event.mods.isLeftButtonDown() && appClicked != nullptr)
        {
            onButtonClick(appClicked);
        }
    }
}

/**
 * Exit the loading state when visibility is lost, enter the loading state
 * if necessary when visibility is gained.
 */
void AppMenuComponent::visibilityChanged()
{
    if (loadingAsync)
    {
        showLoadingSpinner();
    } else if (!isVisible())
    {
        stopWaitingForLoading();
    }
}


