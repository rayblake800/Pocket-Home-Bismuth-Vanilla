#include "TransitionAnimator.h"

OwnedArray<AnimationProxy> TransitionAnimator::proxies;

template<> void TransitionAnimator::translate
(Component* toMove, Rectangle<int>& destination, int animationMilliseconds)
{
    Desktop::getInstance().getAnimator().animateComponent(toMove,
            destination,
            1.0,
            animationMilliseconds,
            0.2,
            1.0);
}

