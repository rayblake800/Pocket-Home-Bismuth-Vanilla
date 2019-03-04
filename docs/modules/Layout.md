# Layout Module Documentation
The Layout module handles the placement of UI elements within the window. UI components can be individually placed at configurable locations using Layout::Component::ConfigLayout, or positioned in groups using Layout::Group::RelativeLayout. The Layout module also provides tools for animating UI components when applying new layouts.

## Relative Group Layout
The Relative Layout submodule defines the positions and sizes of UI components in relation to each other. It can be used to quickly define the placement of many non-overlapping UI components within a rectangular area.

#### [Layout\::Group\::RelativeLayout](../../Source/Layout/Group/Layout_Group_RelativeLayout.h)
RelativeLayout objects hold layout values for a group of UI components. Each layout contains any number of layout rows, and each row contains any number of UI components. Each row is assigned a weight value used to calculate how much vertical space it is allocated relative to all other rows. Each component in the row also holds a weight value, defining how much horizontal space it is allocated relative to other components in the row.

#### [Layout\::Group\::Manager](../../Source/Layout/Group/Layout_Group_Manager.h)
Manager objects apply a RelativeLayout object's layout to the components it contains.

## Configurable Component Layout
The Configurable Component Layout submodule defines the positions of individual UI components within the application window. Component layout values are relative to the size of the main application window. Width and x-coordinate are expressed as a fraction of the window's width, and height and y-coordinate are expressed as a fraction of the window's height. Component layouts are defined within a JSON configuration file the user may edit. This configuration file also provides the preferred font sizes used within the application.

#### [Layout\::Component\::ConfigLayout](../../Source/Layout/Component/Layout_Component_ConfigLayout.h)
ConfigLayout objects hold the position and size of a single UI component.

#### [Layout::Component::Manager](../../Source/Layout/Component/Layout_Component_Manager.h)
Manager objects load and apply a component layout to a UI component.

#### [Layout::Component::TextSize](../../Source/Layout/Component/Layout_Component_TextSize.h)
TextSize lists all font size options defined in the component configuration file.

#### [Layout::Component::ConfigFile](../../Source/Layout/Component/Layout_Component_ConfigFile.h)
ConfigFile objects access the component file resource to load component layouts and calculate appropriate font sizes.

#### [Layout::Component::JSONResource](../../Source/Layout/Component/Layout_Component_JSONResource.h)
The shared JSONResource instance manages the configuration file where component layouts and font sizes are defined.

#### [Layout::Component::JSONKeys](../../Source/Layout/Component/Layout_Component_JSONKeys.h)
JSONKeys provides the keys used to access font sizes and component layouts within the component file resource.

## Layout Transition Animations
The Layout module provides methods for animating transitions between layouts, or for animating components when their initial layout is loaded.

#### [Layout::Transition::Animator](../../Source/Layout/Transition/Layout_Transition_Animator.h)
The Animator namespace provides functions for animating components in new layouts as they are applied, and for animating components in old layouts as they are removed.

#### [Layout::Transition::Type](../../Source/Layout/Transition/Layout_Transition_Type.h)
Transition::Type defines all types of animation that may be created using transition animator functions.



