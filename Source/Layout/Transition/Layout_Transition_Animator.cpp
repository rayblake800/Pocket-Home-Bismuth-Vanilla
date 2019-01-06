#include "Layout_Transition_Animator.h"
#include "Layout_Transition_Type.h"
#include "TempTimer.h"
#include "Utils.h"

/**
 * Duplicates the appearance of other Component objects for transition
 * animations.  These proxy components will be deleted after animation.
 * 
 * This class is almost an exact copy of the private 
 * ComponentAnimator::AnimationTask::ProxyComponent class. It is duplicated here
 * in order to allow simultaneous animation of component proxies and their 
 * source components. 
 */
class AnimationProxy : public juce::Component, private juce::Timer
{
public:

    /*
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
    (juce::Component& source, const unsigned int animationDuration)
    {
        AnimationProxy* proxy = new AnimationProxy(source, animationDuration);
        proxy->selfHolder = proxy;
        return proxy;
    }

    virtual ~AnimationProxy() { }

private:
    AnimationProxy
    (juce::Component& source, const unsigned int animationDuration)
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
                    | juce::ComponentPeer::windowIgnoresKeyPresses);
        }
        else
        {
            jassertfalse; //component is not visible!
        }

        float scale = (float) juce::Desktop::getInstance().getDisplays()
                .getDisplayContaining(getScreenBounds().getCentre()).scale;

        image = source.createComponentSnapshot(source.getLocalBounds(),
                false, scale);

        setVisible(true);
    }

    void paint(juce::Graphics& g) override
    {
        using namespace juce;
        g.setOpacity(1.0f);
        g.drawImageTransformed(image,
                AffineTransform::scale(getWidth() / (float) image.getWidth(),
                getHeight() / (float) image.getHeight()), false);
    }

    /*
     * Deletes the component if it is no longer animating.  If it is still
     * animating, the timer will be reset.
     */
    void timerCallback() override
    {
        const juce::MessageManagerLock mmLock;
        if (juce::Desktop::getInstance().getAnimator().isAnimating(this))
        {
            startTimer(timeBuffer);
        }
        else
        {
            stopTimer();
            juce::MessageManager::callAsync([this]()
            {
                selfHolder = nullptr;
            });
        }
    }

    juce::ScopedPointer<AnimationProxy> selfHolder;

    //Extra time to wait, in milliseconds, to ensure the timer is deleted
    //after animation finishes.
    static const constexpr unsigned int timeBuffer = 500;

    juce::Image image;
};

/*
 * Checks if a component is being animated through the default juce Animator.
 */
bool Layout::Transition::Animator::isAnimating(juce::Component* possiblyAnimating)
{
    return juce::Desktop::getInstance().getAnimator().isAnimating
        (possiblyAnimating);
}

/*
 * Moves one set of components off-window while moving another set of
 * components on-window, animating the transition.
 */
void Layout::Transition::Animator::animateTransition(
        juce::Array<juce::Component*> movingOut,
        juce::Array<std::pair<juce::Component*, juce::Rectangle<int>>> movingIn,
        const Type transition,
        const unsigned int animationMilliseconds)
{
    using juce::Component;
    using juce::Rectangle;
    for (std::pair<Component*, Rectangle<int>>& inComponent : movingIn)
    {
        if (inComponent.first == nullptr || isAnimating(inComponent.first))
        {
            continue;
        }

        int outIndex = movingOut.indexOf(inComponent.first);
        if (outIndex >= 0)
        {
            transitionOut(inComponent.first, transition,
                    animationMilliseconds, true);
            movingOut.remove(outIndex);
        }
        transitionIn(inComponent.first, transition, inComponent.second,
                animationMilliseconds);
    }
    for (Component* outComponent : movingOut)
    {
        if (outComponent == nullptr || isAnimating(outComponent))
        {
            continue;
        }
        transitionOut(outComponent, transition, animationMilliseconds);
    }
}

