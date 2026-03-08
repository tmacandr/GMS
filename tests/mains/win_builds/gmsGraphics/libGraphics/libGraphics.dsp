# Microsoft Developer Studio Project File - Name="libGraphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libGraphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libGraphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libGraphics.mak" CFG="libGraphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libGraphics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libGraphics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libGraphics - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libGraphics - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libGraphics - Win32 Release"
# Name "libGraphics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gms_DN_PolygonMapClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gms_PO_PolygonMapClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsBitmapClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsBrowseMapClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsDataTablesClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsDcwUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsLatLongGridClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsLibRefClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsMapClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsMapStateMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsMbrClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsNamesOfFilesClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsNodeClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsPolygonUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsScreenCoordUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsTextClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsTileClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsTileRefClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsTransformationUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\source\gmsGraphics\gmsWorldCoordUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gms_DN_PolygonMapClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gms_PO_PolygonMapClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsBitmapClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsBrowseMapClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsColors.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsDataTablesClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsDcwTypesAndConstants.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsDcwUtilities.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsLatLongGridClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsLibRefClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsMapClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsMapStateMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsMbrClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsNamesOfFilesClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsNodeClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsPolygonUtilities.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsScreenCoordUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsTextClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsTileClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsTileRefClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsTransformationUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\C_source\include\gmsGraphics\gmsWorldCoordUtils.h
# End Source File
# End Group
# End Target
# End Project
