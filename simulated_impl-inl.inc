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

#include "simulated.h"

namespace dimsum {

namespace simulated {

// TODO(maskray) the forward declarations are used here because of a circular
// dependency limitation. dimsum.h should be separated to dimsum_types.h and
// dimsum_specializations.h and only dimsum_types.h should be included by
// simulated.h .

template <typename T, typename Abi>
Simd<detail::Number<sizeof(T) / 2, detail::NumberKind::kSInt>, Abi>
pack_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

template <typename T, typename Abi>
Simd<detail::Number<sizeof(T) / 2, detail::NumberKind::kUInt>, Abi>
packu_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

template <typename T, typename Abi>
typename std::enable_if<std::is_integral<T>::value, Simd<T, Abi>>::type abs(
    Simd<T, Abi> simd);

template <typename T, typename Abi>
typename std::enable_if<std::is_floating_point<T>::value, Simd<T, Abi>>::type
abs(Simd<T, Abi> simd);

template <typename T, typename Abi>
Simd<T, Abi> reciprocal_estimate(Simd<T, Abi> simd);

template <typename T, typename Abi>
Simd<T, Abi> sqrt(Simd<T, Abi> simd);

template <typename T, typename Abi>
Simd<T, Abi> reciprocal_sqrt_estimate(Simd<T, Abi> simd);

template <typename T, typename Abi>
Simd<T, Abi> round(Simd<T, Abi> simd);

template <typename Dest, typename T, typename Abi>
Simd<Dest, Abi> round_to_integer(Simd<T, Abi> simd);

template <typename T, typename Abi>
Simd<T, Abi> add_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

template <typename T, typename Abi>
Simd<T, Abi> sub_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

template <typename T, typename Abi>
Simd<T, Abi> min(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

template <typename T, typename Abi>
Simd<T, Abi> max(Simd<T, Abi> lhs, Simd<T, Abi> rhs);

}  // namespace simulated

namespace detail {

// Simulated (16 bytes)
using Simulated = detail::Abi<detail::StoragePolicy::kSimulated, 16>;
using HalfSimulated = detail::Abi<detail::StoragePolicy::kSimulated, 8>;

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kSimulated, 16, InternalType)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kSimulated, 16, InternalType)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(
    detail::StoragePolicy::kSimulated);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kSimulated, 8);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kSimulated, 32);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kSimulated, 64);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kSimulated, 128);

}  // namespace detail

template <typename T>
using NativeSimd = Simd<T, detail::Simulated>;

template <typename T>
using Simd128 = Simd<T, detail::Simulated>;

template <typename T>
using Simd64 = Simd<T, detail::HalfSimulated>;

