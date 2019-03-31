#!/usr/bin/perl

# ColourIds Namespace Generator
#
# usage: 
#   perl ColourIdsNamespace.pl [project directory] [output file]
#
#  This script will recursively scan directories to find header files with
# ColourId enums.  It extracts all those enums, and copies their values into
# new enums named after classes within a single ColourIds namespace. This 
# makes it easy to ensure any new ColourId values are unique, and allows
# classes that manage colors to reference ColourIds without having to copy the
# ids over or include every class.
#
use strict;
use warnings;
use Scalar::Util qw(blessed);
use Scalar::Util::Numeric qw(isint);
use File::Slurp;
use lib './project-scripts/ColourID';
use IOUtils;

sub enumScan
{
    print("TODO: Implement enum scan\n");
}

sub namespaceMenu
{
    my $cache = shift;
    IOUtils::printNamespaces($cache);
    print("TODO: Implement namespace menu\n");
}

sub keyMenu
{
    print("TODO: Implement key menu\n");
}

sub categoryMenu
{
    print("TODO: Implement key menu\n");
}

sub updateFiles
{
    print("TODO: Implement file update\n");
}

sub mainMenu
{
    my $cache = shift;
    my $input = "0";
    while(!($input =~ /q/))
    {
        print("ColourId management tool options:\n");
        print("1: Scan project for new ColourId enums:\n");
        print("2: Edit Element namespaces:\n");
        print("3: Edit configuration keys:\n");
        print("4: Assign UI categories:\n");
        print("5: Update Colour files:\n");
        print("q: Exit program:\n");
        my @options =
        (
            \&enumScan,
            \&namespaceMenu,
            \&keyMenu,
            \&categoryMenu,
            \&updateFiles
        );
        $input = <STDIN>;
        if($input =~ /(\d)/)
        {
            my $index = $1 - 1;
            my $action = $options[$index];
            if(exists($options[$index]))
            {
                $options[$index]->($cache);
            }
        }
    }
}


my $cache = new IDCache();
IOUtils::cacheCodeFiles($cache);
mainMenu($cache);
