#!/bin/bash

SRCS=grids/

g++ grids/GridsProtocol.cpp gridstest.cpp  $SRCS/GridsEvent.cpp  -o gridstest -I $SRCS -lpthread -I ../include ../external/jsoncpp/libs/linux-gcc-4.1.2/libjson_linux-gcc-4.1.2_libmt.a  && ./gridstest localhost
