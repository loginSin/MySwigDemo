#!/usr/bin/env bash

rm -rf imlibV2/src/main/java/io/rong/imlib/swig/*

python c-extract-structs.py

swig -c++ -java -package io.rong.imlib.internal.swig \
          -outdir imlibV2/src/main/java/io/rong/imlib/internal/swig \
          -IimlibV2/src/main/cpp/libs/includes \
          -o imlibV2/src/main/cpp/RcClient_wrap.cpp \
          imlibV2/src/main/cpp/RcClient.i