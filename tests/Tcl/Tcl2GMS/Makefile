#=============================================================================
# FILE: Makefile
#
# DESCRIPTION:
#    Build tcl2gms.c into archive to be called by Tcl GMS scripts.
#=============================================================================

CC  = gcc

OPTS = -Wall -DUSE_TCL_STUBS -fPIC

SRC = tcl2gms.c

OBJ = $(SRC:.c=.o)

INCL = -I/usr/include/tcl8.6                \
       -I../../C_source/include/gmsAdrgFile \
       -I../../C_source/include/gmsFile     \
       -I../../C_source/include/gmsGraphics

LIB_PATH = -L../../unixBuild/lib \
           -L/usr/lib/x86_64-linux-gnu

LIBS = -lGmsFile     \
       -lGmsGraph    \
       -lGmsAdrgFile \
       -ltclstub8.6

TO_LIB = libtcltogms.so

$(TO_LIB):$(OBJ)
	@echo "________________________________"
	@echo "Build $@"
	@gcc -shared -o $(TO_LIB) $(OBJ) $(LIB_PATH) $(LIBS)

$(OBJ):$(SRC)
	@echo "________________________________"
	@echo "Compile $?"
	$(CC) $(OPTS) $(INCL) -c $?

clean:
	@echo "________________________________"
	@echo "clean"
	rm -f $(OBJ)
	rm -f $(TO_LIB)
