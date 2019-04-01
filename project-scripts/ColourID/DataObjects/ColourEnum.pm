##### ColourEnum.pm ############################################################
# Stores data parsed from a juce::ColourId enum definition.
################################################################################ 

##### Functions: #####

#==============================================================================#
#--- new: ---
# Creates a new ColourEnum object.
#--- Parameters: --- 
# $namespace:  The namespace name assigned to the enum value.
#
# $text:       The full text of the C++ enum declaration.
#--- Returns: ---
# The new ColourEnum object.
#==============================================================================#

#==============================================================================#
#--- getNamespaceName: ---
# Gets the ColourEnum's namespace name.
#--- Returns: ---
# The namespace identifier assigned to the object on construction.
#==============================================================================#

#==============================================================================#
#--- getSize: ---
# Gets the number of items defined in the enum.
#--- Returns: ---
# The number of name/ID pairs the enum contains.
#==============================================================================#

#==============================================================================#
#--- getNames: ---
# Gets the names of all ColourEnum items.
#--- Returns: ---
# All colour value names, sorted numerically by corresponding ID number.
#==============================================================================#

#==============================================================================#
#--- getIDs: ---
# Gets the integer values of all ColourEnum items.
#--- Returns: ---
# All colour integer IDs, sorted numerically.
#==============================================================================#

use strict;
use warnings;

package ColourEnum;
# Constructs a new ColourEnum:
sub new
{
    my $class = shift;
    my $self =
    {
        _namespace => shift,
        _names     => [],
        _ids       => []
    };
    my $text = shift;
    while($text =~ /
            ^(.*?\s* (\w+)          
            \s*=\s*
            (0x[\da-f]{7})  
            \s*)/sx)
    { 
        my $blockLen = length($1);
        my $name = $2;
        my $id = $3;
        $name =~ s/Colour//;
        $name =~ s/Id//;
        push(@{$self->{_names}},$name);
        push(@{$self->{_ids}},$id);
        $text = substr($text,$blockLen);
    }
    bless($self, $class);
    return $self;
}

# Gets the ColourEnum's namespace name.
sub getNamespaceName
{
    my $self = shift;
    return $self->{_namespace};
}

# Gets the number of items defined in the enum.
sub getSize
{
    my $self = shift;
    return scalar @{$self->{_names}};
}

# Gets the names of all ColourEnum items.
sub getNames
{
    my $self = shift;
    return @{$self->{_names}};
}

# Gets the integer values of all ColourEnum items.
sub getIDs
{
    my $self = shift;
    return @{$self->{_ids}};
}
1;
