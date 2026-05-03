#===========================================================================
# File: Makefile
# Date : 08-MAR-2026 : initial definition (Linux build)
# Author      : T. MacAndrew
#
# Description :
#    Make file to to build the test drivers in:
#                  gmsFile
#===========================================================================

ROOT = ../../..

H_PATH = -I$(ROOT)/C_source/include/gmsFile

L_PATH = -L$(ROOT)/unixBuild/lib

L_LIBS = -lGmsFile -lm

OPTS = -ggdb -Wall -DIS_UNIX

SRC_DIR = ../gmsFile

TESTS = ae_test.cpp              \
        browse_dv.cpp            \
        browse_feature_test.cpp  \
        browse_main.cpp          \
        co_test.cpp              \
        edge_test_2.cpp          \
        edge_test_main.cpp       \
        face_test.cpp            \
        feature_test.cpp         \
        file_name.cpp            \
        lc_test.cpp              \
        mbr_test.cpp             \
        narrative_test.cpp       \
        node_test.cpp            \
        po_test.cpp              \
        ring_test.cpp            \
        schema_class_test.cpp    \
        text_test.cpp            \
        thematic_test.cpp        \
        tile_ref.cpp             \
        util_test.cpp            \
        vdt_test.cpp

EXES = $(TESTS:.cpp=.exe)

all:$(EXES)
	@echo "---> Build all"

ae_test.exe:$(SRC_DIR)/ae_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

browse_dv.exe:$(SRC_DIR)/browse_dv.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

browse_feature_test.exe:$(SRC_DIR)/browse_feature_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

browse_main.exe:$(SRC_DIR)/browse_main.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

co_test.exe:$(SRC_DIR)/co_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

edge_test_2.exe:$(SRC_DIR)/edge_test_2.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

edge_test_main.exe:$(SRC_DIR)/edge_test_main.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

face_test.exe:$(SRC_DIR)/face_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

feature_test.exe:$(SRC_DIR)/feature_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

file_name.exe:$(SRC_DIR)/file_name.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

lc_test.exe:$(SRC_DIR)/lc_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

mbr_test.exe:$(SRC_DIR)/mbr_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

narrative_test.exe:$(SRC_DIR)/narrative_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

node_test.exe:$(SRC_DIR)/node_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

po_test.exe:$(SRC_DIR)/po_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

ring_test.exe:$(SRC_DIR)/ring_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

schema_class_test.exe:$(SRC_DIR)/schema_class_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

text_test.exe:$(SRC_DIR)/text_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

thematic_test.exe:$(SRC_DIR)/thematic_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

tile_ref.exe:$(SRC_DIR)/tile_ref.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

util_test.exe:$(SRC_DIR)/util_test.cpp
	@echo "_______________"
	@echo "---> Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

vdt_test.exe:$(SRC_DIR)/vdt_test.cpp
	@echo "_______________"
	@echo "Build $@"
	@gcc $(OPTS) $(H_PATH) $(L_PATH) $? -o $@ $(L_LIBS)

clean:
	@echo "---> Clean - delete all executables"
	rm *.exe

