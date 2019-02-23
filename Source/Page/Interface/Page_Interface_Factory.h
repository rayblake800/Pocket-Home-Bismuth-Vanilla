#pragma once
/**
 * @file  Page_Interface_Factory.h
 *
 * @brief  Allows individual page components to create new pages.
 */

namespace Page 
{ 
    namespace Interface 
    { 
        class Factory; 
        class FactoryHolder;
    } 
    class Component;
    class Factory;
    enum class Type;
}

/**
 * @brief  The Page::Component's interface to the page factory.
 */
class Page::Interface::Factory
{
public:
    Factory() { }

    virtual ~Factory() { }

protected:
    friend Page::Component;

    /**
     * @brief  Creates a new page to push on top of the page stack.
     * 
     * @param PageType  The type of page to create.
     */
    virtual Component* createPage(const Type pageType) = 0;
};

/**
 * @brief  Creates the connection between the page and its factory.
 */
class Page::Interface::FactoryHolder
{
public:
    FactoryHolder() { }

    virtual ~FactoryHolder() { }

protected:
    /**
     * @brief  Gets the factory holder's interface to the page factory.
     *
     * @return   The factory interface pointer, or nullptr if the factory holder
     *           was not created by a Page::Factory.
     */
    Factory* getFactoryInterface();

private:
    friend Page::Factory;

    /**
     * @brief  Sets the factory holder's factory interface pointer.
     *
     *  The Page::Factory must use this method to pass itself to each 
     * FactoryHolder it creates that needs to access the factory.
     *
     * @param pageFactory  The factory interface pointer to set.
     */
    void setFactoryInterface(Factory* pageFactory);

    Factory* factoryInterface = nullptr;
};
