#=============================================================================
# FILE: build.gmsFile.mak
#
# DESCRIPTION:
#    Visual Studio NMAKE file for building libGmsFile.lib
#
#    Use the 'build.bat' script to invoke.  It sets the Visual Studio
#    environment needed to build.
#
# RUN:
#   nmake /f build.mak
#=============================================================================

GMS_FILE_SRC = $(GMS_ROOT)\C_source\source\gmsFile

SRC = gmsBrowseFeatureTable.cpp      \
      gmsCoverageAttribTable.cpp     \
      gmsDataQualityTable.cpp        \
      gmsDbHeaderTable.cpp           \
      gmsDebugUtil.cpp               \
      gmsEdgeTable.cpp               \
      gmsFaceTable.cpp               \
      gmsFeatureClassSchemaTable.cpp \
      gmsFeatureTable.cpp            \
      gmsFileNameUtilities.cpp       \
      gmsGeoReferenceTable.cpp       \
      gmsIndexFile.cpp               \
      gmsJoinTable.cpp               \
      gmsLibAttribTable.cpp          \
      gmsLibHeaderTable.cpp          \
      gmsMinBoundRectTable.cpp       \
      gmsNarrativeTable.cpp          \
      gmsNodeTable.cpp               \
      gmsRingTable.cpp               \
      gmsSpatialIndex.cpp            \
      gmsTextTable.cpp               \
      gmsThematicIndex.cpp           \
      gmsUtilities.cpp               \
      gmsValueDescriptionTable.cpp

OBJ = $(SRC:.cpp=.obj)

GMS_LIB_PATH = $(GMS_ROOT)\lib

GMS_FILE_LIB = $(GMS_LIB_PATH)\libGmsFile.lib

CC = $(VS_BIN_DIR)\cl.exe

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

all: $(GMS_FILE_LIB)

$(GMS_FILE_LIB): $(OBJ)
	@echo ^-^-^-^> Archive to $(GMS_FILE_LIB)
	$(CC) $(OBJ) /link $(LIBS) /OUT:$(GMS_FILE_LIB) $(LINK_OPTS)

gmsBrowseFeatureTable.obj:$(GMS_FILE_SRC)\gmsBrowseFeatureTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsCoverageAttribTable.obj:$(GMS_FILE_SRC)\gmsCoverageAttribTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsDataQualityTable.obj:$(GMS_FILE_SRC)\gmsDataQualityTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsDbHeaderTable.obj:$(GMS_FILE_SRC)\gmsDbHeaderTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsDebugUtil.obj:$(GMS_FILE_SRC)\gmsDebugUtil.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsEdgeTable.obj:$(GMS_FILE_SRC)\gmsEdgeTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsFaceTable.obj:$(GMS_FILE_SRC)\gmsFaceTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsFeatureClassSchemaTable.obj:$(GMS_FILE_SRC)\gmsFeatureClassSchemaTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsFeatureTable.obj:$(GMS_FILE_SRC)\gmsFeatureTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsFileNameUtilities.obj:$(GMS_FILE_SRC)\gmsFileNameUtilities.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsGeoReferenceTable.obj:$(GMS_FILE_SRC)\gmsGeoReferenceTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsIndexFile.obj:$(GMS_FILE_SRC)\gmsIndexFile.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsJoinTable.obj:$(GMS_FILE_SRC)\gmsJoinTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsLibAttribTable.obj:$(GMS_FILE_SRC)\gmsLibAttribTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsLibHeaderTable.obj:$(GMS_FILE_SRC)\gmsLibHeaderTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsMinBoundRectTable.obj:$(GMS_FILE_SRC)\gmsMinBoundRectTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsNarrativeTable.obj:$(GMS_FILE_SRC)\gmsNarrativeTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsNodeTable.obj:$(GMS_FILE_SRC)\gmsNodeTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsRingTable.obj:$(GMS_FILE_SRC)\gmsRingTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsSpatialIndex.obj:$(GMS_FILE_SRC)\gmsSpatialIndex.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsTextTable.obj:$(GMS_FILE_SRC)\gmsTextTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsThematicIndex.obj:$(GMS_FILE_SRC)\gmsThematicIndex.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsUtilities.obj:$(GMS_FILE_SRC)\gmsUtilities.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

gmsValueDescriptionTable.obj:$(GMS_FILE_SRC)\gmsValueDescriptionTable.cpp
	@echo ^-^-^-^> Compile $(?)
	$(CC) $(COPTS) /Fo$(@) /c $(?)

# Clean target: remove intermediate and output files
clean:
	@echo ^-^-^-^> Clean
	@del /f /q $(OBJ)
	@if exist $(GMS_FILE_LIB) del /f /q $(GMS_FILE_LIB)

