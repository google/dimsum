#!/bin/bash -xe

for ARCH in "--cpu=k8" "--copt=-mavx2"; do
  for COMPILATION_MODE in "--compilation_mode=fastbuild" "--compilation_mode=opt" "--config=asan" "--copt=-DDIMSUM_USE_SIMULATED"; do
    CC=clang bazel test "$ARCH" "$COMPILATION_MODE" ...
  done
done
