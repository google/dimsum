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

#ifndef DIMSUM_SIMD_AVX_H_
#define DIMSUM_SIMD_AVX_H_

#ifdef __AVX__

#include <immintrin.h>

#include "operations.h"

namespace dimsum {

// Some instructions (align, pack, unpack, permutation types, e.g.
// _mm256_alignr_epi8 _mm256_packs_epi32) operate on 128-bit lanes rather the
// whole register, elements do not cross the 128-bit middle boundary.
// Extra shuffles are used to make them intuitional. Refer to Intel Intrinsics
// Guide for details.

template <>
inline Simd256<int8> abs(Simd256<int8> simd) {
  return _mm256_abs_epi8(to_raw(simd));
}

template <>
inline Simd256<int16> abs(Simd256<int16> simd) {
  return _mm256_abs_epi16(to_raw(simd));
}

template <>
inline Simd256<int32> abs(Simd256<int32> simd) {
  return _mm256_abs_epi32(to_raw(simd));
}

#ifdef __AVX512VL__
template <>
inline Simd256<int64> abs(Simd256<int64> simd) {
  return _mm256_abs_epi64(to_raw(simd));
}
#endif

// ::abs is implemented by bitand each lane with shr(-1, 1) to clear the
// sign bit (the most significant bit).
template <>
inline Simd256<float> abs(Simd256<float> simd) {
  return bit_cast<float>(bit_cast<uint32>(simd) & Simd256<uint32>(~(1u << 31)));
}

template <>
inline Simd256<double> abs(Simd256<double> simd) {
  return bit_cast<double>(bit_cast<uint64>(simd) &
                          Simd256<uint64>(~(1ull << 63)));
}

template <>
inline Simd256<float> reciprocal_estimate(Simd256<float> simd) {
  return _mm256_rcp_ps(to_raw(simd));
}

template <>
inline Simd256<float> sqrt(Simd256<float> simd) {
  return _mm256_sqrt_ps(to_raw(simd));
}

template <>
inline Simd256<double> sqrt(Simd256<double> simd) {
  return _mm256_sqrt_pd(to_raw(simd));
}

template <>
inline Simd256<float> reciprocal_sqrt_estimate(Simd256<float> simd) {
  return _mm256_rsqrt_ps(to_raw(simd));
}

template <>
inline Simd256<int8> add_saturated(Simd256<int8> lhs, Simd256<int8> rhs) {
  return _mm256_adds_epi8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<uint8> add_saturated(Simd256<uint8> lhs, Simd256<uint8> rhs) {
  return _mm256_adds_epu8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<int16> add_saturated(Simd256<int16> lhs, Simd256<int16> rhs) {
  return _mm256_adds_epi16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<uint16> add_saturated(Simd256<uint16> lhs, Simd256<uint16> rhs) {
  return _mm256_adds_epu16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<int8> sub_saturated(Simd256<int8> lhs, Simd256<int8> rhs) {
  return _mm256_subs_epi8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<uint8> sub_saturated(Simd256<uint8> lhs, Simd256<uint8> rhs) {
  return _mm256_subs_epu8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<int16> sub_saturated(Simd256<int16> lhs, Simd256<int16> rhs) {
  return _mm256_subs_epi16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<uint16> sub_saturated(Simd256<uint16> lhs, Simd256<uint16> rhs) {
  return _mm256_subs_epu16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd256<uint64> reduce_add<uint64, 4>(Simd256<uint8> simd) {
  return _mm256_sad_epu8(to_raw(simd), to_raw(Simd256<uint8>(0)));
}

template <>
inline Simd256<int32> reduce_add<int32, 8>(Simd256<int16> simd) {
  return _mm256_madd_epi16(to_raw(simd), to_raw(Simd256<int16>(1)));
}

template <>
inline ResizeTo<Simd256<uint64>, 1> reduce_add<uint64, 1>(Simd256<uint8> simd) {
  return reduce_add<uint64, 1>(reduce_add<uint64, 4>(simd));
}

template <>
inline ResizeTo<Simd256<uint64>, 1> reduce_add<uint64, 1>(
    Simd256<uint16> simd) {
  uint64 even_sum =
      reduce_add<uint64, 1>(bit_cast<uint8>(simd & Simd256<uint16>(0x00ff)))[0];
  uint64 odd_sum =
      reduce_add<uint64, 1>(bit_cast<uint8>(simd & Simd256<uint16>(0xff00)))[0];
  return (odd_sum << 8) + even_sum;
}

template <>
inline Simd256<int32> mul_sum<int32>(Simd256<int16> lhs, Simd256<int16> rhs,
                                     Simd256<int32> acc) {
  return _mm256_add_epi32(to_raw(acc),
                          _mm256_madd_epi16(to_raw(lhs), to_raw(rhs)));
}

// _MM_FROUND_TO_NEAREST_INT specifies round-to-even.
template <>
inline Simd256<float> round(Simd256<float> simd) {
  return _mm256_round_ps(to_raw(simd),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline Simd256<double> round(Simd256<double> simd) {
  return _mm256_round_pd(to_raw(simd),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline Simd256<int32> round_to_integer(Simd256<float> simd) {
  return _mm256_cvtps_epi32(to_raw(simd));
}

template <typename T>
Simd256<ScaleBy<T, 2>> mul_widened(Simd128<T> lhs, Simd128<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#endif  // __AVX__
#endif  // DIMSUM_SIMD_AVX_H_
