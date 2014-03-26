#!/usr/bin/perl -w
use strict;

#given a file, output a list of file, each file contains a line of the input file. file is named by the name prefix given by command line parameter and line number.

(@ARGV==2)||die "Usage: prgram <truth file> <predicted file> \n";

my ($truth, $predicted)=@ARGV;

open(IN,$truth)||die "can not open $truth";
open(IN1,$predicted)|| die "can not open $predicted";

my $total=0;
my $wrong=0;
my $pline;
my @tags1;
my @tags2;
my $linenum=0;
while(my $line = <IN>)
{
    $linenum++;
    chomp $line;
    $line =~s/^\s+//;
    $line =~s/\s+$//;
    $pline = <IN1>;
    chomp $pline;
    @tags1 = split(/\s+/,$line);
    @tags2 = split(/\s+/,$pline);
    pop(@tags2);
    for(my $i=1;$i<@tags1;$i=$i+2)
    {
	$total++;
	if($tags1[$i] ne $tags2[$i])
	{
	    print "L:$linenum ","$tags1[$i-1]","[",$tags1[$i],"] -> ",$tags2[$i-1],"[", $tags2[$i],"]\n";
	    $wrong++;
	}
    }
}

my $percent = ($total-$wrong)/$total;
print "total:$total, wrong:$wrong, accurcy:$percent\n";
