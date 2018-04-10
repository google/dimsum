#!/bin/bash -xe

for ARCH in "--copt=-msse4.1" "--copt=-mavx2" "--copt=-DDIMSUM_USE_SIMULATED"; do
  for COMPILATION_MODE in "--compilation_mode=fastbuild" "--compilation_mode=opt" "--copt=-fsanitize=address --linkopt=-fsanitize=address"; do
    CC=clang bazel test "$ARCH" $COMPILATION_MODE ...

    # __sanitizer_annotate_contiguous_container in simd does not compile on gcc. Need to fix it in libcxx.
    #CC=gcc bazel test "$ARCH" $COMPILATION_MODE --copt='-Wno-return-type' --copt='-Wno-sign-compare' ...
  done
done
