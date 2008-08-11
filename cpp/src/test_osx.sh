#!/bin/bash

SRCS=grids

g++ $SRCS/protocol.cpp gridstest.cpp  $SRCS/event.cpp  -o gridstest -I $SRCS -lpthread -I ../include ../lib/libjsoncpp-OSX_i686.a  && ./gridstest happiland.net
