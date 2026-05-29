#!/bin/bash
#=============================================================================
# FILE: build.sh
#
# DESCRIPTION:
#    Steps to use 'qmake' tool to build 'DCW browse' as a Qt app.
#=============================================================================

qmake -o Makefile dcw_browse.pro

make clean

make

