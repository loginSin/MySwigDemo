rm -rf imlibV2/src/main/java/io/rong/imlib/swig/*

python c-extract-structs.py

swig -c++ -java -package io.rong.imlib.swig -outdir imlibV2/src/main/java/io/rong/imlib/swig -IimlibV2/src/main/cpp/libs/includes -o imlibV2/src/main/cpp/rc_adapter_wrap.cpp imlibV2/src/main/cpp/rc_adapter.i