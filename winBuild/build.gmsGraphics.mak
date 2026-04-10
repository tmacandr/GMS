#=============================================================================
# FILE: build.gmsGraphics.mak
#
# DESCRIPTION:
#    Visual Studio NMAKE file for building libGmsGraphics.lib
#
#    Use the 'build.bat' script to invoke.  It sets the Visual Studio
#    environment needed to build.
#
# RUN:
#   nmake /f build.gmsGraphics.mak
#=============================================================================

GMS_GRAPHICS_SRC = $(GMS_ROOT)\C_source\source\gmsGraphics

#
# Skip 'gmsBrowse_PO_PolygonMapClass.cpp'.  It's a duplicate to
# 'gms_PO_PolygonMapClass.cpp'.  I don't know what I was doing that
# caused this duplication.
#
SRC = gmsBitmapClass.cpp                \
      gmsBrowseMapClass.cpp             \
      gmsDataTablesClass.cpp            \
      gmsDcwUtilities.cpp               \
      gmsLatLongGridClass.cpp           \
      gmsLibRefClass.cpp                \
      gmsMapClass.cpp                   \
      gmsMapStateMgr.cpp                \
      gmsMbrClass.cpp                   \
      gmsNamesOfFilesClass.cpp          \
      gmsNodeClass.cpp                  \
      gmsPolygonUtilities.cpp           \
      gmsScreenCoordUtils.cpp           \
      gmsTextClass.cpp                  \
      gmsTileClass.cpp                  \
      gmsTileRefClass.cpp               \
      gmsTransformationUtils.cpp        \
      gmsWorldCoordUtils.cpp            \
      gms_DN_PolygonMapClass.cpp        \
      gms_PO_PolygonMapClass.cpp

OBJ = $(SRC:.cpp=.obj)

GMS_LIB_PATH = .\lib

GMS_GRAPHICS_LIB = $(GMS_LIB_PATH)\libGmsGraphics.lib

CC = $(VS_BIN_DIR)\cl.exe

#
# Trying to be UNIX equivalent clever ...
#
AR = $(VS_BIN_DIR)\LIB.exe

# Define the compiler flags
# /Fo specifies the object file name

GMS_INCL = $(GMS_ROOT)\C_source\include

COPTS = $(INCL) \
       /I$(GMS_INCL)\gmsAdrgFile  \
       /I$(GMS_INCL)\gmsFile      \
       /I$(GMS_INCL)\gmsGraphics  \
       /D WIN32 /D _DEBUG /D _LIB \
       /D _MBCS \
       /EHsc

#
# Avoid warning LNK4098:
#       defaultlib 'LIBCMT' conflicts with use of other libs
#
LINK_OPTS = /NODEFAULTLIB:LIBCMT

all: $(GMS_GRAPHICS_LIB)

$(GMS_GRAPHICS_LIB): $(OBJ)
	@echo ^-^-^-^> Archive to $(GMS_GRAPHICS_LIB)
	$(AR) $(OBJ) $(LIBS) /OUT:$(GMS_GRAPHICS_LIB) $(LINK_OPTS)

gmsBitmapClass.obj:$(GMS_GRAPHICS_SRC)\gmsBitmapClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsBrowseMapClass.obj:$(GMS_GRAPHICS_SRC)\gmsBrowseMapClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsDataTablesClass.obj:$(GMS_GRAPHICS_SRC)\gmsDataTablesClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsDcwUtilities.obj:$(GMS_GRAPHICS_SRC)\gmsDcwUtilities.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsLatLongGridClass.obj:$(GMS_GRAPHICS_SRC)\gmsLatLongGridClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsLibRefClass.obj:$(GMS_GRAPHICS_SRC)\gmsLibRefClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsMapClass.obj:$(GMS_GRAPHICS_SRC)\gmsMapClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsMapStateMgr.obj:$(GMS_GRAPHICS_SRC)\gmsMapStateMgr.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsMbrClass.obj:$(GMS_GRAPHICS_SRC)\gmsMbrClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsNamesOfFilesClass.obj:$(GMS_GRAPHICS_SRC)\gmsNamesOfFilesClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsNodeClass.obj:$(GMS_GRAPHICS_SRC)\gmsNodeClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsPolygonUtilities.obj:$(GMS_GRAPHICS_SRC)\gmsPolygonUtilities.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsScreenCoordUtils.obj:$(GMS_GRAPHICS_SRC)\gmsScreenCoordUtils.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsTextClass.obj:$(GMS_GRAPHICS_SRC)\gmsTextClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsTileClass.obj:$(GMS_GRAPHICS_SRC)\gmsTileClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsTileRefClass.obj:$(GMS_GRAPHICS_SRC)\gmsTileRefClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsTransformationUtils.obj:$(GMS_GRAPHICS_SRC)\gmsTransformationUtils.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsWorldCoordUtils.obj:$(GMS_GRAPHICS_SRC)\gmsWorldCoordUtils.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gms_DN_PolygonMapClass.obj:$(GMS_GRAPHICS_SRC)\gms_DN_PolygonMapClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gms_PO_PolygonMapClass.obj:$(GMS_GRAPHICS_SRC)\gms_PO_PolygonMapClass.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

# Clean target: remove intermediate and output files
clean:
	@echo ^-^-^-^> Clean
	@del /f /q $(OBJ)
	@if exist $(GMS_GRAPHICS_LIB) del /f /q $(GMS_GRAPHICS_LIB)
