#include "TempTimer.h"
#include "Utils.h"
#include "SwitchComponent.h"

SwitchComponent::SwitchComponent()
{
#    if JUCE_DEBUG
    setName("Switch Component");
#    endif
    addAndMakeVisible(handle);
}

void SwitchComponent::paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    float radius = float(pillBounds.getHeight()) / 2.0f;

    g.setColour(findColour(backgroundColourId));
    g.fillRoundedRectangle(pillBounds.getX(), pillBounds.getY(), pillBounds.getWidth(),
            pillBounds.getHeight(), radius);
}

void SwitchComponent::resized()
{
    DBG("switch resized");
    pillBounds = getLocalBounds();
    float ratio = (float) pillBounds.getWidth() / (float) pillBounds.getHeight();
    if (ratio > widthByHeight) // too wide, reduce width
    {
        pillBounds.reduce((pillBounds.getWidth()
                           - (pillBounds.getHeight() * widthByHeight)) / 2, 0);
    }
    else if (ratio < widthByHeight)// too tall, reduce height
    {
        pillBounds.reduce(0,
                (pillBounds.getHeight() - pillBounds.getWidth()
                 / widthByHeight) / 2);
    }
    DBG("local bounds " << getLocalBounds().toString() << ", pill bounds"
            << pillBounds.toString() << ", ratio = " << ratio);
    int handleMargin = std::max(pillBounds.getHeight() * 0.04, 2.0);
    int handleSize = std::max(pillBounds.getHeight() - handleMargin * 2, 2);
    handleBoundsOff.setBounds(
            pillBounds.getX() + handleMargin,
            pillBounds.getY() + handleMargin,
            handleSize,
            handleSize);
    handleBoundsOn = handleBoundsOff.withRightX(pillBounds.getRight() - handleMargin);
    Colour handleColour;
    Rectangle<int>& handleBounds = handleBoundsOff;
    if (getToggleState())
    {
        handleColour = findColour(handleColourId);
        handleBounds = handleBoundsOn;
    }
    else
    {
        handleColour = findColour(handleOffColourId);
    }
    handle.setFill(FillType(handleColour));
    handle.setBounds(handleBounds);
    updateHandlePath();
}

void SwitchComponent::clicked()
{
    DBG("switch clicked");
    const Rectangle<int>& bounds = getToggleState() ?
            handleBoundsOn : handleBoundsOff;
    Desktop::getInstance().getAnimator().animateComponent(&handle, bounds, 1.0f,
            150, false, 0.0, 0.0);
    handle.setFill(FillType(findColour(getToggleState() ?
            handleColourId : handleOffColourId)));
    /*
     * DrawablePath paths don't move relative to their component bounds.
     * They animate correctly, but on the next redraw they snap back to
     * their old location.  Until the Juce library fixes that, the path will
     * just have to be updated to match the bounds after the animation finishes.
     */
    TempTimer::initTimer(200, [this]()
    {
        updateHandlePath();
    });
}


void SwitchComponent::updateHandlePath()
{
    DBG("Handle resized");
    Path path;
    path.addEllipse(handle.getX(),
            handle.getY(),
            handle.getWidth(),
            handle.getHeight());
    handle.setPath(path);
}
