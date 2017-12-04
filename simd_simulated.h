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

#ifndef DIMSUM_SIMD_SIMULATED_H_
#define DIMSUM_SIMD_SIMULATED_H_

#include "operations.h"
#include "simulated.h"

namespace dimsum {

template <typename T>
using NativeSimd =
    Simd<T, detail::Abi<detail::StoragePolicy::kSimulated, 16 / sizeof(T)>>;

template <typename T>
using Simd128 =
    Simd<T, detail::Abi<detail::StoragePolicy::kSimulated, 16 / sizeof(T)>>;

template <typename T>
using Simd64 =
    Simd<T, detail::Abi<detail::StoragePolicy::kSimulated, 8 / sizeof(T)>>;

template <>
inline Simd128<int8> abs(Simd128<int8> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<int16> abs(Simd128<int16> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<int32> abs(Simd128<int32> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<int64> abs(Simd128<int64> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<float> abs(Simd128<float> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<double> abs(Simd128<double> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return simulated::reciprocal_estimate(simd);
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return simulated::sqrt(simd);
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return simulated::sqrt(simd);
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
  return simulated::reciprocal_sqrt_estimate(simd);
}

template <>
inline Simd128<int8> add_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd128<uint8> add_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd128<int16> add_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd128<uint16> add_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd128<int8> sub_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd128<uint8> sub_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd128<int16> sub_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd128<uint16> sub_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd128<int8> min(Simd128<int8> lhs, Simd128<int8> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<int16> min(Simd128<int16> lhs, Simd128<int16> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<int32> min(Simd128<int32> lhs, Simd128<int32> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<int64> min(Simd128<int64> lhs, Simd128<int64> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<uint8> min(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<uint16> min(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<uint32> min(Simd128<uint32> lhs, Simd128<uint32> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<float> min(Simd128<float> lhs, Simd128<float> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<double> min(Simd128<double> lhs, Simd128<double> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd128<int8> max(Simd128<int8> lhs, Simd128<int8> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<int16> max(Simd128<int16> lhs, Simd128<int16> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<int32> max(Simd128<int32> lhs, Simd128<int32> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<int64> max(Simd128<int64> lhs, Simd128<int64> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<uint8> max(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<uint16> max(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<uint32> max(Simd128<uint32> lhs, Simd128<uint32> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<uint64> max(Simd128<uint64> lhs, Simd128<uint64> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<float> max(Simd128<float> lhs, Simd128<float> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<double> max(Simd128<double> lhs, Simd128<double> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd128<float> round(Simd128<float> simd) {
  return simulated::round(simd);
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return simulated::round(simd);
}

template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return simulated::round_to_integer<int32>(simd);
}

// TODO(maskray) this specialization is duplicated in each simd_*.h file,
// which should be addressed later. The implementation of dimsum::split should
// also be revisited, it does not work on int8/uint8 Simd types.
template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

template <>
inline Simd128<float> fma(Simd128<float> a, Simd128<float> b,
                          Simd128<float> c) {
  return simulated::fma(a, b, c);
}

template <>
inline Simd128<double> fma(Simd128<double> a, Simd128<double> b,
                           Simd128<double> c) {
  return simulated::fma(a, b, c);
}

}  // namespace dimsum

#endif  // DIMSUM_SIMD_SIMULATED_H_
