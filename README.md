This is not an official Google product.

## Background

Dimsum is a portable C++ [SIMD](https://en.wikipedia.org/wiki/SIMD) library,
that is heavily influenced by the C++ standard library proposal
[P0214](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0214r6.pdf).
Currently, the library does not implement P0214, but its ultimate state is a
standard conforming implementation.

During the implementation, we examined all the differences our our intended
interfaces and P0214, and provided a feedback proposal
[P0820](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0820r0.pdf).

Due to prioritization, Dimsum does not currently implement the following list
of features from P0214, including but not limited to:
* simd\_mask<>
* simd\_abi::fixed\_size<>
* simd\_abi::compatible<>
* Non-power-of-two sizes
* SIMD version of \<cmath\>

Dimsum provides an extra set of opreations, mostly as free functions
in the namespace "dimsum" and namespace "dimsum::x86". The extra operations in
"dimsum" include many "horizontal" operations like shuffle, zip, and reduce-add;
the extra operations in "x86" provides some x86-specific semantics, with native
implementations on x86 and emulations on other architectures.

## Documentation

Refer to standard proposals for designs and public interfaces.

For the implementation, all the public interfaces should be documented in comments.

## Status

The above mentioned proposals are not part of the standard yet, nor is this
implementation fully conforming to the proposals. Any part of the API is subject
to change, without providing a fixing tool.

Some functionalities are with strawman implementations, with non-optimal
performance, due to prioritization.

## Requirements

Build system:
* Bazel

Compiler:
* Clang 3.8 or newer

The following sub-architectures are planed to be supported:
* x86 SSE 4.1 or newer
* x86 AVX2
* Power v2.07 VSX

We are also interested in supporting the following toolchain and architectures
in the future:
* (WIP) GCC 4.9 or newer
* (WIP) ARMv8 NEON (64-bit)

## Build and Test

The project is header-only, therefore no build is needed.

For testing, use "CC=clang bazel test --copt='...' <>" to run the unit tests.

For example:
* CC=clang bazel test --copt='-msse4.1' ... # x86 host
* CC=clang bazel test --copt='-mavx2' ... # x86 host
* CC=clang bazel test --copt='-maltivec' ... # Power host

Cross compilation is a bit tricky, but we found the following working, as long as
the toolchains support cross compilation:
* CC=clang bazel build --copt='-target' --copt='powerpc64le-linux-gnu'
      --copt='-maltivec' --linkopt='-target' --linkopt='powerpc64le-linux-gnu'
      ... # x86 host

## Fuzzing

Link dimsum\_fuzz against fuzz engines like libFuzzer, then run the result
binary.

(TODO) Add it to [OSS-Fuzz](https://github.com/google/oss-fuzz)
