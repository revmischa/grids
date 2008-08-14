#!/bin/bash

SRCS=grids

g++ $SRCS/*.cpp  -o grids_full -I $SRCS -lSDL -lSDL_net -lGL -L../lib kaleidoscope/*  -I ../include ../lib/libjson_linux-gcc-4.1.2_libmt.a  && ./grids_full