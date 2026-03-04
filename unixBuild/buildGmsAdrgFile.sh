#!/bin/bash
#===========================================================================
# File        : buildAdrgFile.sh
#
# Description :
#    Script to build and archive the C++ ADRG files.
#===========================================================================

HERE=`pwd`

cd ..

Root=`pwd`

cd $HERE

Base=$Root/C_source

H_PATH="-I$Base/include/gmsAdrgFile \
        -I$Base/include/gmsFile"

AR_LIB="$HERE/lib/libGmsAdrgFile.a"

if test -r $AR_LIB; then

   echo "Deleting old archive file"

   rm $AR_LIB

fi

#
# GMS macros
# ------------
#    NOT_CPP
#    IS_UNIX
#    BIG_ENDIAN
#
OPTIONS="-DIS_UNIX"

cd $Base/source/gmsAdrgFile

FILE_NAME=`ls *.cpp`

#
# Okay ... go for it
#
for eachFile in $FILE_NAME; do

   each=`echo $eachFile | sed 's/\.cpp//g'`

   echo "_______________"
   echo "Compiling $each"

   gcc $OPTIONS $H_PATH -c $each.cpp

   if test -r $each.o; then

      echo "Archiving"

      ar -r $AR_LIB $each.o

      rm $each.o

   else

      echo "File : $each did NOT compile"

      exit

   fi

done
