#=============================================================================
# FILE: build.mak
#
# DESCRIPTION:
#    Visual Studio NMAKE file for building the "file" file_name.cpp
#    test.
#
#    Use the 'build.bat' script to invoke.  It sets the Visual Studio
#    environment needed to build.
#
# RUN:
#   nmake /f build.mak
#=============================================================================

TEST_NAME = file_name.cpp

SRC = $(GMS_ROOT)\tests\mains\gmsFile\$(TEST_NAME)

OBJ = $(TEST_NAME:.cpp=.obj)

EXE = $(TEST_NAME:.cpp=.exe)

CC = $(VS_BIN_DIR)\cl.exe

# Define the compiler flags
# /Fo specifies the object file name

GMS_INCL = $(GMS_ROOT)\C_source\include

TEST_INCL = $(GMS_ROOT)\tests\mains\win_builds\RESOURCE

COPTS = $(INCL) \
       /I$(GMS_INCL)\gmsAdrgFile  \
       /I$(GMS_INCL)\gmsFile      \
       /I$(GMS_INCL)\gmsGraphics  \
       /I$(TEST_INCL)             \
       /D WIN32 /D _DEBUG /D _CONSOLE \
       /D _MBCS \
       /EHsc    \
       /Fo$(OBJ)

#
# C link libs
#
BASE_LIBS = kernel32.lib user32.lib   gdi32.lib   winspool.lib \
            comdlg32.lib advapi32.lib shell32.lib ole32.lib    \
            oleaut32.lib uuid.lib     odbc32.lib  odbccp32.lib

#
# C++ link libs
#    ucrtd.lib
#    vcruntimed.lib
#
CPP_LIBS = msvcrtd.lib

#
# GMS L-path
#
GMS_LPATH = /LIBPATH:$(GMS_ROOT)\winBuild\lib

#
# GMS link libs
#
GMS_LIBS = libGmsFile.lib      \
           libGmsGraphics.lib  \
           libGmsAdrgFile.lib

#
# Summarize
#
ALL_L_PATH = $(LPATH) $(GMS_LPATH)

ALL_LIBS = $(GMS_LIBS) $(BASE_LIBS) $(CPP_LIBS)

#
# Avoid warning LNK4098:
#       defaultlib 'LIBCMT' conflicts with use of other libs
#
LINK_OPTS = /NODEFAULTLIB:LIBCMT

all: $(EXE)

$(EXE): $(OBJ)
	@echo ^-^-^-^> Link $(EXE)
	$(CC) $(OBJ) /link $(ALL_L_PATH) $(ALL_LIBS) /OUT:$(EXE) $(LINK_OPTS)

$(OBJ): $(SRC)
	@echo ^-^-^-^> Compile $(SRC)
	$(CC) $(COPTS) /c $(SRC)

# Clean target: remove intermediate and output files
clean:
	@echo ^-^-^-^> Clean
	@if exist $(OBJ) del /f /q $(OBJ)
	@if exist $(EXE) del /f /q $(EXE)

