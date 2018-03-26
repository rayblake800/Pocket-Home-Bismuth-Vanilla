use strict;
use warnings;

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
