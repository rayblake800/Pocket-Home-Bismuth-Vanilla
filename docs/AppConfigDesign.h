class MenuItemData : public juce::ReferencedCountedObject;
{
public:
    typedef ReferenceCountedObjectPtr<MenuItemData> Ptr;
    
    MenuItemData(const int depth, const int index, MenuItemData::Ptr parent);

    virtual juce::String getTitle() const = 0;
    virtual juce::String getIconName() const = 0;
    virtual juce::String getCommand() const = 0;
    virtual bool getLaunchedInTerm() const = 0;
    virtual juce::StringArray getCategories() const = 0;

    virtual void setTitle(const juce::String& newTitle) = 0;
    virtual void setIconName() = 0;
    virtual void setCommand() = 0;
    virtual void setLaunchedInTerm() = 0;
    virtual void setCategories() = 0;

    virtual bool isFolder() const = 0;


    MenuItemData::Ptr getParent() const;
    int getDepth() const;
    int getIndex() const;

private:
    MenuItemData::Ptr parent;
    juce::Array<MenuItemData::Ptr> children;
};
