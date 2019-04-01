##### InputMenu.pm #############################################################
# Creates and manages a text menu interface.                                   #
##### Functions: #####

#==============================================================================#
#--- new: ---
# Initializes a new InputMenu.
#--- Parameters: ---
# $title:       The title to print at the top of the menu.
#
# [$exitText]:  The text to print describing the exit command.
#
# [$action]:    The action to run when a valid option is selected, passing in
#               the option value as the first parameter. If undefined, the
#               default action attempts to run the selected option value as a
#               function reference, passing on all additional arguments.
#
# [@args]:      Additional arguments to pass to the action function.
#--- Returns: ---
# The new menu object.
#==============================================================================#

#==============================================================================#
#--- addOption: ---
# Adds a new option to the menu.
#--- Parameters: ---
# $description: Text to print describing the option.
#
# $optionData:  The first parameter to pass to the action command if the user 
#               selects this option.
#==============================================================================#

#==============================================================================#
#--- openMenu: ---
# Displays the menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#==============================================================================#

use strict;
use warnings;

# Creates a custom command line input menu.
package InputMenu;

# Initializes a new InputMenu.
sub new
{
    my $class    = shift;
    my $title    = shift;
    my $exitText = shift;
    my $action   = shift;
    my @args     = @_;
    if(!$title)
    {
        $title = "Select a menu option:";
    }
    if(!$exitText)
    {
        $exitText = "Exit";
    }
    if(!$action)
    {
        $action = sub
        {
            my $option = shift;
            my @otherArgs = @_;
            $option->(@otherArgs);
        };
    }
    my $self =
    {
        _title    => $title,
        _exitText => $exitText,
        _action   => $action,
        _args     => \@args,
        _options  => []
    };
    bless($self, $class);
    return $self;
}

# Adds a new option to the menu.
sub addOption
{
    my $self = shift;
    my $description = shift;
    my $optionData = shift;
    push(@{$self->{_options}}, [$description, $optionData]);
};

# Displays the menu, accepting input until the user enters 'q'
sub openMenu
{
    my $self = shift;
    my $input = "";
    while(!($input =~ /q/))
    {
        my $infoText = $self->{_title}."\n";
        my $index = 1;
        my $numOptions = @{$self->{_options}};
        foreach my $option(@{$self->{_options}})
        {
            $infoText = $infoText."$index: ".$option->[0]."\n";
            $index++;
        }
        $infoText = $infoText."q: ".$self->{_exitText}."\n";
        print($infoText);
        $input = <STDIN>;
        if($input =~ /(\d)/)
        {
            my $index = $1 - 1;
            if(($index >= 0) && ($index < $numOptions))
            {
                my $optionData = $self->{_options}->[$index]->[1];
                {
                    $self->{_action}->($optionData, @{$self->{_args}});
                }
            }
        }
    }

}
1;
