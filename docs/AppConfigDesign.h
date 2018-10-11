class MenuItemData : public juce::ReferencedCountedObject;
{
public:
    typedef ReferenceCountedObjectPtr<MenuItemData> Ptr;
    
    MenuItemData(const int index, MenuItemData::Ptr parent);

    virtual ~MenuItemData() { }

    virtual juce::String getTitle() const = 0;
    virtual juce::String getIconName() const = 0;
    virtual juce::String getCommand() const = 0;
    virtual bool getLaunchedInTerm() const = 0;
    virtual juce::StringArray getCategories() const = 0;

    virtual void setTitle(const juce::String& newTitle) = 0;
    virtual void setIconName(const juce::String& newIcon) = 0;
    virtual void setCommand(const juce::String& newCommand) = 0;
    virtual void setLaunchedInTerm(const bool launchedInTerm) = 0;
    virtual void setCategories(const juce::StringArray& newCategories) = 0;
    
    MenuItemData::Ptr getParentFolder() const;
    int getIndex() const;

    bool isFolder() const;
    int getFolderSize() const;
    virtual int lastMovableChildIndex() = 0;

    MenuItemData::Ptr getChild(const int index) const;
    juce::Array<MenuItemData::Ptr> getChildren() const;

    void addChild(const MenuItemData::Ptr newChild);
    bool insertChild(const MenuItemData::Ptr newChild, const int index);
    bool replaceChild(const MenuItemData::Ptr newChild, const int index);
    bool removeChild(const int index);
    bool swapChildren(const int childIdx1, const int childIdx2);
    
    virtual void saveChanges() = 0;
    virtual void removeFromSource() = 0;

private:
    MenuItemData::Ptr parent;
    juce::Array<MenuItemData::Ptr> children;
    int index;
};

class AppMenuItem
{
public:    
    AppMenuItem(const MenuItemData::Ptr menuData);

    AppMenuItem();

    virtual ~AppMenuItem() { }

    bool isNull();

    juce::String getTitle();
    juce::String getIconName();
    juce::String getCommand();
    bool getLaunchedInTerm();
    juce::StringArray getCategories();

    AppMenuItem getParentFolder() const;
    int getIndex();

    bool isFolder() const;
    int getFolderSize() const;
    int lastMovableChildIndex() const; 
    AppMenuItem getFolderItem(const int folderIndex) const;
    juce::Array<AppMenuItem> getFolderItems() const;

    class Editor
    {
    public:
        Editor() { }
        virtual ~Editor() { }

        MenuItemData::Ptr getMenuItemData(AppMenuItem& menuItem) const;

        MenuItemData::Ptr clearMenuItemData(AppMenuItem& menuItem) const;
    };
}

class DesktopEntryMenuData : public MenuItemData
{
public:
    DesktopEntryMenuData(const DesktopEntry& desktopEntry,
            const int index, const MenuItemData::Ptr parentFolder);
    virtual DesktopEntryMenuData() { }

    virtual juce::String getTitle() const override;
    virtual juce::String getIconName() const override;
    virtual juce::String getCommand() const override;
    virtual bool getLaunchedInTerm() const override;
    virtual juce::StringArray getCategories() const override;

    virtual void setTitle(const juce::String& newTitle) override;
    virtual void setIconName(const juce::String& newIcon) override;
    virtual void setCommand(const juce::String& newCommand) override;
    virtual void setLaunchedInTerm(const bool launchedInTerm) override;
    virtual void setCategories(const juce::StringArray& newCategories) override;
    virtual int lastMovableChildIndex() override;
    virtual void saveChanges() override;
    virtual void removeFromSource() override;
private:
    DesktopEntry desktopEntry;
};

class AppJSON : public ConfigJSON, private AppMenuItem::Editor
{
public:    
    AppJSON();

    virtual ~AppJSON() { }

    AppMenuItem getRootFolderItem() const;

    void addMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder);

    bool insertMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder,
            const int index);

    bool replaceMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder,
            const int index);

    bool removeMenuItem(AppMenuItem& folder, const int index);
    
    bool swapMenuItems(AppMenuItem& folder,
            const int folderIdx1, const int folderIdx2);

private:
    void writeDataToJSON() override final;

    MenuItemData::Ptr rootFolderItem;

    class ConfigMenuItemData : public MenuItemData
    {
    public:
        ConfigMenuItemData(const juce::var& configData,
                const int index, const MenuItemData::Ptr parent);
        
        virtual ~ConfigMenuItemData() { }

        virtual juce::String getTitle() const override;
        virtual juce::String getIconName() const override;
        virtual juce::String getCommand() const override;
        virtual bool getLaunchedInTerm() const override;
        virtual juce::StringArray getCategories() const override;

        virtual void setTitle(const juce::String& newTitle) override;
        virtual void setIconName(const juce::String& newIcon) override;
        virtual void setCommand(const juce::String& newCommand) override;
        virtual void setLaunchedInTerm(const bool launchedInTerm) override;
        virtual void setCategories
            (const juce::StringArray& newCategories) override;

        virtual int lastMovableChildIndex() override;
        virtual void saveChanges() override;
        virtual void removeFromSource() override;

    private:
        juce::var menuData;
    };
};

class AppConfigFile : public ConfigFile<AppJSON>
{
public:
    AppConfigFile();

    virtual ~AppConfigFile() { }

    AppMenuItem getRootFolderItem();

    void addMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder);

    bool insertMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder,
            const int index);

    bool replaceMenuItem(const AppMenuItem& toCopy, AppMenuItem& folder,
            const int index);

    bool removeMenuItem(AppMenuItem& folder, const int index);
    
    bool swapMenuItems(AppMenuItem& folder,
            const int folderIdx1, const int folderIdx2);

};
