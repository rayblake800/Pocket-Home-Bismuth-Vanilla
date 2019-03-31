# Namespace.pm
# Represents an inner ColourId namespace containing Element object definitions
use strict;
use warnings;

package Namespace;
use lib './project-scripts/ColourID';
use Element;
use Scalar::Util qw(blessed);

# Constructs a new namespace:
# title:   The namespace title
#
# return: A new Namespace object.
sub new
{
    my $class = shift;
    my $self =
    {
        _title    => shift,
        _elements => []
    };
    bless($self, $class);
    return $self;
}

# Gets the namespace title
sub getTitle
{
    my $self = shift;
    return $self->{_title};
}

# Sorts the internal Element list by ID value
sub sortElements
{
    my $self = shift;
    @{$self->{_elements}} = sort {hex($a->getID()) <=> hex($b->getID())}
            @{$self->{_elements}};
}

# Inserts an element into its sorted position in the namespace
sub addElement
{
    my $self = shift;
    my $element = shift;
    if(blessed($element) eq 'Element')
    {
        push(@{$self->{_elements}}, $element);
        sortElements();
    }
    else
    {
        die "Invalid element $element";
    }
}

# Gets all Element objects in the namespace
sub getElements
{
    my $self = shift;
    return @{$self->{_elements}};
}

# Gets the ID value of the first Element in the namespace
# Return: The ID as a number, or -1 if the namespace is empty
sub getFirstID
{
    my $self = shift;
    if((scalar @{$self->{_elements}}) == 0)
    {
        return -1;
    }
    return hex(${$self->{_elements}}[0]->getID());
}

# Returns the namespace's c++ declarations
sub getDeclarationText()
{
    my $self = shift;
    my $decl = "namespace ".$self->getTitle()."\n{\n";
    my @elements = $self->getElements();
    foreach my $element(@elements)
    {
       $decl = $decl.$element->getDeclaration(1)."\n"; 
    }
    return $decl."}\n";
}

# Returns the namespace's c++ definitions
sub getDefinitionText()
{
    my $self = shift;
    my $def = "";
    my @elements = $self->getElements();
    foreach my $element(@elements)
    {
       $def = $def.$element->getDefinition()."\n"; 
    }
    return $def."\n";
}

1;
