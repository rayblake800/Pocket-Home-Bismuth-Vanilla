#include <set>
#include "../PocketHomeApplication.h"
#include "Popup Editor Components/NewConfigAppEditor.h"
#include "Popup Editor Components/NewDesktopAppEditor.h"
#include "Popup Editor Components/NewFolderEditor.h"
#include "AppMenuButton/DesktopEntryButton.h"
#include "AppMenuButton/ConfigAppButton.h"
#include "AppMenuButton/AppFolderButton.h"
#include "AppMenuPage.h"
#include "AppMenuComponent.h"

AppMenuComponent::AppMenuComponent(AppConfigFile& appConfig) :
ConfigurableComponent(ComponentConfigFile::appMenuKey),
loadingAsync(false),
appConfig(appConfig),
showPopupCallback([this](AppMenuPopupEditor* newEditor)
{

    buttonEditor = newEditor;
    addAndMakeVisible(newEditor);
})
{
    AppMenuButton::setReloadButtonsCallback([this]()
    {
        loadButtons();
    });
    applyConfigBounds();
    x_origin = getBounds().getX();
    y_origin = getBounds().getY();
    setWantsKeyboardFocus(false);
    loadingSpinner = new OverlaySpinner();
    loadingSpinner->setAlwaysOnTop(true);

    loadButtons();
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
    int selection = selected.empty() ? 0 : selected[0]->getIndex();
    while (!buttonColumns.empty())
    {
        closeFolder();
    }
    buttonNameMap.clear();
    buttonColumns.clear();
    columnTops.clear();
    selected.clear();
    selected.push_back(nullptr);
    columnTops.push_back(y_origin);
    buttonColumns.emplace(buttonColumns.begin());

    //read in main page apps from config
    Array<AppConfigFile::AppItem> favorites = appConfig.getFavorites();
    for (const AppConfigFile::AppItem& favorite : favorites)
    {
        DBG(String("AppMenu:Found app in config:") + favorite.name);
        addButton(new ConfigAppButton(appConfig, favorite,
                buttonColumns[activeColumn()].size(),
                activeColumn(), iconThread));
    }

    //add category buttons
    Array<AppConfigFile::AppFolder> categories = appConfig.getFolders();
    for (const AppConfigFile::AppFolder& category : categories)
    {
        addButton(new AppFolderButton(appConfig, category,
                buttonColumns[activeColumn()].size(),
                activeColumn(), iconThread));
    }
    DBG(String("added ") + String(buttonColumns[activeColumn()].size())
            + " buttons");
    if (selection >= buttonColumns[0].size())
    {
        selection = buttonColumns[0].size() - 1;
    }
    selected[0] = buttonColumns[0][selection];
    buttonColumns[0][selection]->setSelected(true);
    scrollToSelected(false);
    if (!loadingAsync)
    {
        loadingAsync = true;
        showLoadingSpinner();
        desktopEntries.loadEntries([this](String loadingMsg)
        {
            loadingSpinner->setLoadingText(loadingMsg);
        },
        [this]()
        {
            loadingAsync = false;
            loadingSpinner->setLoadingText("");
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
        selectIndex(getSelectedButton()->getIndex() + indexOffset);
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
        if (selectedButton->isFolder())
        {
            appMenu.addItem(4, "New application link");
        } else if (selectedButton->getColumn() != 0)
        {
            appMenu.addItem(6, "Pin to favorites");
        }
        if (selectedButton->canChangeIndex(-1))
        {
            appMenu.addItem(7, "Move up");
        }
        if (selectedButton->canChangeIndex(1))
        {
            appMenu.addItem(8, "Move down");
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
            selectedButton->confirmRemoveButtonSource();
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
                newAppEditor->setCategories(selectedButton->getCategories());
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
            newFavorite.name = selectedButton->getAppName();
            newFavorite.icon = selectedButton->getIconName();
            newFavorite.shell = selectedButton->getCommand();
            newFavorite.launchInTerminal = false;
            appConfig.addFavoriteApp(newFavorite,
                    appConfig.getFavorites().size());
            confirmNew();
            break;
        }
        case 7://User selects "Move up"
            swapButtons(selectedButton,
                    buttonColumns[selectedButton->getColumn()]
                    [selectedButton->getIndex() - 1]);
            break;
        case 8://User selects "Move down"
            swapButtons(selectedButton,
                    buttonColumns[selectedButton->getColumn()]
                    [selectedButton->getIndex() + 1]);
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
    int folderIndex = selected[activeColumn()]->getIndex() -
            appConfig.getFavorites().size();
    AppConfigFile::AppFolder selectedFolder =
            appConfig.getFolders()[folderIndex];
    std::set<DesktopEntry> folderItems =
            desktopEntries.getCategoryListEntries(categoryNames);
    if (folderItems.empty())return;
    int columnTop = y_origin;
    if (selected[activeColumn()] != nullptr)
    {
        columnTop = selected[activeColumn()]->getY();
    }
    selected.push_back(nullptr);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton::Ptr>());
    DBG(String("found ") + String(folderItems.size()) + " items in folder");
    for (DesktopEntry desktopEntry : folderItems)
    {
        if (!desktopEntry.getValue(DesktopEntry::hidden)
                && !desktopEntry.getValue(DesktopEntry::hidden))
        {
            String name = desktopEntry.getValue(DesktopEntry::name);
            AppMenuButton* addedButton;
            if (buttonNameMap[name] != nullptr &&
                    buttonNameMap[name]->getParentComponent() == nullptr)
            {
                addedButton = buttonNameMap[name];
                addedButton->setIndex(buttonColumns[activeColumn()].size());
                addedButton->setColumn(activeColumn());
            } else
            {
                addedButton = new DesktopEntryButton(desktopEntry,
                        buttonColumns[activeColumn()].size(),
                        activeColumn(), iconThread);
            }
            addButton(addedButton);
        }
    }
    selected[activeColumn()] = buttonColumns[activeColumn()][0];
    buttonColumns[activeColumn()][0]->setSelected(true);
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
        toRemove->setSelected(false);
        Component * parent = toRemove->getParentComponent();
        if (parent != nullptr)
        {
            parent->removeChildComponent(toRemove);
        }
        buttonColumns[activeColumn()].pop_back();
    }
    selected.pop_back();
    buttonColumns.pop_back();
    columnTops.pop_back();
    scrollToSelected();
}

/**
 * @return true if currently loading information or a new child process.
 */
bool AppMenuComponent::isLoading()
{
    return loadingSpinner->isShowing();
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
            parentPage->removeChildComponent(loadingSpinner);
        }
    }
}

