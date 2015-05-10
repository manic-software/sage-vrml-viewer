#!/usr/bin/perl

open PRO, ">tmp.pro";
print PRO "QT+=qt3support\n";
print PRO "QT+=opengl\n";
close PRO;

open MAKE, ">makefile.qt";
open QMAKE, "qmake -o - tmp.pro|";
while ($line=<QMAKE>) {
    chomp($line);
    if ($line !~ /^#/) {
	($para, $val)=$line =~ /^(\w+)\s*=\s*(.+)/;
	if ($para=~"CXXFLAGS|LIBS|LFLAGS|INCPATH") {
	    print MAKE "QT_$para:=$val\n"; 
	    if ($para=~"INCPATH") {
		$val=~s/-I\.//g;
		$val=~s/-I/-isystem /g;
		print MAKE "QT_INCDEP:=$val\n";
	    }
	}
    }
}
close QMAKE;

#unlink "tmp.pro";
