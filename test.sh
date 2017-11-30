#!/bin/bash -xe

for ARCH in "--cpu=k8" "--copt=-mavx2" "--copt=-DDIMSUM_USE_SIMULATED"; do
  for COMPILATION_MODE in "--compilation_mode=fastbuild" "--compilation_mode=opt" "--copt=-fsanitize=address --linkopt=-fsanitize=address"; do
    CC=clang bazel test "$ARCH" $COMPILATION_MODE ...
    CC=gcc bazel test "$ARCH" $COMPILATION_MODE --copt='-Wno-return-type' --copt='-Wno-sign-compare' ...
  done
done
