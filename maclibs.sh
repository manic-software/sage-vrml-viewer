#!/bin/bash

exe=$1
oldlibdir=$2
newloc=$3
copyto=$4

echo "changing embedded library paths \c"
for lib in `otool -L $exe | grep $oldlibdir | cut -d ' ' -f 1`
do
    install_name_tool -change $lib $newloc/`basename $lib` $exe
    if [ -e $lib ]; then 
	cp $lib $copyto/
    else 
	cp $oldlibdir/`basename $lib` $copyto/
    fi
    libs="$libs `basename $lib`"
    install_name_tool -id `basename $lib` $copyto/`basename $lib`
    echo ".\c"
    echo relocating `basename $lib`
done
dir=`pwd`
cd $copyto
for item in $libs
do
    for lib in `otool -L $item | grep $oldlibdir | cut -d ' ' -f 1`
    do
	install_name_tool -change $lib $newloc/`basename $lib` $item
	if [ ! -e `basename $lib` ]; then
	    if [ -e $lib ]; then 
		cp $lib .
	    else 
		cp $oldlibdir/`basename $lib` .
	    fi
	fi
	install_name_tool -id `basename $lib` `basename $lib`
    echo ".\c"
    done
done
cd $dir
echo
