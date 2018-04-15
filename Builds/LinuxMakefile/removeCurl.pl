#!/usr/bin/perl
use strict;
use warnings;

# The Projucer is still the easiest way to make sure the makefiles generate
# correctly, but it insists on linking libcurl even though that option is
# disabled.  This will remove all instances of the string " libcurl" from the
# makefile.

chdir "/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Builds/LinuxMakefile";
open(my $makefile, "<", "Makefile") or die "Can't read from makefile!";

my $file = "";
my $buffer;
while(my $buffer = <$makefile>)
{
    $file = $file.$buffer;
}
close $makefile;
$file =~ s/\s?libcurl//g;
open(my $outfile, ">", "Makefile") or die "Can't write to makefile!";
print $outfile $file;
close $outfile;
