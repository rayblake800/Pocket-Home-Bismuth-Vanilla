# Category.pm
# Represents a Theme::Colour::UICategory value

use strict;
use warnings;

package Category;
use Scalar::Util qw(blessed);
use Scalar::Util::Numeric qw(isint);

# Category types:
use constant WINDOW_BACKGROUND      => 0;
use constant WIDGET_BACKGROUND      => 1;
use constant WIDGET_ON              => 2;
use constant WIDGET_OFF             => 3;
use constant MENU_BACKGROUND        => 4;
use constant OUTLINE                => 5;
use constant FOCUSED_OUTLINE        => 6;
use constant TEXT                   => 7;
use constant HIGHLIGHTED_TEXT       => 8;
use constant TEXT_FIELD             => 9;
use constant HIGHLIGHTED_TEXT_FIELD => 10;
use constant NONE                   => 11;

use constant NUM_TYPES => 12;

my @categoryNames = (
        'windowBackground',
        'widgetBackground',
        'widgetOn',
        'widgetOff',
        'menuBackground',
        'outline',
        'focusedOutline',
        'text',
        'highlightedText',
        'textField',
        'highlightedTextField',
        'none',
);

my @categoryKeys = (
        'Window background',
        'Widget background',
        'Widget',
        'Widget(Off)',
        'Menu background',
        'Outline',
        'Focused outline',
        'Text',
        'Highlighted text',
        'Text field',
        'Highlighted text field',
        'none',
);

# Create a Category object from a category value, another category, a category
# name, or a category key.
sub new
{
    my $class = shift;
    my $type = shift;
    my $self = undef;
    if(isint($type) && ($type >= 0) && ($type < NUM_TYPES))
    {
        $self = 
        {
            _value => $type
        };
    }
    elsif(blessed($type) && blessed($type) eq 'Category')
    {
        $self =
        {
            _value => $type->getValue()
        };
    }
    sub checkNames
    {
        my $self = shift;
        my $type = shift;
        my @nameArray = @_;
        if(!defined($self))
        {
            foreach my $i(0 ... (NUM_TYPES - 1))
            {
                if($type eq $nameArray[$i])
                {
                    $self =
                    {
                        _value => $i
                    };
                    return $self;
                }
            }
        }
        return $self;
    }
    $self = checkNames($self, $type, @categoryNames);
    $self = checkNames($self, $type, @categoryKeys);
    if(defined($self))
    {
        bless($self, $class);
    }
    return $self;
}

# Gets the Category's numeric category value
sub getValue
{
    my $self = shift;
    return $self->{_value};
}

# Gets the Category's value type name
sub getTypeName
{
    my $self = shift;
    return $categoryNames[$self->{_value}];
}

# Gets the Category's value key
sub getKey
{
    my $self = shift;
    return $categoryKeys[$self->{_value}];
}

1;
