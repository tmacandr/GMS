#!/bin/ksh
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
#
# Copyright (c), Timothy MacAndrew, all rights reserved
#----------------------------------------------------------------------

if test $# -ne 1; then

   echo "Usage: buildFlatBrowse.ksh <hp | sun | litton>";

   exit;

fi

if test "$1" = "litton"; then

   export Compiler_Path=/opt/cots/SUNWspro/SC5.0/bin

   export CPP="$Compiler_Path/CC";

elif test "$1" = "hp"; then

   export Compiler_Path=/opt/aCC/bin

   export CPP="$Compiler_Path/aCC";

elif test "$1" = "sun"; then

   export Compiler_Path=/opt/cecs/bin

      export CPP="$Compiler_Path/c++";

else

    echo "Usage : buildFlatBrowse.ksh <hp | sun | litton>";

    exit;

fi

export Root=$HOME/thesis

export Base=$Root/C_source

#export SUN_H_PATH="-I$Tool_Base/include/CC
#                   -I$Tool_Base/include/cc";

export H_PATH="-I/usr/include
               -I/usr/openwin/include
               -I/usr/dt/share/include
               -I$Base/include/gmsFile
               -I$Base/include/gmsGraphics";

export L_PATH="-L/usr/dt/lib
               -L/usr/openwin/lib
               -L$Base/lib";

export MOTIF="-lXm -lXt -lX11";

export GMS="-lGmsGraph -lGmsFile";

export L_LIBS="$GMS $MOTIF -lnsl -lm";

export OPTIONS="-DIS_UNIX -DBIG_ENDIAN"

export Src=$Root/unixBuild/mains;

cd $Src;

echo "________________________";
echo "---> Build: ";

export Exe=flatBrowseExe;

$CPP $OPTIONS $H_PATH $L_PATH unix_flat_browse.cpp $L_LIBS -o $Exe;

if test -x $Exe; then

   mv $Exe $Root/unixBuild/bin;

else

   echo "Compile/Link failed";

fi

