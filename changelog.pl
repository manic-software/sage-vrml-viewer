#!/usr/bin/perl

$started=0;

open IN, "<arcon.spec";
open OUT, ">ChangeLog";
while ($line=<IN>) {
    chomp $line;
    if ($started) {
	    print OUT "$line\n";
	} elsif ("$line" =~ /^%changelog/) {
	    $started=1;
	}
}
close IN;
close OUT;
