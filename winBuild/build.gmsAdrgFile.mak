#=============================================================================
# FILE: build.gmsAdrgFile.mak
#
# DESCRIPTION:
#    Visual Studio NMAKE file for building libGmsAdrgFile.lib
#
#    Use the 'build.bat' script to invoke.  It sets the Visual Studio
#    environment needed to build.
#
# RUN:
#   nmake /f build.gmsAdrgFile.mak
#=============================================================================

GMS_ADRG_SRC = $(GMS_ROOT)\C_source\source\gmsAdrgFile

SRC = gmsAdrgTestPatchImage.cpp \
      gmsAdrgUtilities.cpp

OBJ = $(SRC:.cpp=.obj)

GMS_LIB_PATH = .\lib

GMS_ADRG_LIB = $(GMS_LIB_PATH)\libGmsAdrgFile.lib

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
       /I$(GMS_INCL)\gmsAdrgFile  \
       /D WIN32 /D _DEBUG /D _LIB \
       /D _MBCS \
       /EHsc

#
# Avoid warning LNK4098:
#       defaultlib 'LIBCMT' conflicts with use of other libs
#
LINK_OPTS = /NODEFAULTLIB:LIBCMT

all: $(GMS_ADRG_LIB)

$(GMS_ADRG_LIB): $(OBJ)
	@echo ^-^-^-^> Archive to $(GMS_ADRG_LIB)
	$(AR) $(OBJ) $(LIBS) /OUT:$(GMS_ADRG_LIB) $(LINK_OPTS)

gmsAdrgTestPatchImage.obj:$(GMS_ADRG_SRC)\gmsAdrgTestPatchImage.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsAdrgUtilities.obj:$(GMS_ADRG_SRC)\gmsAdrgUtilities.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

# Clean target: remove intermediate and output files
clean:
	@echo ^-^-^-^> Clean
	@del /f /q $(OBJ)
	@if exist $(GMS_ADRG_LIB) del /f /q $(GMS_ADRG_LIB)

