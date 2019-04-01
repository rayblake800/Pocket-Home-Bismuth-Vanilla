##### Element.pm  ##############################################################
# Represents a C++ Theme::Colour::Element object.
################################################################################

##### Functions: #####

#==============================================================================#
#--- new ---
# Creates a new Element object from element data params.
#--- Parameters --- 
# $namespace:   The element's namespace name.
#
# $name:        The element's object name.
#
# $id:          The element's integer ID value.
#
# [$category]:  The element's UICategory object, a value that can be used to
#               construct a valid Category object, or no value to default to
#               Category::NONE.
#--- Returns ---
# The new element object, or undef if invalid parameters were given.
#==============================================================================#

#==============================================================================#
#--- new: ---
# Creates a new Element object by parsing a C++ Element definition.
#--- Parameters: --- 
# $namespace: The element's namespace name.
#
# $text:      The full text of a C++ Element definition.
#--- Returns: ---
# The new element object parsed from the C++ declaration, or undef if the text
# was not a valid declaration.
#==============================================================================#

#==============================================================================#
#--- getNamespace: ---
# Gets the name of the namespace that contains the Element.
#--- Returns: ---
# The namespace name provided on construction.
#==============================================================================#

#==============================================================================#
#--- getName: ---
# Gets the Element's colour value name.
#--- Returns: ---
# The name of the UI colour element the object represents.
#==============================================================================#

#==============================================================================#
#--- getID: ---
# Gets the Element's ColourId value.
#--- Returns: ---
# The element's hexadecimal number string value.
#==============================================================================#

#==============================================================================#
#--- getCategory: ---
# Gets the element's Category enum object.
#--- Returns: ---
# The UI element category that best matches the Element
#==============================================================================#

#==============================================================================#
#--- setCategory: ---
# Sets the element's Category enum object
#--- Parameters: --- 
# $newCategory: A value that can be used to construct a valid Category object.
#==============================================================================#

#==============================================================================#
#--- getKey: ---
# Gets the element's optional JSON colour key.
#--- Returns: ---
# The key string used to store the element's colour in the colours.json file,
# or the empty string if no key is set.
#==============================================================================#

#==============================================================================#
#--- setKey: ---
# Sets the element's optional JSON colour key.
#--- Parameters: --- 
# $key:  The new JSON key string the element should hold.
#==============================================================================#

#==============================================================================#
#--- getFullName: ---
# Gets the full name used to identify the Element in C++.
#--- Returns: ---
# The full name including namespace, formatted as Namespace::Name.
#==============================================================================#

#==============================================================================#
#--- getDeclaration: ---
# Gets a C++ Theme::Colour::Element constant declaration using this object's 
# values.
#--- Parameters: --- 
# [$indent]: Optional number of indentations to include at the beginning of
#            each line in the returned declaration.
#--- Returns: ---
# An Element declaration that may be used in a C++ header file.
#==============================================================================#

#==============================================================================#
#--- getDefinition: ---
# Gets a C++ Theme::Colour::Element constant definition using this object's 
# values.
#--- Parameters: --- 
# [$indent]: Optional number of indentations to include at the beginning of
#            each line in the returned definition.
#--- Returns: ---
# An Element definition that may be used in a C++ source file.
#==============================================================================#

#==============================================================================#
#--- sortValue: ---
# Gets a value that may be used when sorting Elements by ID.
#--- Returns: ---
# The numeric representation of the Element ID value.
#==============================================================================#
use strict;
use warnings;

package Element;
use Scalar::Util qw(blessed);
use lib './project-scripts/ColourID/DataObjects';
use Category;

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
    my $namespace = shift;
    my $name, my $id, my $category;
    my $numParams = @_;
    if($numParams == 1)
    {
        my $text = shift;
        if($text =~ /
                    (?:Element\s+)?
                    (\w+)\s*\(\s*           # match object name
                    (0x[0-9a-fA-F]+),\s*    # match hex ID
                    UICategory::(\w+)\s*\); # match category name
                /sx)
        {
            $name = $1;
            $id = $2;
            $category = $3;
        }
    }
    else
    {
        $name = shift;
        $id = shift;
        $category = shift;
    }
    if(!defined($category))
    {
        $category = Category::NONE;
    }
    $category = new Category($category);
    if(defined($namespace) && defined($name) && defined($id) 
            && defined($category))
    {
        my $self =
        {
            _namespace => $namespace,
            _name      => $name,
            _id        => $id,
            _category  => $category,
            _key       => ""
        };
        bless($self, $class);
        return $self;
    }
    return undef;
}

# Gets the name of the namespace that contains the Element.
sub getNamespace
{
    my $self = shift;
    return $self->{_namespace};
}

# Gets the Element's colour value name.
sub getName
{
    my $self = shift;
    return $self->{_name};
}

# Gets the Element's ColourId value.
sub getID
{
    my $self = shift;
    return $self->{_id};
}

# Gets the element's Category enum object.
sub getCategory
{
    my $self = shift;
    return $self->{_category};
}

# Sets the element's Category enum object
sub setCategory
{
    my $self = shift;
    my $newCategory = new Category(shift);
    $self->{_category} = $newCategory;
}

# Gets the element's optional JSON colour key.
sub getKey
{
    my $self = shift;
    return $self->{_key};
}

# Sets the element's optional JSON colour key.
sub setKey
{
    my $self = shift;
    $self->{_key} = shift;
}

# Gets the full name used to identify the Element in C++.
sub getFullName
{
    my $self = shift;
    return $self->getNamespace()."::".$self->getName();
}

# Gets a C++ declaration for the Element as a Theme::Colour::Element constant.
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

# Gets a value that may be used when sorting Elements by ID.
sub sortValue
{
    my $self = shift;
    return hex($self->getID() || -1);
}
1;
