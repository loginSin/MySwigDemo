rm -rf imlibV2/src/main/java/io/rong/imlib/swig/*

swig -c++ -java -package io.rong.imlib.swig -outdir imlibV2/src/main/java/io/rong/imlib/swig -IimlibV2/src/main/cpp/libs/includes -o imlibV2/src/main/cpp/swig2/rcim_client_wrap.cxx imlibV2/src/main/cpp/swig2/rcim_client.i