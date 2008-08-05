#!/bin/bash

g++ GridsProtocol.cpp gridstest.cpp -o gridstest -I../external/jsoncpp/include ../external/jsoncpp/libs/linux-gcc-4.1.2/libjson_linux-gcc-4.1.2_libmt.a && ./gridstest localhost
