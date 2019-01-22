#ifndef APPMENU_IMPLEMENTATION
  #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_ConfigData.h
 *
 * @brief  An ItemData subclass that handles menu data read from the JSON 
 *         configuration file.
 */

#include "Locale/TextUser.h"
#include "AppMenu_ItemData.h"

/** 
 *  ConfigData menu items are defined within the appMenu.json configuration 
 * file. These menu items may represent either a menu folder, or an application
 * shortcut. Changes to ConfigData objects are written directly back to the
 * source JSON file.
 *
 *  To prevent circular dependencies, the methods for writing changes back to
 * the JSON file are left abstract, and should be implemented in a ConfigData
 * subclass by the Config::FileResource subclass responsible for managing the
 * JSON menu data.
 *
 * @see  AppMenu_ConfigJSON.h
 */
namespace AppMenu { class ConfigData; }

class AppMenu::ConfigData : public ItemData, public Locale::TextUser
{
public:
    /**
     * @brief  Creates a new menu item that initially holds no data.
     */
    ConfigData();

    virtual ~ConfigData() { }
    
    /**
     * @brief  Recursively initializes menu item data, creating and initializing
     *         all its child folder items.
     *
     * @param menuData  A JSON data object defining a menu item.
     */
    void initMenuData(juce::var& menuData);

    /**
     * @brief  Gets the menu item's displayed title.
     *
     * @return  The displayed title string.
     */
    virtual juce::String getTitle() const override;

    /**
     * @brief  Gets the name or path used to load the menu item's icon file.
     *
     * @return  The name or path of the icon.
     */
    virtual juce::String getIconName() const override;

    /**
     * @brief  Gets the menu item's application launch command.
     *
     * @return  The launch command string, or the empty string if the menu item
     *          does not launch an application.
     */
    virtual juce::String getCommand() const override;

    /**
     * @brief  Checks if this menu item launches an application in a new
     *         terminal window.
     *
     * @return  Whether the menu item has a launch command it should run in a 
     *          new terminal window.
     */
    virtual bool getLaunchedInTerm() const override;

    /**
     * @brief  Gets the application categories used to load this item's
     *         desktop entry child folder items.
     *
     * @return  Any category strings assigned to this menu item.
     */
    virtual juce::StringArray getCategories() const override;

    /**
     * @brief  Sets the menu item's displayed title.
     *
     * @param title  The new title string to display.
     */
    virtual void setTitle(const juce::String& title) override;

    /**
     * @brief  Sets the name or path used to load the menu item's icon file.
     *
     * @param iconName  The new icon name or path.
     */
    virtual void setIconName(const juce::String& iconName) override;

    /**
     * @brief  Sets the menu item's application launch command.
     *
     * @param newCommand  The new command string to run when this menu item is 
     *                    clicked.
     */
    virtual void setCommand(const juce::String& newCommand) override;

    /**
     * @brief  Sets if this menu item runs its command in a new terminal window.
     *
     * @param termLaunch  True to run any launch command assigned to this menu 
     *                    item within a new terminal window, false to run 
     *                    commands normally.
     */
    virtual void setLaunchedInTerm(const bool termLaunch) override;

    /**
     * @brief  Sets the application categories used to load this item's desktop 
     *         entry child folder items.
     *
     * @param categories  The new set of category strings to assign to this 
     *                    menu item.
     */
    virtual void setCategories(const juce::StringArray& categories) override;

    /**
     * @brief  Gets the number of folder items held by this menu item that 
     *         can be reordered.
     *
     * @return  The number of child folder items held by this menu item that are
     *          directly defined by JSON data.
     */
    virtual int getMovableChildCount() const override;
    
    /**
     * @brief  Checks if this menu item could be moved within its folder, 
     *         assuming that another movable menu item exists that could be
     *         swapped with this one.
     *
     * @return  Always true, as JSON data defined menu items can be freely 
     *          swapped.
     */
    virtual bool isMovable() const override;
    
    /**
     * @brief  Gets an appropriate title to use for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation title string.
     */
    virtual juce::String getConfirmDeleteTitle() const override;

    /**
     * @brief  Gets appropriate descriptive text for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation description string.
     */
    virtual juce::String getConfirmDeleteMessage() const override;

    /**
     * @brief  Gets an appropriate title to use for a menu item editor.
     *
     * @return  A localized editor title string.
     */
    virtual juce::String getEditorTitle() const override;

    /**
     * @brief  Checks if a data field within this menu item can be edited.
     *
     * @param dataField  The type of data being checked.
     *
     * @return           Whether the data field is editable.
     */
    virtual bool isEditable(const DataField dataField) const override;

private:
    /**
     * @brief  Creates an empty child menu item.
     *
     * This method is used so that ConfigData can define how to recursively
     * create its child menu items despite being an abstract class. This should 
     * only be called by initMenuData when recursively initializing the menu 
     * item.
     *
     * @return  A pointer to an empty child menu item.
     */
    virtual ConfigData::Ptr createChildItem() = 0;

    /* Tracks whether initMenuData has been called before, to ensure it only
       runs once. */
    bool initialized = false;

    /* The displayed title */
    juce::String title;

    /* The icon name or path */
    juce::String iconName;

    /* An application launch command, or the empty string. */
    juce::String command;

    /* Whether the menu item launches an application in a terminal window. */
    bool launchInTerm = false;

    /* Application categories used to select desktop entry folder items. */
    juce::StringArray categories;
};
