#!/usr/bin/perl
use strict;
use warnings;

my @filenames = (
        "FolderComponent",
        "Initializer",
        "InputHandler",
        "MenuButton",
        "MenuComponent");

my @formats = (
        "Scrolling",
        "Paged",
        "Tabbed");

my $formatDir = "./Source/AppMenu/MenuFormats";

foreach my $format (@formats)
{
    foreach my $file(@filenames)
    {
        system("perl", "./dev scripts/cpp_from_h.pl",
                "$formatDir/$format/$file.h",
                "$formatDir/$format/$file.cpp");
    }
}
