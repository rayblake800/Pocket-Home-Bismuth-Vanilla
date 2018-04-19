#!/usr/bin/perl
use strict;
use warnings;
use File::Slurp;

# The Projucer is still the easiest way to make sure the makefiles generate
# correctly, but it insists on linking libcurl even though that option is
# disabled.  This will remove all instances of the string " libcurl" from the
# makefile.

chdir("../Builds/LinuxMakefile");
my $file = read_file("./Makefile") or die "Can't read from makefile!";
$file =~ s/\s?libcurl//g;
write_file("./Makefile", $file);
