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

#include "simd.h"

namespace dimsum {
namespace detail {

using VSX = detail::Abi<detail::StoragePolicy::kVsxReg, 16>;
using HalfVSX = detail::Abi<detail::StoragePolicy::kVsxReg, 8>;

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kVsxReg, 16,
                    __vector signed char)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kVsxReg, 16, __vector short)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kVsxReg, 16, __vector int)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kVsxReg, 16,
                    __vector long long)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kVsxReg, 16,
                    __vector unsigned char)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kVsxReg, 16,
                    __vector unsigned short)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kVsxReg, 16,
                    __vector unsigned int)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kVsxReg, 16,
                    __vector unsigned long long)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kVsxReg, 16, __vector float)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kVsxReg, 16, __vector double)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(detail::StoragePolicy::kVsxReg);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kVsxReg, 8);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kVsxReg, 32);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kVsxReg, 64);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kVsxReg, 128);

template <typename T>
typename detail::SimdTraits<T, detail::VSX>::ExternalType
load_vsx_vector_aligned(const T* buffer);

template <typename T>
struct LoadImpl<T, detail::HalfVSX, flags::vector_aligned_tag> {
  static Simd<T, detail::HalfVSX> Apply(const T* buffer) {
    Simd<T, detail::HalfVSX> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumBytes>
struct LoadImpl<T, detail::Abi<detail::StoragePolicy::kVsxReg, kNumBytes>,
                flags::vector_aligned_tag> {
  static Simd<T, detail::Abi<detail::StoragePolicy::kVsxReg, kNumBytes>> Apply(
      const T* buffer) {
    Simd<T, detail::Abi<detail::StoragePolicy::kVsxReg, kNumBytes>> ret;
    __vector int ret1[sizeof(ret) / 16];
    for (int i = 0; i < sizeof(ret) / 16; i++)
      ret1[i] = reinterpret_cast<const __vector int*>(buffer)[i];
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
struct LoadImpl<T, detail::VSX, flags::vector_aligned_tag> {
  static Simd<T, detail::VSX> Apply(const T* buffer) {
    return *reinterpret_cast<
        const typename Simd<T, detail::VSX>::Traits::ExternalType*>(buffer);
  }
};

}  // namespace detail

template <typename T>
using NativeSimd = Simd<T, detail::VSX>;

template <typename T>
using Simd128 = Simd<T, detail::VSX>;

template <typename T>
using Simd64 = Simd<T, detail::HalfVSX>;

template <>
inline Simd<int8, detail::VSX> abs(Simd<int8, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<int16, detail::VSX> abs(Simd<int16, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<int32, detail::VSX> abs(Simd<int32, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<int64, detail::VSX> abs(Simd<int64, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<float, detail::VSX> abs(Simd<float, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<double, detail::VSX> abs(Simd<double, detail::VSX> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd<float, detail::VSX> reciprocal_estimate(
    Simd<float, detail::VSX> simd) {
  return vec_re(simd.raw());
}

template <>
inline Simd<float, detail::VSX> sqrt(Simd<float, detail::VSX> simd) {
  return vec_sqrt(simd.raw());
}

template <>
inline Simd<double, detail::VSX> sqrt(Simd<double, detail::VSX> simd) {
  return vec_sqrt(simd.raw());
}

template <>
inline Simd<float, detail::VSX> reciprocal_sqrt_estimate(
    Simd<float, detail::VSX> simd) {
  return vec_rsqrte(simd.raw());
}

template <typename T, typename Abi>
inline Simd<T, Abi> add_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return vec_adds(lhs.raw(), rhs.raw());
}

template <typename T, typename Abi>
inline Simd<T, Abi> sub_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return vec_subs(lhs.raw(), rhs.raw());
}

template <typename T, typename Abi>
Simd<T, Abi> min(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return vec_min(lhs.raw(), rhs.raw());
}

template <typename T, typename Abi>
Simd<T, Abi> max(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return vec_max(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int32, detail::VSX> reduce_add<int32, 4>(
    Simd<int8, detail::VSX> simd) {
  return vec_sum4s(simd.raw(), Simd<int32, detail::VSX>(0).raw());
}

template <>
inline Simd<uint32, detail::VSX> reduce_add<uint32, 4>(
    Simd<uint8, detail::VSX> simd) {
  return vec_sum4s(simd.raw(), Simd<uint32, detail::VSX>(0).raw());
}

template <>
inline Simd<int32, detail::VSX> reduce_add<int32, 4>(
    Simd<int16, detail::VSX> simd) {
  return vec_sum4s(simd.raw(), Simd<int32, detail::VSX>(0).raw());
}

template <>
inline Simd<uint32, detail::VSX> reduce_add<uint32, 4>(
    Simd<uint16, detail::VSX> simd) {
  return vec_msum(simd.raw(), Simd<uint16, detail::VSX>(1).raw(),
                  Simd<uint32, detail::VSX>(0).raw());
}

// Common reduce_add<T, 1> specializations.

// vec_sums(vector int, vector int) (vsumsws) returns saturated sum.
// Note, reduce_add overflow has undefined behavior, so we are free to use this
// saturation semantics.
template <>
inline ResizeTo<Simd<int32, detail::VSX>, 1> reduce_add<int32, 1>(
    Simd<int32, detail::VSX> simd) {
  return vec_sums(simd.raw(), Simd<int32, detail::VSX>(0).raw())[3];
}

template <>
inline ResizeTo<Simd<uint32, detail::VSX>, 1> reduce_add<uint32, 1>(
    Simd<uint8, detail::VSX> simd) {
  return bit_cast<uint32>(
      reduce_add<int32, 1>(bit_cast<int32>(reduce_add<uint32, 4>(simd))));
}

template <>
inline ResizeTo<Simd<uint64, detail::VSX>, 1> reduce_add<uint64, 1>(
    Simd<uint8, detail::VSX> simd) {
  return reduce_add<uint32, 1>(simd)[0];
}

template <>
inline ResizeTo<Simd<uint32, detail::VSX>, 1> reduce_add<uint32, 1>(
    Simd<uint16, detail::VSX> simd) {
  return bit_cast<uint32>(
      reduce_add<int32, 1>(bit_cast<int32>(reduce_add<uint32, 4>(simd))));
}

template <>
inline ResizeTo<Simd<uint64, detail::VSX>, 1> reduce_add<uint64, 1>(
    Simd<uint16, detail::VSX> simd) {
  return reduce_add<uint32, 1>(simd)[0];
}

template <>
inline float reduce<float, detail::VSX, std::plus<float>>(
    const Simd<float, detail::VSX>& simd, std::plus<float>) {
  return (simd[0] + simd[2]) + (simd[1] + simd[3]);
}

template <>
inline Simd<int32, detail::VSX> mul_sum(Simd<int16, detail::VSX> lhs,
                                        Simd<int16, detail::VSX> rhs,
                                        Simd<int32, detail::VSX> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

template <>
inline Simd<uint32, detail::VSX> mul_sum(Simd<uint16, detail::VSX> lhs,
                                         Simd<uint16, detail::VSX> rhs,
                                         Simd<uint32, detail::VSX> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

// vec_rint calls XVR[SD]PIC which round floating points according to the
// Rounding Control field RN in FPSCR, which defaults to round-to-even.
template <>
inline Simd<float, detail::VSX> round(Simd<float, detail::VSX> simd) {
  return vec_rint(simd.raw());
}

template <>
inline Simd<double, detail::VSX> round(Simd<double, detail::VSX> simd) {
  return vec_rint(simd.raw());
}

// The alnerative implementation of float-to-int conversion on PPC. Note that
// there is no instruction to directly convert a f.p. vector to an integer
// vector. It is implememented by first rounding it to the nearest representable
// floating-point integers, and then truncating them to integers.
template <>
inline Simd<int32, detail::VSX> round_to_integer(
    Simd<float, detail::VSX> simd) {
  return vec_cts(vec_rint(simd.raw()), 0);
}

template <typename T>
Simd<ScaleBy<T, 2>, detail::VSX> mul_widened(Simd<T, detail::HalfVSX> lhs,
                                             Simd<T, detail::HalfVSX> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#undef SIMD_SPECIALIZATION

#endif  // DIMSUM_SIMD_VSX_H_
