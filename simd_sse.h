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

#ifndef DIMSUM_SIMD_SSE_H_
#define DIMSUM_SIMD_SSE_H_

#ifdef __SSE3__
# include <pmmintrin.h>
#endif
#ifdef __SSSE3__
# include <tmmintrin.h>
#endif
#ifdef __SSE4_1__
# include <smmintrin.h>
#endif

#include "operations.h"

namespace dimsum {

#ifdef __SSSE3__
template <>
inline Simd128<int8> abs(Simd128<int8> simd) {
  return _mm_abs_epi8(to_raw(simd));
}

template <>
inline Simd128<int16> abs(Simd128<int16> simd) {
  static_assert(simd.size() == 8, "");
  return _mm_abs_epi16(to_raw(simd));
}

template <>
inline Simd128<int32> abs(Simd128<int32> simd) {
  return _mm_abs_epi32(to_raw(simd));
}
#endif

#ifdef __AVX512VL__
template <>
inline Simd128<int64> abs(Simd128<int64> simd) {
  return _mm_abs_epi64(to_raw(simd));
}
#endif

// ::abs for floating points is implemented by bit_and each lane with shr(-1, 1)
// to clear the sign bit (the most significant bit).
template <>
inline Simd128<float> abs(Simd128<float> simd) {
  return bit_cast<float>(bit_cast<uint32>(simd) & Simd128<uint32>(~(1u << 31)));
}

template <>
inline Simd128<double> abs(Simd128<double> simd) {
  return bit_cast<double>(bit_cast<uint64>(simd) &
                          Simd128<uint64>(~(1ull << 63)));
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return _mm_rcp_ps(to_raw(simd));
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return _mm_sqrt_ps(to_raw(simd));
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return _mm_sqrt_pd(to_raw(simd));
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
  return _mm_rsqrt_ps(to_raw(simd));
}

template <>
inline Simd128<int8> add_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return _mm_adds_epi8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint8> add_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return _mm_adds_epu8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int16> add_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return _mm_adds_epi16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint16> add_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return _mm_adds_epu16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int8> sub_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return _mm_subs_epi8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint8> sub_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return _mm_subs_epu8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int16> sub_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return _mm_subs_epi16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint16> sub_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return _mm_subs_epu16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint64> reduce_add<uint64, 2>(Simd128<uint8> simd) {
  return _mm_sad_epu8(to_raw(simd), to_raw(Simd128<uint8>(0)));
}

template <>
inline Simd128<int32> reduce_add<int32, 4>(Simd128<int16> simd) {
  return _mm_madd_epi16(to_raw(simd), to_raw(Simd128<int16>(1)));
}

template <>
inline ResizeTo<Simd128<uint64>, 1> reduce_add<uint64, 1>(Simd128<uint8> simd) {
  return reduce_add<uint64, 1>(reduce_add<uint64, 2>(simd));
}

template <>
inline ResizeTo<Simd128<uint64>, 1> reduce_add<uint64, 1>(
    Simd128<uint16> simd) {
  uint64 even_sum =
      reduce_add<uint64, 1>(bit_cast<uint8>(simd & Simd128<uint16>(0x00ff)))[0];
  uint64 odd_sum =
      reduce_add<uint64, 1>(bit_cast<uint8>(simd & Simd128<uint16>(0xff00)))[0];
  return (odd_sum << 8) + even_sum;
}

template <>
inline Simd128<int32> mul_sum<int32>(Simd128<int16> lhs, Simd128<int16> rhs,
                                     Simd128<int32> acc) {
  return _mm_add_epi32(to_raw(acc), _mm_madd_epi16(to_raw(lhs), to_raw(rhs)));
}

#ifdef __SSE4_1__
// _MM_FROUND_TO_NEAREST_INT specifies round-to-even.
template <>
inline Simd128<float> round(Simd128<float> simd) {
  return _mm_round_ps(to_raw(simd),
                      _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return _mm_round_pd(to_raw(simd),
                      _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}
#endif

template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return _mm_cvtps_epi32(to_raw(simd));
}

template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#endif  // DIMSUM_SIMD_SSE_H_
