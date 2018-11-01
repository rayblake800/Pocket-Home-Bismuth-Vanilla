#pragma once
#include "JuceHeader.h"

/**
 * @file TransitionAnimator.h
 * 
 * @brief Animates juce::Component transitions.
 *
 * TransitionAnimator animates the transition when replacing visible Components 
 * on the screen, making removed components slide out while new components slide
 * in from the opposite direction.  
 */

namespace TransitionAnimator
{
    //Lists all available transition animations
    enum Transition
    {
        moveUp,
        moveDown,
        moveLeft,
        moveRight,
        toDestination,
        none
    };

    /**
     * @brief  Checks if a component is being animated through the default
     *         juce Animator.
     *
     * @param possiblyAnimating  A component that is possibly being animated.
     *
     * @return                   Whether the component is being animated by the
     *                           dedault animator.
     */
    bool isAnimating(juce::Component* possiblyAnimating);

    /**
     * @brief  Moves one set of components off-screen, while moving another set 
     *         of components on-screen, animating the transition.  
     *
     * Any component that is in both sets will appear to move off-screen while a
     * duplicate component moves on-screen simultaneously.
     * 
     * @param movingOut              An array of all components that will move
     *                               off of the screen.  If a component in this
     *                               array is also in the transitionIn list,
     *                               a proxy component will be created to slide
     *                               off-screen.  Any changes made to the
     *                               component after calling this method will
     *                               not affect the proxy component.
     * 
     * @param movingIn               An array of all components that will move
     *                               onto the screen, paired with their target
     *                               bounding rectangle.  Components will be
     *                               moved before animating so that they slide 
     *                               in from the correct direction.
     * 
     * @param transition             The animation type to apply to all moving
     *                               Components.
     * 
     * @param animationMilliseconds  Duration of the component animation, in
     *                               milliseconds.
     */
    void animateTransition(
        juce::Array<juce::Component*> movingOut,
        juce::Array<std::pair<juce::Component*,juce::Rectangle<int>>> movingIn,
        const Transition transition,
        const unsigned int animationMilliseconds);
    
    /**
     * @brief  Moves a component off-screen, animating the transition.
     * 
     * @param component              An on-screen component to move off-screen.  
     *                               If the component is already outside of the
     *                               screen bounds, nothing will happen.
     * 
     * @param transition             The animation type used to move the
     *                               component out of screen bounds.
     * 
     * @param animationMilliseconds  The duration of the component animation, in
     *                               milliseconds.
     *
     * @param useProxy               If true, the component will immediately be
     *                               moved to its final bounds, and a temporary
     *                               proxy component will be used to simulate
     *                               the transition animation.
     */
    void transitionOut(
        juce::Component* const component,
        const Transition transition,
        const unsigned int animationMilliseconds,
        const bool useProxy = false);

    /**
     * Moves a component into the screen bounds, animating the transition.
     * 
     * @param component              A component to move onto the screen. Before
     *                               animating, it will be re-positioned outside
     *                               the screen bounds to ensure it moves in
     *                               from the right direction.
     * 
     * @param transition             The animation type used to move the
     *                               component onto screen bounds.
     * 
     * @param destination            The destination bounds of the component,
     *                               relative to its parent component.  If this
     *                               does not intersect with the screen bounds,
     *                               the component will not be re-positioned or
     *                               animated.
     * 
     * @param animationMilliseconds  The duration of the component animation, in
     *                               milliseconds.
     */
    void transitionIn(juce::Component* component,
	    const Transition transition,
        const juce::Rectangle<int> destination,
	    const unsigned int animationMilliseconds);

    /**
     * Updates a component's bounds, animating the transformation.
     * 
     * @param component              The component being transformed.
     * 
     * @param destination            The new bounds to apply to the component,
     *                               relative to its parent component.
     * 
     * @param animationMilliseconds  The duration of the component animation, in
     *                               milliseconds.
     *
     * @param useProxy               Sets whether a proxy component should be
     *                               animated in place of the real component.
     *
     * @param onFinish               An optional function to run once animation
     *                               is finished.
     */
    void transformBounds(
        juce::Component* component,
        const juce::Rectangle<int>& destination,
        const unsigned int animationMilliseconds,
        const bool useProxy = false,
        const std::function<void()> onFinish = std::function<void()>());
};