template <>
inline Simd<int8, detail::Simulated> abs(Simd<int8, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<int16, detail::Simulated> abs(Simd<int16, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<int32, detail::Simulated> abs(Simd<int32, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<float, detail::Simulated> abs(Simd<float, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<int64, detail::Simulated> abs(Simd<int64, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<double, detail::Simulated> abs(
    Simd<double, detail::Simulated> simd) {
  return simulated::abs(simd);
}

template <>
inline Simd<float, detail::Simulated> reciprocal_estimate(
    Simd<float, detail::Simulated> simd) {
  return simulated::reciprocal_estimate(simd);
}

template <>
inline Simd<float, detail::Simulated> sqrt(
    Simd<float, detail::Simulated> simd) {
  return simulated::sqrt<float, detail::Simulated>(simd);
}

template <>
inline Simd<double, detail::Simulated> sqrt(
    Simd<double, detail::Simulated> simd) {
  return simulated::sqrt(simd);
}

template <>
inline Simd<float, detail::Simulated> reciprocal_sqrt_estimate(
    Simd<float, detail::Simulated> simd) {
  return simulated::reciprocal_sqrt_estimate(simd);
}

template <>
inline Simd<int8, detail::Simulated> add_saturated(
    Simd<int8, detail::Simulated> lhs, Simd<int8, detail::Simulated> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd<uint8, detail::Simulated> add_saturated(
    Simd<uint8, detail::Simulated> lhs, Simd<uint8, detail::Simulated> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd<int16, detail::Simulated> add_saturated(
    Simd<int16, detail::Simulated> lhs, Simd<int16, detail::Simulated> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd<uint16, detail::Simulated> add_saturated(
    Simd<uint16, detail::Simulated> lhs, Simd<uint16, detail::Simulated> rhs) {
  return simulated::add_saturated(lhs, rhs);
}

template <>
inline Simd<int8, detail::Simulated> sub_saturated(
    Simd<int8, detail::Simulated> lhs, Simd<int8, detail::Simulated> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd<uint8, detail::Simulated> sub_saturated(
    Simd<uint8, detail::Simulated> lhs, Simd<uint8, detail::Simulated> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd<int16, detail::Simulated> sub_saturated(
    Simd<int16, detail::Simulated> lhs, Simd<int16, detail::Simulated> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd<uint16, detail::Simulated> sub_saturated(
    Simd<uint16, detail::Simulated> lhs, Simd<uint16, detail::Simulated> rhs) {
  return simulated::sub_saturated(lhs, rhs);
}

template <>
inline Simd<int8, detail::Simulated> min(Simd<int8, detail::Simulated> lhs,
                                         Simd<int8, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<int16, detail::Simulated> min(Simd<int16, detail::Simulated> lhs,
                                          Simd<int16, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<int32, detail::Simulated> min(Simd<int32, detail::Simulated> lhs,
                                          Simd<int32, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<int64, detail::Simulated> min(Simd<int64, detail::Simulated> lhs,
                                          Simd<int64, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<uint8, detail::Simulated> min(Simd<uint8, detail::Simulated> lhs,
                                          Simd<uint8, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<uint16, detail::Simulated> min(
    Simd<uint16, detail::Simulated> lhs, Simd<uint16, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<uint32, detail::Simulated> min(
    Simd<uint32, detail::Simulated> lhs, Simd<uint32, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<float, detail::Simulated> min(Simd<float, detail::Simulated> lhs,
                                          Simd<float, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<double, detail::Simulated> min(
    Simd<double, detail::Simulated> lhs, Simd<double, detail::Simulated> rhs) {
  return simulated::min(lhs, rhs);
}

template <>
inline Simd<int8, detail::Simulated> max(Simd<int8, detail::Simulated> lhs,
                                         Simd<int8, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<int16, detail::Simulated> max(Simd<int16, detail::Simulated> lhs,
                                          Simd<int16, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<int32, detail::Simulated> max(Simd<int32, detail::Simulated> lhs,
                                          Simd<int32, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<int64, detail::Simulated> max(Simd<int64, detail::Simulated> lhs,
                                          Simd<int64, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<uint8, detail::Simulated> max(Simd<uint8, detail::Simulated> lhs,
                                          Simd<uint8, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<uint16, detail::Simulated> max(
    Simd<uint16, detail::Simulated> lhs, Simd<uint16, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<uint32, detail::Simulated> max(
    Simd<uint32, detail::Simulated> lhs, Simd<uint32, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<uint64, detail::Simulated> max(
    Simd<uint64, detail::Simulated> lhs, Simd<uint64, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<float, detail::Simulated> max(Simd<float, detail::Simulated> lhs,
                                          Simd<float, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<double, detail::Simulated> max(
    Simd<double, detail::Simulated> lhs, Simd<double, detail::Simulated> rhs) {
  return simulated::max(lhs, rhs);
}

template <>
inline Simd<int8, detail::Simulated> pack_saturated(
    Simd<int16, detail::Simulated> lhs, Simd<int16, detail::Simulated> rhs) {
  return simulated::pack_saturated(lhs, rhs);
}

template <>
inline Simd<int16, detail::Simulated> pack_saturated(
    Simd<int32, detail::Simulated> lhs, Simd<int32, detail::Simulated> rhs) {
  return simulated::pack_saturated(lhs, rhs);
}

template <>
inline Simd<uint8, detail::Simulated> packu_saturated(
    Simd<int16, detail::Simulated> lhs, Simd<int16, detail::Simulated> rhs) {
  return simulated::packu_saturated(lhs, rhs);
}

template <>
inline Simd<uint16, detail::Simulated> packu_saturated(
    Simd<int32, detail::Simulated> lhs, Simd<int32, detail::Simulated> rhs) {
  return simulated::packu_saturated(lhs, rhs);
}

template <>
inline Simd<float, detail::Simulated> round(
    Simd<float, detail::Simulated> simd) {
  return simulated::round(simd);
}

template <>
inline Simd<double, detail::Simulated> round(
    Simd<double, detail::Simulated> simd) {
  return simulated::round(simd);
}

template <>
inline Simd<int32, detail::Simulated> round_to_integer(
    Simd<float, detail::Simulated> simd) {
  return simulated::round_to_integer<int32>(simd);
}

// TODO(maskray) this specialization is duplicated in each *_impl-inl.inc file,
// which should be addressed later. The implementation of dimsum::split should
// also be revisited, it does not work on int8/uint8 Simd types.
template <typename T>
Simd<ScaleBy<T, 2>, detail::Simulated> mul_widened(
    Simd<T, detail::HalfSimulated> lhs, Simd<T, detail::HalfSimulated> rhs) {
  return convert<ScaleBy<T, 2>>(lhs) * convert<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum
