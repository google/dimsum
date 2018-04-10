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

#ifndef DIMSUM_SIMD_NEON_H_
#define DIMSUM_SIMD_NEON_H_

#include <arm_neon.h>

#include "operations.h"

namespace dimsum {
namespace detail {

// llvm.experimental.vector.reduce.add.* already lowers to single instructions
// like addv, but I'm not sure if there are intrinsics for them.
}  // namespace detail

template <>
inline Simd128<int8> abs(Simd128<int8> simd) {
  return vabsq_s8(to_raw(simd));
}

template <>
inline Simd128<int16> abs(Simd128<int16> simd) {
  return vabsq_s16(to_raw(simd));
}

template <>
inline Simd128<int32> abs(Simd128<int32> simd) {
  return vabsq_s32(to_raw(simd));
}

template <>
inline Simd128<int64> abs(Simd128<int64> simd) {
  return vabsq_s64(to_raw(simd));
}

template <>
inline Simd128<float> abs(Simd128<float> simd) {
  return vabsq_f32(to_raw(simd));
}

template <>
inline Simd128<double> abs(Simd128<double> simd) {
  return vabsq_f64(to_raw(simd));
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return vrecpeq_f32(to_raw(simd));
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return vsqrtq_f32(to_raw(simd));
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return vsqrtq_f64(to_raw(simd));
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
  return vrsqrteq_f32(to_raw(simd));
}

template <>
inline Simd128<int8> add_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vqaddq_s8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint8> add_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vqaddq_u8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int16> add_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vqaddq_s16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint16> add_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vqaddq_u16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int8> sub_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vqsubq_s8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint8> sub_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vqsubq_u8(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int16> sub_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vqsubq_s16(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<uint16> sub_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vqsubq_u16(to_raw(lhs), to_raw(rhs));
}

// An alternative implementation of the SSE intrinsic function _mm_madd_epi16
// on ARM. It breaks a Simd object into the low and high parts. Then values in
// each part are multiplied and summed pairwisely before being concatenated.
template <>
inline Simd128<int32> mul_sum(Simd128<int16> lhs, Simd128<int16> rhs,
                              Simd128<int32> acc) {
  int16x8_t lhs_raw = to_raw(lhs);
  int16x8_t rhs_raw = to_raw(rhs);
  int32x4_t mullo = vmull_s16(vget_low_s16(lhs_raw), vget_low_s16(rhs_raw));
  int32x4_t mulhi = vmull_s16(vget_high_s16(lhs_raw), vget_high_s16(rhs_raw));
  int32x2_t addlo = vpadd_s32(vget_low_s32(mullo), vget_high_s32(mullo));
  int32x2_t addhi = vpadd_s32(vget_low_s32(mulhi), vget_high_s32(mulhi));
  return vaddq_s32(to_raw(acc), vcombine_s32(addlo, addhi));
}

// vrndnq_f{32,64} translate to VRINTN.F{16,32}, which round floating points
// using the round-to-even rule (Round to Nearest rounding mode in ARM
// parlance).
template <>
inline Simd128<float> round(Simd128<float> simd) {
  return vrndnq_f32(to_raw(simd));
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return vrndnq_f64(to_raw(simd));
}

template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return vcvtnq_s32_f32(to_raw(simd));
}

template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#endif  // DIMSUM_SIMD_NEON_H_