/**
 * Sets what should happen when a button is left clicked.
 * This opens selected buttons, and selects unselected button
 * @param button
 */
void AppMenuComponent::onButtonClick(AppMenuButton* button)
{
    if (button->getColumn() < activeColumn())
    {
        while (button->getColumn() < activeColumn())
        {
            closeFolder();
        }
        selectIndex(button->getIndex());
        return;
    }
    if (selected[activeColumn()] == button)
    {
        if (button->isFolder())
        {
            openFolder(button->getCategories());
        } else
        {
            showLoadingSpinner();
            appLauncher.startOrFocusApp(button->getAppName(),
                    button->getCommand());
        }
    } else
    {
        selectIndex(button->getIndex());
    }
}

/**
 * Returns a pop-up editor component for updating the selected button.
 */
AppMenuPopupEditor* AppMenuComponent::getEditorForSelected()
{
    if (!isLoading() && getSelectedButton() != nullptr)
    {
        return getSelectedButton()->getEditor();
    }
    return nullptr;
}

/**
 * Add a new application button to the active menu column.
 */
void AppMenuComponent::addButton(AppMenuButton* appButton)
{
    String name = appButton->getAppName();
    Rectangle<int>buttonSize = AppMenuButton::getButtonSize();
    int buttonWidth = buttonSize.getWidth();
    int buttonHeight = buttonSize.getHeight();
    if (buttonNameMap[name] == nullptr)
    {
        buttonNameMap[name] = appButton;
    }
    int index = appButton->getIndex();
    int column = appButton->getColumn();
    int x = column*buttonWidth;
    int y = columnTops[column] + buttonHeight * buttonColumns[column].size();
    appButton->setBounds(x, y, buttonWidth, buttonHeight);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addMouseListener(this, true);
    this->buttonColumns[column].push_back(appButton);
    if ((x + buttonWidth) > getWidth())
    {
        setBounds(getX(), getY(), x + buttonWidth, getHeight());
    }
    if ((y + buttonHeight) > getHeight())
    {
        setBounds(getX(), getY(), getWidth(), y + buttonHeight);
    }
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
                selectedButton->setSelected(false);
                selectedButton->repaint();
            }
            selectedButton = column[index];
            selectedButton->setSelected(true);
            selectedButton->repaint();

            selected[activeColumn()] = selectedButton;
            scrollToSelected();
        }
    }
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
 * Swaps the positions of two different buttons in the menu. This updates
 * their column and index values, swaps their bounds, and changes their
 * placements in the buttonColumns array.
 */
