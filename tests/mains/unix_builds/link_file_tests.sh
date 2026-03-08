#!/bin/bash
#===========================================================================
# File        : link_file_tests.ksh
# Date        : xx-xxx-99 : initial definition (CSUN HP lab)
#               10-Feb-00 : use to build on SUN SparcUltra at Litton DSD
#               11-Feb-00 : back to CSUN HPUX
#               15-Feb-00 : build "smarts" to build on HP, SUN, Litton
# Author      : T. MacAndrew
#
# Description :
#    Script to build and archive the files that read
#    the files from the DCW library.
#===========================================================================

HERE=`pwd`

cd ../../../

ROOT=`pwd`

cd $HERE

H_PATH=-I$ROOT/C_source/include/gmsFile

L_PATH=-L$ROOT/unixBuild/lib

L_LIBS="-lGmsFile -lm"

OPTIONS="-ggdb -Wall -DIS_UNIX"

cd ../gmsFile

SRC_DIR=`pwd`

Test_Drivers=`ls *.cpp`

cd $HERE

for eachCpp in $Test_Drivers; do

   eachDriver=`echo $eachCpp | sed 's/\.cpp//'`

   EXE_NAME=$eachDriver.exe

   echo "_______________"
   echo "Compiling $eachCpp"

   gcc $OPTIONS $H_PATH $L_PATH $SRC_DIR/$eachCpp -o $EXE_NAME $L_LIBS

   if test ! -x $EXE_NAME; then

      echo "Exe $EXE_NAME FAILED to link."

      exit

   fi

done

echo "Done."

