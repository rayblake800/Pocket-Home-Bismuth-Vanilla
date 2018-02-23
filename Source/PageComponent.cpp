#include "PocketHomeApplication.h"
#include "PageComponent.h"

PageComponent::PageComponent(const String& name,
        GridLayoutManager::Layout layout,
        bool showBackButton, bool backButtonOnRight) :
backButtonOnRight(backButtonOnRight)
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

void PageComponent::setBackgroundImage(Image bgImage)
{

    backgroundImage = new Image(bgImage);
}

void PageComponent::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}

void PageComponent::updateLayout(GridLayoutManager::Layout layout)
{
    layoutManager.clearLayout(true);
    layoutManager.setLayout(layout, this);
    if (isVisible())
    {

        resized();
    }
}

void PageComponent::setMarginFraction(float marginFraction)
{

    this->marginFraction = marginFraction;
}

void PageComponent::setPadding
(float verticalFraction, float horizontalFraction)
{

    verticalPadding = verticalFraction;
    horizontalPadding = horizontalFraction;
}

void PageComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    int horizontalMargin = (int) (bounds.getHeight() * marginFraction);
    if (backButton != nullptr)
    {

        backButton->applyConfigBounds();
        horizontalMargin = backButton->getWidth();
    }
    bounds.reduce(horizontalMargin, (int) bounds.getHeight() * marginFraction);
    layoutManager.layoutComponents(bounds,
            (int) bounds.getWidth() * horizontalPadding,
            (int) bounds.getHeight() * verticalPadding);
    pageResized();
}

void PageComponent::buttonClicked(Button* button)
{
    if (button == backButton)
    {
        PocketHomeApplication::getInstance()->getMainStack()
                .popPage(backButtonOnRight ?
                    PageStackComponent::kTransitionTranslateHorizontalLeft :
                    PageStackComponent::kTransitionTranslateHorizontal);
    }
    else
    {
        pageButtonClicked(button);
    }
}

void PageComponent::paint(Graphics& g)
{
    if (backgroundImage != nullptr && *backgroundImage != Image::null)
    {
        g.drawImage(*backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        g.fillAll(findColour(backgroundColourId));
    }
}