/*
 * Moves a component off-window, animating the transition.
 */
void Layout::Transition::Animator::transitionOut(
        juce::Component* component,
        const Type transition,
        const unsigned int animationMilliseconds,
        const bool useProxy)
{
    using juce::Component;
    using juce::Rectangle;
    if (component == nullptr)
    {
        return;
    }
    Rectangle<int> windowBounds = getWindowBounds();
    Rectangle<int> destination = component->getBounds();
    if (component->getScreenBounds().intersects(windowBounds))
    {
        switch (transition)
        {
            case Type::moveUp:
                destination.setY(destination.getY() - windowBounds.getHeight());
                break;
            case Type::moveDown:
                destination.setY(destination.getY() + windowBounds.getHeight());
                break;
            case Type::moveLeft:
                destination.setX(destination.getX() - windowBounds.getWidth());
                break;
            case Type::moveRight:
                destination.setX(destination.getX() + windowBounds.getWidth());
                break;
            case Type::toDestination:
            case Type::none:
                return;
        }
        if(useProxy)
        {
            Component * proxy = static_cast<Component*>
                    (AnimationProxy::getNewProxy(*component, 
                     animationMilliseconds));
            component->setBounds(destination);
            component = proxy;
        }
        transformBounds(component, destination, animationMilliseconds);
    }
}

/*
 * Moves a component into the window bounds, animating the transition.
 */
void Layout::Transition::Animator::transitionIn(
        juce::Component* component,
        const Type transition,
        const juce::Rectangle<int> destination,
        const unsigned int animationMilliseconds)
{
    using juce::Component;
    using juce::Rectangle;
    if (component == nullptr)
    {
        return;
    }
    if(transition == Type::none)
    {
        component->setBounds(destination);
        return;
    }
    if(transition == Type::toDestination)
    {
        transformBounds(component, destination, animationMilliseconds);
        return;
    }
    Rectangle<int> windowBounds = getWindowBounds();
    Rectangle<int> startBounds = destination;
    if(destination.intersects(windowBounds))
    {
        switch (transition)
        {
            case Type::moveUp:
                startBounds.setY(startBounds.getY() + windowBounds.getHeight());
                break;
            case Type::moveDown:
                startBounds.setY(startBounds.getY() - windowBounds.getHeight());
                break;
            case Type::moveLeft:
                startBounds.setX(startBounds.getX() + windowBounds.getWidth());
                break;
            case Type::moveRight:
                startBounds.setX(startBounds.getX() - windowBounds.getWidth());
                break;
            case Type::toDestination:
                startBounds = component->getBounds();
                break;
            default:
                DBG("Layout::Transition::Animator::" << __func__ 
                        << ": Unhandled transition type!");
                jassertfalse;
        }
        component->setBounds(startBounds);
        transformBounds(component, destination, animationMilliseconds);
    }
}

/*
 * Updates a component's bounds, animating the transformation.
 */
void Layout::Transition::Animator::transformBounds(
        juce::Component* const component,
        const juce::Rectangle<int>& destination,
        const unsigned int animationMilliseconds,
        const bool useProxy,
        const std::function<void()> onFinish)
{
    using juce::Component;
    if(component == nullptr)
    {
        return;
    }
    if (component != nullptr)
    {
        juce::Component* toAnimate = component;
        if(useProxy)
        {
            Component * proxy = static_cast<Component*>
                    (AnimationProxy::getNewProxy(*toAnimate, 
                     animationMilliseconds));
            toAnimate->setBounds(destination);
            toAnimate = proxy;
        }
        juce::Desktop::getInstance().getAnimator().animateComponent(toAnimate,
                destination, 1.0, animationMilliseconds, false, 0.2, 1.0);
    }
    if(onFinish)
    {
        TempTimer::initTimer(animationMilliseconds, 
                [onFinish]() { onFinish(); });
    }
}
