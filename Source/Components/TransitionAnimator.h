#pragma once
#include "JuceHeader.h"

/**
 * @file TransitionAnimator.h
 * 
 * @brief Animates the transition when replacing visible Components on the 
 *        screen, making removed components slide out while new components
 *        slide in from the opposite direction. 
 */

class TransitionAnimator
{
public:

    enum Direction
    {
        moveUp,
        moveDown,
        moveLeft,
        moveRight
    };

    /**
     * Replaces the on-screen components, animating the transition.
     * 
     * @param transitionIn           New Component or Component Array to move 
     *                               on-screen. Component bounds should already 
     *                               be set to the desired destination bounds.  
     *                               If not visible, Component(s) will become 
     *                               visible after being moved off-screen for 
     *                               animation.
     * 
     * @param transitionOut          Visible, on-screen Component or Component 
     *                               Array to move off-screen.  If a 
     *                               transitionOut component is also in 
     *                               transitionIn, a proxy component will be 
     *                               used to simulate the transition off-screen.
     * 
     * @param direction              The direction Components will move during
     *                               the transition.
     * 
     * @param animationMilliseconds  Animation duration in milliseconds.
     */
    template <class T> static void animateTransition(
            T transitionIn,
            T transitionOut,
            const Direction direction,
            const unsigned int animationMilliseconds);
private:
    /**
     * Constructor is private because there should never be any reason to create 
     * a TransitionAnimator object.  
     * 
     * TODO: figure out if there's a way to resolve issues with using both 
     *       templates and private methods, so TransitionAnimator can be
     *       declared as a namespace instead of a class.
     */
    TransitionAnimator() { }
    ~TransitionAnimator() { }

    /**
     * Animates a Component* or Array<Component*>, translating them to a
     * destination rectangle.
     * 
     * @tparam T                     Either Component* or Array<Component*>
     * 
     * @param toMove                 The Component or component array being
     *                               translated.
     * 
     * @param destination            The final bounds of the Component*, or
     *                               a rectangle containing the final bounds
     *                               of all Component* in the Array.  If moving
     *                               an Array of Components, their relative
     *                               positions will be preserved.
     * 
     * @param animationMilliseconds  Duration of the translation, in ms. 
     */
    template <class T > static void translate
    (T toMove, Rectangle<int>& destination, int animationMilliseconds);

    /**
     * Duplicates the appearance of other Component objects.
     */
    class AnimationProxy :
    public ComponentAnimator::AnimationTask::ProxyComponent, private Timer
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
        AnimationProxy(Component& source, int animationDuration);

        virtual AnimationProxy() { }

    private:
        /**
         * Deletes the component if it is no longer animating.  If it is still
         * animating, the timer will be reset.
         */
        void timerCallback() override;
        
        //Saved animation duration
        const int duration;
    };

    //Stores all proxy components while they're needed for animation.
    static OwnedArray<AnimationProxy> proxies;
}