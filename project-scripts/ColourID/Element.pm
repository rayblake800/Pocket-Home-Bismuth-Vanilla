# Element.pm
# Represents a Theme::Colour::Element object.
use strict;
use warnings;

package Element;
use lib './project-scripts/ColourID';
use Category;
use Scalar::Util qw(blessed);

# Constructs a new element:
# namespace: The element's namespace name.
# name:      The element's object name.
# id:        The element's hex ID string.
# Category:  The element's UICategory object.
#
# return:    A new element object.
sub new
{
    my $class = shift;
    my $self =
    {
        _namespace => shift,
        _name      => shift,
        _id        => shift,
        _category  => new Category(shift),
        _key       => ""
    };
    bless($self, $class);
    return $self;
}

# Parses and returns an element from a block of text
# text:       A block of text to search for a single Element declaration.
# namespace:  The namespace name to apply to the new Element.
#
# return:     The parsed element object, or undef if no Element was found.
sub parseElement
{
    my $text = shift;
    # parseElement can be called safely with or without an existing Element.
    if(blessed($text) && blessed($text) eq 'Element')
    {
        $text = shift;
    }
    my $namespace = shift;
    if($text =~ /
                (?:Element\s+)?
                (\w+)\s*\(\s*           # match object name
                (0x[0-9a-fA-F]+),\s*    # match hex ID
                UICategory::(\w+)\s*\); # match category name
            /sx)
    {
        my $name = $1;
        my $hexValue = $2;
        my $category = new Category($3);
        if(defined($category))
        {
            return new Element($namespace, $name, $hexValue, $category);
        }
        else
        {
            print("Element::parseElement: Invalid category $category\n");
        }
    }
    return undef;
}

# Gets the Element's containing namespace
sub getNamespace
{
    my $self = shift;
    return $self->{_namespace};
}

# Gets the Element's object name
sub getName
{
    my $self = shift;
    return $self->{_name};
}

# Gets the Element's hex ColourId value
sub getID
{
    my $self = shift;
    return $self->{_id};
}

# Gets the Element's UICategory name
sub getCategory
{
    my $self = shift;
    return $self->{_category};
}

# Gets the Element's optional color key
sub getKey
{
    my $self = shift;
    return $self->{_key};
}

# Sets the Element's UICategory 
# category: A new category value, key, name, or object to save.
sub setCategory
{
    my $self = shift;
    my $newCategory = new Category(shift);
    $self->{_category} = $newCategory;
}

# Sets the Element's optional color key
# key: The new key string to save
sub setKey
{
    my $self = shift;
    $self->{_key} = shift;
}

# Gets 'Namespace::Name' to use as a unique key for storing and finding 
# elements.
sub getNameKey
{
    my $self = shift;
    return $self->getNamespace()."::".$self->getName();
}

# Gets a C++ declaration for the Element as a Theme::Colour::Element constant.
# indent: number of indentations to place before the declaration.
sub getDeclaration
{
    my $self = shift;
    my $indent = shift;
    if(defined($indent))
    {
        $indent = " " x ($indent*4);
    }
    else
    {
        $indent = "";
    }
    return $indent."static const Element ".$self->getName().";";
}

# Gets a C++ definition for the Element as a Theme::Colour::Element constant.
# indent: number of indentations to place before the definition.
sub getDefinition
{
    my $self = shift;
    my $indent = shift;
    if(defined($indent))
    {
        $indent = " " x ($indent*4);
    }
    else
    {
        $indent = "";
    }
    return $indent."const Element ".$self->getNameKey()."(\n"
            .$indent.(" " x 8).$self->getID()
            .", UICategory::".$self->getCategory()->getTypeName().");";
}

# Gets the full name used to reference the Element in C++
sub getFullName
{
    my $self = shift;
    return $self->getNamespace()."::".$self->getName();
}

1;
