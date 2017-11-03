/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DIMSUM_DIMSUM_X86_H_
#define DIMSUM_DIMSUM_X86_H_

#include "dimsum.h"
#include "simulated.h"

// dimsum::x86 provides x86 intrinsics which are not covered by dimsum::* free
// functions. There are two purposes:
// 1. providing non-x86 simulation so they build and have a not-too-bad
// performance on non-x86.
// 2. improving performance on x86 by utilizing x86 intrinsics which are not
// covered by dimsum::* functions. dimsum::x86 aims to provide an ergonomic
// interface.

// dimsum::x86::* functions are overloaded and their names are derived by
// stemming x86 intrinsic function names (e.g. _mm{,256}_movemask_*
// => movemask). Operations on 128-bit SIMD types are also provided for ARM and
// Power, which are simulated by ARM and Power intrinsics, or
// if intrinsics are unavailable, dimsum::simulated::* for-loop implementation.
// Because of simulation, the performance on ARM and Power is inferior, so uses
// of portable SIMD functions (dimsum::*) are encouraged.

#ifdef __SSE4_1__
# include <smmintrin.h>
# ifdef __AVX2__
#  include <immintrin.h>
# endif
#endif

#ifdef __VSX__
# include <altivec.h>
#endif

namespace dimsum {

namespace x86 {

template <typename Abi>
Simd<int16, Abi> maddubs(Simd<uint8, Abi> lhs, Simd<int8, Abi> rhs) {
  return simulated::maddubs(lhs, rhs);
}

#ifndef DIMSUM_USE_SIMULATED
#ifdef __SSE4_1__
template <>
inline Simd<int16, detail::XMM> maddubs(Simd<uint8, detail::XMM> lhs,
                                        Simd<int8, detail::XMM> rhs) {
  return _mm_maddubs_epi16(lhs, rhs);
}
#ifdef __AVX2__
template <>
inline Simd<int16, detail::YMM> maddubs(Simd<uint8, detail::YMM> lhs,
                                        Simd<int8, detail::YMM> rhs) {
  return _mm256_maddubs_epi16(lhs, rhs);
}
#endif  // __AVX2__
#endif  // __SSE4_1__
#endif  // DIMSUM_USE_SIMULATED

// TODO(maskray) {int,uint}{8,32,64} specializations are provided.
// Provide float/double specializations if there are use cases to check sign
// bits
template <typename T, typename Abi>
int movemask(Simd<T, Abi> simd) {
  return simulated::movemask(simd);
}

#ifndef DIMSUM_USE_SIMULATED
#ifdef __SSE4_1__

template <>
inline int movemask<int8, detail::XMM>(Simd<int8, detail::XMM> simd) {
  return _mm_movemask_epi8(simd.raw());
}

template <>
inline int movemask<uint8, detail::XMM>(Simd<uint8, detail::XMM> simd) {
  return _mm_movemask_epi8(simd.raw());
}

template <>
inline int movemask<int32, detail::XMM>(Simd<int32, detail::XMM> simd) {
  return _mm_movemask_ps(bit_cast<float>(simd).raw());
}

template <>
inline int movemask<uint32, detail::XMM>(Simd<uint32, detail::XMM> simd) {
  return _mm_movemask_ps(bit_cast<float>(simd).raw());
}

template <>
inline int movemask<int64, detail::XMM>(Simd<int64, detail::XMM> simd) {
  return _mm_movemask_pd(bit_cast<double>(simd).raw());
}

template <>
inline int movemask<uint64, detail::XMM>(Simd<uint64, detail::XMM> simd) {
  return _mm_movemask_pd(bit_cast<double>(simd).raw());
}

# ifdef __AVX2__
template <>
inline int movemask<int8, detail::YMM>(Simd<int8, detail::YMM> simd) {
  return _mm256_movemask_epi8(simd);
}

template <>
inline int movemask<uint8, detail::YMM>(Simd<uint8, detail::YMM> simd) {
  return _mm256_movemask_epi8(simd.raw());
}

template <>
inline int movemask<int32, detail::YMM>(Simd<int32, detail::YMM> simd) {
  return _mm256_movemask_ps(bit_cast<float>(simd).raw());
}

template <>
inline int movemask<uint32, detail::YMM>(Simd<uint32, detail::YMM> simd) {
  return _mm256_movemask_ps(bit_cast<float>(simd).raw());
}

template <>
inline int movemask<int64, detail::YMM>(Simd<int64, detail::YMM> simd) {
  return _mm256_movemask_pd(bit_cast<double>(simd).raw());
}

template <>
inline int movemask<uint64, detail::YMM>(Simd<uint64, detail::YMM> simd) {
  return _mm256_movemask_pd(bit_cast<double>(simd).raw());
}
#endif  // __AVX2__
#endif  // __SSE4_1__

#if defined(__VSX__)
template <>
inline int movemask<uint8, detail::VSX>(Simd<uint8, detail::VSX> simd) {
  __vector unsigned char mask;
  // vec_lvsl (little-endian version is deprecated on clang and GCC) inserts a
  // vec_perm on little-endian that we don't want.
  // On little-endian platforms, the lvsl instruction is equivalent to:
  // mask = {15, 14, 13, ..., 1, 0};
#if defined(__clang__)
  mask = __builtin_altivec_lvsl(0, (const unsigned char*)0);
#elif defined(__GNUC__)
  // The const unsigned char* overload of __builtin_altivec_lvsl is not defined
  // on GCC and other overloads have different behavior from clang (may be a
  // bug).
  asm("lvsl %0, %1, %1" : "=v"(mask) : "r"(0));
#else
# error Unsupported compiler
#endif

  // We then left shift mask by 3,
  // mask << 3 => {0x78, 0x70, 0x68, ..., 8, 0}
  // And call vec_vbpermq to pick the most significant bit of each byte (0th,
  // 8th, 16th, ... bits of simd).
  // lvsl and vbpermq neutralize big-endian effect of each other, and the
  // net effect applies to both big-endian and little-endian.
  // NOLINTNEXTLINE(runtime/int)
  __vector unsigned long long result =
      vec_vbpermq(static_cast<__vector unsigned char>(simd), mask << 3);

  static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
                "Only PowerPC little endian is supported");
  return result[1];
}

template <>
inline int movemask<int8, detail::VSX>(Simd<int8, detail::VSX> simd) {
  return movemask<uint8, detail::VSX>(bit_cast<uint8>(simd));
}
#endif  // __VSX__
#endif  // DIMSUM_USE_SIMULATED

}  // namespace x86

}  // namespace dimsum

#endif  // DIMSUM_DIMSUM_X86_H_
