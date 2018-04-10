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

#ifndef DIMSUM_SIMD_VSX_H_
#define DIMSUM_SIMD_VSX_H_

#include <altivec.h>

#include "operations.h"

namespace dimsum {

template <typename T>
Simd128<T> abs(Simd128<T> simd) {
  return vec_abs(to_raw(simd));
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return vec_re(to_raw(simd));
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return vec_sqrt(to_raw(simd));
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return vec_sqrt(to_raw(simd));
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
  return vec_rsqrte(to_raw(simd));
}

template <typename T>
Simd128<T> add_saturated(Simd128<T> lhs, Simd128<T> rhs) {
  return vec_adds(to_raw(lhs), to_raw(rhs));
}

template <typename T>
Simd128<T> sub_saturated(Simd128<T> lhs, Simd128<T> rhs) {
  return vec_subs(to_raw(lhs), to_raw(rhs));
}

template <>
inline Simd128<int32> reduce_add<int32, 4>(Simd128<int8> simd) {
  return vec_sum4s(to_raw(simd), to_raw(Simd128<int32>(0)));
}

template <>
inline Simd128<uint32> reduce_add<uint32, 4>(Simd128<uint8> simd) {
  return vec_sum4s(to_raw(simd), to_raw(Simd128<uint32>(0)));
}

template <>
inline Simd128<int32> reduce_add<int32, 4>(Simd128<int16> simd) {
  return vec_sum4s(to_raw(simd), to_raw(Simd128<int32>(0)));
}

template <>
inline Simd128<uint32> reduce_add<uint32, 4>(Simd128<uint16> simd) {
  return vec_msum(to_raw(simd), to_raw(Simd128<uint16>(1)),
                  to_raw(Simd128<uint32>(0)));
}

// Common reduce_add<T, 1> specializations.

// vec_sums(vector int, vector int) (vsumsws) returns saturated sum.
// Note, reduce_add overflow has undefined behavior, so we are free to use this
// saturation semantics.
template <>
inline ResizeTo<Simd128<int32>, 1> reduce_add<int32, 1>(Simd128<int32> simd) {
  return vec_sums(to_raw(simd), to_raw(Simd128<int32>(0)))[3];
}

template <>
inline ResizeTo<Simd128<uint32>, 1> reduce_add<uint32, 1>(Simd128<uint8> simd) {
  return bit_cast<uint32>(
      reduce_add<int32, 1>(bit_cast<int32>(reduce_add<uint32, 4>(simd))));
}

template <>
inline ResizeTo<Simd128<uint64>, 1> reduce_add<uint64, 1>(Simd128<uint8> simd) {
  return reduce_add<uint32, 1>(simd)[0];
}

template <>
inline ResizeTo<Simd128<uint32>, 1> reduce_add<uint32, 1>(
    Simd128<uint16> simd) {
  return bit_cast<uint32>(
      reduce_add<int32, 1>(bit_cast<int32>(reduce_add<uint32, 4>(simd))));
}

template <>
inline ResizeTo<Simd128<uint64>, 1> reduce_add<uint64, 1>(
    Simd128<uint16> simd) {
  return reduce_add<uint32, 1>(simd)[0];
}

template <>
inline Simd128<uint32> mul_sum<uint32>(Simd128<uint8> lhs, Simd128<uint8> rhs,
                                       Simd128<uint32> acc) {
  return vec_msum(to_raw(lhs), to_raw(rhs), to_raw(acc));
}

template <>
inline Simd128<int32> mul_sum<int32>(Simd128<int16> lhs, Simd128<int16> rhs,
                                     Simd128<int32> acc) {
  return vec_msum(to_raw(lhs), to_raw(rhs), to_raw(acc));
}

template <>
inline Simd128<uint32> mul_sum<uint32>(Simd128<uint16> lhs, Simd128<uint16> rhs,
                                       Simd128<uint32> acc) {
  return vec_msum(to_raw(lhs), to_raw(rhs), to_raw(acc));
}

// vec_rint calls XVR[SD]PIC which round floating points according to the
// Rounding Control field RN in FPSCR, which defaults to round-to-even.
template <>
inline Simd128<float> round(Simd128<float> simd) {
  return vec_rint(to_raw(simd));
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return vec_rint(to_raw(simd));
}

// The alnerative implementation of float-to-int conversion on PPC. Note that
// there is no instruction to directly convert a f.p. vector to an integer
// vector. It is implememented by first rounding it to the nearest representable
// floating-point integers, and then truncating them to integers.
template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return vec_cts(vec_rint(to_raw(simd)), 0);
}

template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#endif  // DIMSUM_SIMD_VSX_H_
