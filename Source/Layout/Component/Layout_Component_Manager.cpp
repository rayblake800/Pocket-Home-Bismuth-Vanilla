#include "Layout_Component_Manager.h"
#include "Layout_Component_ConfigFile.h"
#include "Utils.h"

namespace ComponentLayout = Layout::Component;

ComponentLayout::Manager::Manager(juce::Component* component,
        const juce::Identifier& layoutKey) :
component(component), layoutKey(layoutKey)
{
    ConfigFile layoutReader;
    layout = layoutReader.getLayout(layoutKey);
}

/*
 * Uses configuration data to set the component's bounds.
 */
void ComponentLayout::Manager::applyConfigBounds()
{
    if(component != nullptr)
    {
        juce::Rectangle<int> newBounds = layout.getBounds();
        if (newBounds.getX() < 0)
        {
            newBounds.setX(component->getX());
        }
        if (newBounds.getY() < 0)
        {
            newBounds.setY(component->getY());
        }
        if (newBounds.getWidth() < 0)
        {
            newBounds.setWidth(component->getWidth());
        }
        if (newBounds.getHeight() < 0)
        {
            newBounds.setHeight(component->getHeight());
        }
        component->setBounds(newBounds);
    }
}

/*
 * Gets the key that selects the component's layout.
 */
const juce::Identifier& ComponentLayout::Manager::getLayoutKey() const
{
    return layoutKey;
}

/*
 * Gets the layout object this Manager will apply.
 */
ComponentLayout::ConfigLayout ComponentLayout::Manager::getLayout() const
{
    return layout;
}

