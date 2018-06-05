#include "Utils.h"
#include "TransitionAnimator.h"

/**
 * Duplicates the appearance of other Component objects for transition
 * animations.  These proxy components will be deleted after animation.
 * 
 * This class is almost an exact copy of the private 
 * ComponentAnimator::AnimationTask::ProxyComponent class. It is duplicated here
 * in order to allow simultaneous animation of component proxies and their 
 * source components. 
 */
class AnimationProxy : public Component, private Timer
{
public:

    /**
     * @param source             The proxy component will add itself to this
     *                           component's parent, and copy its bounds and
     *                           appearance.
     * 
     * @param animationDuration  The duration in milliseconds the proxy will
     *                           be needed for animation.  Once this time
     *                           has elapsed and the component is finished
     *                           animating, it will be deleted.
     */
    static AnimationProxy * getNewProxy
    (Component& source, const unsigned int animationDuration)
    {
        AnimationProxy* proxy = new AnimationProxy(source, animationDuration);
        proxy->selfHolder = proxy;
        return proxy;
    }

    virtual ~AnimationProxy() { }
private:

    AnimationProxy(Component& source, const unsigned int animationDuration)
    {
        startTimer(animationDuration + timeBuffer);
        setWantsKeyboardFocus(false);
        setBounds(source.getBounds());
        setTransform(source.getTransform());
        setAlpha(source.getAlpha());
        setInterceptsMouseClicks(false, false);

        if (auto* parent = source.getParentComponent())
        {
            parent->addAndMakeVisible(this);
        }
        else if (source.isOnDesktop() && source.getPeer() != nullptr)
        {
            addToDesktop(source.getPeer()->getStyleFlags()
                    | ComponentPeer::windowIgnoresKeyPresses);
        }
        else
        {
            jassertfalse; //component is not visible!
        }

        float scale = (float) Desktop::getInstance().getDisplays()
                .getDisplayContaining(getScreenBounds().getCentre()).scale;

        image = source.createComponentSnapshot(source.getLocalBounds(),
                false, scale);

        setVisible(true);
    }

    void paint(Graphics& g) override
    {
        g.setOpacity(1.0f);
        g.drawImageTransformed(image,
                AffineTransform::scale(getWidth() / (float) image.getWidth(),
                getHeight() / (float) image.getHeight()), false);
    }

    /**
     * Deletes the component if it is no longer animating.  If it is still
     * animating, the timer will be reset.
     */
    void timerCallback() override
    {
        if (Desktop::getInstance().getAnimator().isAnimating(this))
        {
            startTimer(timeBuffer);
        }
        else
        {
            stopTimer();
            MessageManager::callAsync([this]()
            {
                selfHolder = nullptr;
            });
        }
    }

    ScopedPointer<AnimationProxy> selfHolder;
    //Extra time to wait, in milliseconds, to ensure the timer is deleted
    //after animation finishes.
    static const constexpr unsigned int timeBuffer = 500;
    Image image;
};

/*
 * Moves one set of components off-screen, while moving another set of
 * components on-screen, animating the transition.  Any component that is
 * in both sets will appear to move off-screen while a duplicate component
 * moves on-screen simultaneously.
 */
void TransitionAnimator::animateTransition(
        Array<Component*> movingOut,
        Array<std::pair<Component*, Rectangle<int>>> movingIn,
        const TransitionAnimator::Direction direction,
        const unsigned int animationMilliseconds)
{
    Array<Component*> useProxy;
    for (std::pair<Component*, Rectangle<int>>&inComponent : movingIn)
    {
        if (inComponent.first == nullptr || Desktop::getInstance().getAnimator()
            .isAnimating(inComponent.first))
        {
            continue;
        }

        int outIndex = movingOut.indexOf(inComponent.first);
        if (outIndex >= 0)
        {
            AnimationProxy* proxy = AnimationProxy::getNewProxy(
                    *inComponent.first,
                    animationMilliseconds);
            transitionOut(static_cast<Component*> (proxy), direction,
                    animationMilliseconds);
            movingOut.remove(outIndex);
        }
        transitionIn(inComponent.first, direction, inComponent.second,
                animationMilliseconds);
    }
    for (Component* outComponent : movingOut)
    {
        if (outComponent == nullptr || Desktop::getInstance().getAnimator()
            .isAnimating(outComponent))
        {
            continue;
        }
        transitionOut(outComponent, direction, animationMilliseconds);
    }

}

/*
 * Moves a component off-screen, animating the transition.
 */
void TransitionAnimator::transitionOut(Component* component,
        const TransitionAnimator::Direction direction,
        const unsigned int animationMilliseconds)
{
    if (component == nullptr)
    {
        return;
    }
    Rectangle<int> windowBounds = getWindowBounds();
    Rectangle<int> destination = component->getBounds();
    if (component->getScreenBounds().intersects(windowBounds))
    {
        switch (direction)
        {
            case Direction::moveUp:
                destination.setY(destination.getY() - windowBounds.getHeight());
                break;
            case Direction::moveDown:
                destination.setY(destination.getY() + windowBounds.getHeight());
                break;
            case Direction::moveLeft:
                destination.setX(destination.getX() - windowBounds.getWidth());
                break;
            case Direction::moveRight:
                destination.setX(destination.getX() + windowBounds.getWidth());
        }
        transformBounds(component, destination, animationMilliseconds);
    }
}

/*
 * Moves a component into the screen bounds, animating the transition.
 */
void TransitionAnimator::transitionIn(Component* component,
        const TransitionAnimator::Direction direction,
        const Rectangle<int> destination,
        const unsigned int animationMilliseconds)
{
    if (component == nullptr)
    {
        return;
    }
    Rectangle<int> windowBounds = getWindowBounds();
    Rectangle<int> startBounds = destination;
    if (destination.intersects(windowBounds))
    {
        switch (direction)
        {
            case Direction::moveUp:
                startBounds.setY(startBounds.getY() + windowBounds.getHeight());
                break;
            case Direction::moveDown:
                startBounds.setY(startBounds.getY() - windowBounds.getHeight());
                break;
            case Direction::moveLeft:
                startBounds.setX(startBounds.getX() + windowBounds.getWidth());
                break;
            case Direction::moveRight:
                startBounds.setX(startBounds.getX() - windowBounds.getWidth());
        }
        component->setBounds(startBounds);
        transformBounds(component, destination, animationMilliseconds);
    }
}

/*
 * Updates a component's bounds, animating the transformation.
 */
void TransitionAnimator::transformBounds(Component* component,
        const Rectangle<int>& destination,
        const unsigned int animationMilliseconds)
{
    if (component != nullptr)
    {
        Desktop::getInstance().getAnimator().animateComponent(component,
                destination, 1.0, animationMilliseconds, false, 0.2, 1.0);
    }
}
