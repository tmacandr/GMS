#!/bin/bash
#----------------------------------------------------------------------
# File: buildFlatBrowse.ksh
# Date: 10-Feb-00 : initial def for SPARC ULTRA at Litton DSD
#       11-Feb-00 : back to CSUN HPs
#       15-Feb-00 : add "smarts" to build on HP, SUN, Litton
#       24-Feb-00 : point to unix version of test driver
#
# Description:
#    Script that uses the SUNWspro C++ compiler to compile and
#    link the "motifHello.cpp" program.
#----------------------------------------------------------------------

Root=$HOME/thesis

Base=$Root/C_source


H_PATH="-I/usr/include
        -I/usr/openwin/include
        -I/usr/dt/share/include
        -I$Base/include/gmsFile
        -I$Base/include/gmsGraphics"

L_PATH="-L/usr/dt/lib
        -L/usr/openwin/lib
        -L$Base/lib"

MOTIF="-lXm -lXt -lX11"

GMS="-lGmsGraph -lGmsFile"

L_LIBS="$GMS $MOTIF -lnsl -lm"

OPTIONS="-DIS_UNIX -DBIG_ENDIAN"

Src=$Root/unixBuild/mains

cd $Src

echo "________________________"
echo "---> Build: "

Exe=flatBrowseExe

gcc $OPTIONS $H_PATH $L_PATH unix_flat_browse.cpp $L_LIBS -o $Exe

if test -x $Exe; then

   mv $Exe $Root/unixBuild/bin

else

   echo "Compile/Link failed"

fi

