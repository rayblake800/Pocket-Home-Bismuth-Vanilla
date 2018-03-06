#include "Configuration/ComponentConfigFile.h"
#include "PageComponent.h"

PageComponent::PageComponent(const String& name,
        GridLayoutManager::Layout layout,
        bool showBackButton, bool backButtonOnRight) :
backButtonOnRight(backButtonOnRight),
backgroundImage(Image::null)
{
    layoutManager.setLayout(layout);
    if (showBackButton)
    {
        backButton = new ConfigurableImageButton(backButtonOnRight ?
                ComponentConfigFile::pageRightKey
                : ComponentConfigFile::pageLeftKey);
        backButton->addListener(this);
        addAndMakeVisible(backButton);
    }
}

PageComponent::~PageComponent() { }

/**
 * Sets a background image to draw behind all page components.
 */
void PageComponent::setBackgroundImage(Image bgImage)
{
    backgroundImage = bgImage;
}

/**
 * Adds all components in the layout to the page and makes them visible.
 */
void PageComponent::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}

/**
 * Replaces the page layout.  All components in the old layout will be
 * removed from the page before setting the new layout.  Components in
 * the new layout will be added to the page and made visible.
 */
void PageComponent::updateLayout(GridLayoutManager::Layout layout)
{
    layoutManager.clearLayout(true);
    layoutManager.setLayout(layout, this);
    if (isVisible())
    {

        resized();
    }
}

/**
 * Sets the amount of space to leave between page components and the edges
 * of the page.
 */
void PageComponent::setMarginFraction(float marginFraction)
{

    horizontalMargin = marginFraction;
    verticalMargin = marginFraction;
}

/**
 * Sets the amount of space to leave between components in the page layout.
 */
void PageComponent::setPadding
(float verticalFraction, float horizontalFraction)
{

    verticalPadding = verticalFraction;
    horizontalPadding = horizontalFraction;
}

/**
 * Inheriting classes can override this method to change the behavior of the
 * back button. It will be called every time the back button is clicked, and
 * if it returns true, the back button will not remove the page.
 * 
 * @return true if the back button's action was replaced, false to allow
 * the back button to remove the page as usual.
 */
bool PageComponent::overrideBackButton()
{
    return false;
}

/**
 * Repositions all page components using the layout manager along with
 * the margin and padding values.
 */
void PageComponent::layoutComponents()
{
    Rectangle<int> bounds = getLocalBounds();
    if (!bounds.isEmpty())
    {
        int xMargin = (int) (bounds.getHeight() * horizontalMargin);
        bounds.reduce(xMargin, (int) bounds.getHeight() * verticalMargin);
        if(backButton != nullptr)
        {
            
            int overlap = std::min<int>
                    (backButton->getRight() - bounds.getX(),
                     bounds.getRight() - backButton->getX());
            if(overlap > 0)
            {
                bounds.reduce(overlap,0);
            }
        }
        layoutManager.layoutComponents(bounds,
                (int) bounds.getWidth() * horizontalPadding,
                (int) bounds.getHeight() * verticalPadding);
    }

}

/**
 * Recalculate component layout and back button bounds when the page is
 * resized.
 */
void PageComponent::resized()
{
    if (backButton != nullptr)
    {
        backButton->applyConfigBounds();
    }
    layoutComponents();
    pageResized();
}

/**
 * Closes the page when the back button is clicked, and passes all other
 * button clicks to the pageButtonClicked method.
 */
void PageComponent::buttonClicked(Button* button)
{
    if (button == backButton && !overrideBackButton())
    {
        removeFromStack(backButtonOnRight ?
                PageStackComponent::kTransitionTranslateHorizontalLeft :
                PageStackComponent::kTransitionTranslateHorizontal);
    }
    else
    {
        pageButtonClicked(button);
    }
}

/**
 * Fills the page background with an image or color.
 */
void PageComponent::paint(Graphics& g)
{
    if (backgroundImage != Image::null)
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        g.fillAll(findColour(backgroundColourId));
    }
}