void AppMenuComponent::swapButtons(AppMenuButton* button1, AppMenuButton* button2)
{
    DBG(String("Swapping ")+button1->getAppName()+String(" and ")+button2->getAppName());
    int column = button1->getColumn();
    int index = button1->getIndex();
    if (column != button2->getColumn())
    {
        DBG("AppMenuComponent::swapButtons: swapping buttons between columns isn't allowed.");
        return;
    }
    if (button1->canChangeIndex(button2->getIndex()-index))
    {
        Rectangle<int> bounds = button1->getBounds();

        button1->setColumn(button2->getColumn());
        button1->moveDataIndex(button2->getIndex()-index);
        button1->setIndex(button2->getIndex());
//        if (button2 == selected.at(button2->getColumn()))
//        {
//            selected[button2->getColumn()] = button1;
//            button1->setSelected(true);
//        } else
//        {
//            button1->setSelected(false);
//        }
        buttonColumns[button2->getColumn()][button2->getIndex()] = button1;
        button1->setBounds(button2->getBounds());

        button2->setColumn(column);
        button2->setIndex(index);
//        if (button1 == selected.at(column))
//        {
//            selected[column] = button2;
//            button2->setSelected(true);
//        } else
//        {
//            button2->setSelected(false);
//        }
        buttonColumns[column][index] = button2;
        button2->setBounds(bounds);
    }
    else{
        DBG(String("Cant move index ")+String(index)+String(" by ")+String(button2->getIndex()-index));
    }
}

/**
 * Scroll the menu so that the selected button is centered.
 */
void AppMenuComponent::scrollToSelected(bool animatedScroll)
{
    AppMenuButton* selectedButton = getSelectedButton();
    if (selectedButton != nullptr)
    {
        int buttonWidth = selectedButton->getWidth();
        int buttonHeight = selectedButton->getHeight();
        int index = selectedButton->getIndex();
        int column = selectedButton->getColumn();
        Rectangle<int> dest = getBounds();
        //calculate y-position
        if (selectedButton->isVisible())
        {
            int buttonYPos = selectedButton->getY();
            int screenHeight = getWindowSize().getHeight();

            int distanceFromCenter = abs(buttonYPos - getY()
                    + screenHeight / 2);
            //only scroll vertically if selected button is outside 
            //the center 3/5 
            if (distanceFromCenter > screenHeight / 5 * 3)
            {
                dest.setY(y_origin - buttonYPos + screenHeight / 2
                        - buttonHeight / 2);
            }
            if (dest.getY() > y_origin)
            {
                dest.setY(y_origin);
            }
        } else
        {
            dest.setY(y_origin - columnTops[column]);
        }
        //calculate x-position
        if (column == 0)
        {
            dest.setX(x_origin);
        } else
        {
            dest.setX(x_origin - column * buttonWidth + buttonHeight);
        }
        //scroll to the calculated position
        if (animatedScroll)
        {
            ComponentAnimator& animator = Desktop::getInstance().getAnimator();
            if (animator.isAnimating(this))
            {
                animator.cancelAnimation(this, false);
            }
            animator.animateComponent(this, dest, 1, 100, false, 1, 1);
        } else
        {
            setBounds(dest);
        }
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
 * Resize all child components.
 */
void AppMenuComponent::resized()
{
    loadingSpinner->setBounds(getWindowSize());
    if (loadingAsync)
    {
        showLoadingSpinner();
    }
    ComponentConfigFile& config = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    ComponentConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(ComponentConfigFile::appMenuKey);
    Rectangle<int> menuBounds = menuSettings.getBounds();
    x_origin = menuBounds.getX();
    y_origin = menuBounds.getY();
    //resize all buttons
    Rectangle<int>buttonSize = AppMenuButton::getButtonSize();
    int buttonWidth = buttonSize.getWidth();
    int buttonHeight = buttonSize.getHeight();
    int numColumns = selected.size();
    if (menuBounds.getWidth() < numColumns * buttonWidth)
    {
        menuBounds.setWidth(numColumns * buttonWidth);
    }
    for (int c = 0; c < numColumns; c++)
    {
        if (c > 0)
        {
            columnTops[c] = selected[c - 1]->getY();
        }
        int numRows = buttonColumns[c].size();
        if (menuBounds.getHeight() < numRows * buttonHeight + columnTops[c])
        {
            menuBounds.setHeight(numRows * buttonHeight + columnTops[c]);
        }
        for (int i = 0; i < numRows; i++)
        {
            AppMenuButton * button = buttonColumns[c][i];
            button->setBounds(buttonSize.withPosition(c*buttonWidth,
                    i * buttonHeight + columnTops[c]));
        }
    }
    setBounds(menuBounds);
    if (activeColumn() >= 0 && selected[activeColumn()] != nullptr
            && !Desktop::getInstance().getAnimator().isAnimating())
    {

        scrollToSelected();
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


