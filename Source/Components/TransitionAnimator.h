#pragma once
#include "JuceHeader.h"

/**
 * @file TransitionAnimator.h
 * 
 * @brief Animates the transition when replacing visible Components on the 
 *        screen, making removed components slide out while new components
 *        slide in from the opposite direction. 
 */

namespace TransitionAnimator
{
    //Defines the directions of movement available for Component transitions
    enum Direction
    {
        moveUp,
        moveDown,
        moveLeft,
        moveRight
    };

    /**
     * Replaces the on-screen components, animating the transition.
     */
    void animateTransition(
	    Array<Component*> transitionOut,
            Array<std::pair<Component*, Rectangle<int>> transitionIn,
            const Direction direction,
            const unsigned int animationMilliseconds);
    
    void transitionOut(Component* component,
	    const Direction direction,
            const unsigned int animationMilliseconds,
	    const bool useProxy = false);

    void transitionIn(Component* component,
            const Direction direction,
            const Rectangle<int> destination,
	    const unsigned int animationMilliseconds);

    void translate(Component* component,
            const  Rectangle<int>& destination,
	    const unsigned int animationMilliseconds);

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
