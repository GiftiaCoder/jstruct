#!/bin/bash

git submodule update --init

ROOTDIR=`pwd`

# make tests
cd ${ROOTDIR} && rm -rf build && mkdir build && cd build && cmake .. && make -sj ${nproc} && make test
