# Widgets Module Documentation
    The Widgets module provides basic UI component classes used to build the application, expanding on the Component classes provided by the Juce library. It includes all custom Component classes designed to be reused throughout the application.

#### [Widgets::BoundedLabel](../../Source/Widgets/Widgets_BoundedLabel.h)
    A juce::Label component with additional options for controlling font size.

#### [Widgets::Switch](../../Source/Widgets/Widgets_Switch.h)
    An animated binary switch component.

#### [Widgets::Clock](../../Source/Widgets/Widgets_Clock.h)
    A configurable label that prints the current time.

#### [Widgets::IconSlider](../../Source/Widgets/Widgets_IconSlider.h)
    A horizontal slider component with icons displayed on both of its sides.

#### [Widgets::Spinner](../../Source/Widgets/Widgets_Spinner.h)
    A loading indicator component that shows an animated spinning circle.

#### [Widgets::OverlaySpinner](../../Source/Widgets/Widgets_OverlaySpinner.h)
    A Spinner and optional text label displayed over an overlay that covers the entire window.

#### [Widgets::FilePathEditor](../../Source/Widgets/Widgets_FilePathEditor.h)
    A text editor and button used to select and edit a file path.

#### [Widgets::DrawableImage](../../Source/Widgets/Widgets_DrawableImage.h)
    An image component that automatically scales its image to match its bounds.

#### [Widgets::DrawableImageButton](../../Source/Widgets/Widgets_DrawableImageButton.h)
    A Juce Button component that is also a DrawableImage.

#### [Widgets::Counter](../../Source/Widgets/Widgets_Counter.h)
    A counter component used to select an integer value.

#### [Widgets::NavButton](../../Source/Widgets/Widgets_NavButton.h)
    A navigation button that can be set to point up, down, left, or right.

#### [Widgets::PagedList](../../Source/Widgets/Widgets_PagedList.h)
    A list component that can split its content between multiple pages, navigated using NavButton components.

#### [Widgets::FocusingPagedList](../../Source/Widgets/Widgets_FocusingPagedList.h)
    A PagedList with selectable list items. Selected list items expand to fill the entire list component, hiding all other list content until they are deselected.

#### [Widgets::PopupEditor](../../Source/Widgets/Widgets_PopupEditor.h)
    A component used to display popup content editor tools.

#### [Widgets::ListEditor](../../Source/Widgets/Widgets_ListEditor.h)
    A PopupEditor used to edit a list of strings.

#### [Widgets::ColourPicker](../../Source/Widgets/Widgets_ColourPicker.h)
    A component used to select a color value.
