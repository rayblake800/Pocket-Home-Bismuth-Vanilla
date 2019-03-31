# IDCache.pm
# Stores a set of cached ColourId Element Namespace objects
use strict;
use warnings;

package IDCache;
use lib './project-scripts/ColourID';
use Namespace;
use Scalar::Util qw(blessed);

# Constructs a new cache object
sub new
{
    my $class = shift;
    my $self =
    {
        _namespaces => {}, # Store namespace objects by name
        _elements   => {}, # Store element objects by name
        _keys       => {}, # Store element objects by key
        _ids        => {} # Store element objects by ID
    };
    bless($self, $class);
    return $self;
}

# Searches for an Element by nameKey, key, or ID
sub findElement
{
    my $self = shift;
    my $searchValue = shift;
    my $element = $self->{_elements}->{$searchValue};
    if(exists($self->{_elements}->{$searchValue}))
    {
        return $self->{_elements}->{$searchValue};
    }
    if(exists($self->{_keys}->{$searchValue}))
    {
        return $self->{_keys}->{$searchValue};
    }
    if(exists($self->{_ids}->{$searchValue}))
    {
        return $self->{_ids}->{$searchValue};
    }
    return undef;
}

# Searches for a namespace object by name
sub findNamespace
{
    my $self = shift;
    my $name = shift;
    if(exists($self->{_namespaces}->{$name}))
    {
        return $self->{_namespaces}->{$name};
    }
    return undef;
}

# Checks if the cache contains an element with a specific ID string
sub hasID
{
    my $self = shift;
    my $id = shift;
    return defined($id) && exists($self->{_ids}->{$id})
            && defined($self->{_ids}->{$id});
}

# Gets the value used to sort a specific element by ID
# searchValue:  A string used to find the element
# return:       The element's ID value, or -1 if the element isn't found.
sub sortValue
{
    my $self = shift;
    my $searchValue = shift;
    my $element = $self->findElement($searchValue);
    if(!defined($element))
    {
        return -1;
    }
    return hex($element->getID());
}

# Gets the list of namespace names, sorted by held element IDs
sub getNamespaceNames
{
    my $self = shift;
    my @names = keys(%{$self->{_namespaces}});
    sub sortVal
    {
        my $name = shift;
        my $self = shift;
        return $self->{_namespaces}->{$name}->getFirstID();
    }
    return sort {sortVal($a, $self) <=> sortVal($b, $self)} @names;
}

# Gets a stored Namepace object.
# name: The name of the object to find.
sub getNamespace
{
    my $self = shift;
    my $name = shift;
    if(exists($self->{_namespaces}->{$name}))
    {
        return $self->{_namespaces}->{$name};
    }
    return undef;
}

# Gets the list of element nameKeys, sorted by element ID
sub getElementNames
{
    my $self = shift;
    my @names = keys(%{$self->{_elements}});
    return sort {$self->sortValue($a) <=> $self->sortValue($b)} @names;
}

# Gets the list of element key values, sorted by element ID
sub getElementKeys
{
    my $self = shift;
    my @names = keys(@{$self->{_keys}});
    return sort {$self->sortValue($a) <=> $self->sortValue($b)} @names;
}

# Adds an Element object to the cache
sub addElement
{
    my $self = shift;
    my $element = shift;
    if(!blessed($element) || blessed($element) ne 'Element')
    {
        die "Invalid Element param $element!\n";
    }
    $self->{_elements}->{$element->getNameKey()} = $element;
    my $nsName = $element->getNamespace();
    my $namespace = $self->{_namespaces}->{$nsName};
    if(!defined($namespace))
    {
        $namespace = new Namespace($nsName);
        $self->{_namespaces}->{$nsName} = $namespace;
    }
    $namespace->addElement($element);
    my $key = $element->getKey();
    if($key)
    {
        $self->{_keys}->{$key} = $element;
    }
    $self->{_ids}->{$element->getID()} = $element;
}

# Assigns a key string to an Element
# searchVal:  The Namespace::Name, current key, or ID value of the element to 
#             update.
# key:        The new key string to assign
sub assignKey
{
    my $self = shift;
    my $searchVal = shift;
    my $key = shift;

    if(exists($self->{_keys}->{$key}))
    {
        print("IDCache::assignKey: key $key is already in use!\n");
        return;
    }
    my $element = $self->findElement($searchVal);
    if(defined($element))
    {
        my $currentKey = $element->getKey();
        if($currentKey)
        {
            delete($self->{_keys}->{$currentKey});
        }
        $element->setKey($key);
        $self->{_keys}->{$key} = $element;
    }
    else
    {
        print("IDCache::assignKey: failed to find element $searchVal\n");
    }
};
1;
