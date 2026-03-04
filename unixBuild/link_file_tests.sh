#!/bin/ksh
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

if test $# -ne 1; then

   echo "Usage: link_file_tests.ksh <hp | sun | litton>";
   
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

   echo "Usage : link_file_tests.ksh <hp | sun | litton>";

   exit;

fi

export MV=mv

export Base=$HOME/thesis

export Root=$Base/C_source;

export H_PATH=-I$Root/include/gmsFile;

export L_PATH=-L$Root/lib;

export L_LIBS="-lGmsFile -lm"

export BIN_DIR=$Base/unixBuild/bin;

export OPTIONS="-DIS_UNIX -DBIG_ENDIAN";

cd $Root/mains/gmsFile;

export Test_Drivers=`ls *.cpp`;

for eachCpp in $Test_Drivers; do

   export eachDriver=`echo $eachCpp | sed 's/\.cpp//'`;

   export EXE_NAME=$eachDriver.exe;

   echo "_______________";
   echo "Compiling $eachCpp"

   $CPP $OPTIONS $H_PATH $L_PATH $eachCpp -o $EXE_NAME $L_LIBS

   if test -x $EXE_NAME; then

      $MV $EXE_NAME $BIN_DIR

   else

      echo "Exe $EXE_NAME FAILED to link.";

      exit;

   fi

done

echo "Okay ... done."

