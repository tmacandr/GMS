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

Base=..

Root=$Base/C_source

H_PATH=-I$Root/include/gmsFile

L_PATH=-L$Root/lib

L_LIBS="-lGmsFile -lm"

BIN_DIR=$Base/unixBuild/bin

OPTIONS="-DIS_UNIX -DBIG_ENDIAN"

cd $Root/mains/gmsFile

Test_Drivers=`ls *.cpp`

for eachCpp in $Test_Drivers; do

    eachDriver=`echo $eachCpp | sed 's/\.cpp//'`

    EXE_NAME=$eachDriver.exe

   echo "_______________"
   echo "Compiling $eachCpp"

   $CPP $OPTIONS $H_PATH $L_PATH $eachCpp -o $EXE_NAME $L_LIBS

   if test -x $EXE_NAME; then

      $MV $EXE_NAME $BIN_DIR

   else

      echo "Exe $EXE_NAME FAILED to link."

      exit

   fi

done

echo "Okay ... done."

