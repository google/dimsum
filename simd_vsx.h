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

SIMD_SPECIALIZATION(int8, StoragePolicy::kVsx, 16, __vector signed char)
SIMD_SPECIALIZATION(int16, StoragePolicy::kVsx, 16, __vector short)
SIMD_SPECIALIZATION(int32, StoragePolicy::kVsx, 16, __vector int)
SIMD_SPECIALIZATION(int64, StoragePolicy::kVsx, 16, __vector long long)
SIMD_SPECIALIZATION(uint8, StoragePolicy::kVsx, 16, __vector unsigned char)
SIMD_SPECIALIZATION(uint16, StoragePolicy::kVsx, 16, __vector unsigned short)
SIMD_SPECIALIZATION(uint32, StoragePolicy::kVsx, 16, __vector unsigned int)
SIMD_SPECIALIZATION(uint64, StoragePolicy::kVsx, 16,
                    __vector unsigned long long)
SIMD_SPECIALIZATION(float, StoragePolicy::kVsx, 16, __vector float)
SIMD_SPECIALIZATION(double, StoragePolicy::kVsx, 16, __vector double)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(StoragePolicy::kVsx);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kVsx, 8);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kVsx, 32);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kVsx, 64);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kVsx, 128);

template <typename T>
typename SimdTraits<T, Abi<StoragePolicy::kVsx, 16>>::ExternalType
load_vsx_vector_aligned(const T* buffer);

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, 8>, flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, 8>> Apply(const T* buffer) {
    Simd<T, Abi<StoragePolicy::kVsx, 8>> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumBytes>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, kNumBytes>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, kNumBytes>> Apply(const T* buffer) {
    Simd<T, Abi<StoragePolicy::kVsx, kNumBytes>> ret;
    __vector int ret1[sizeof(ret) / 16];
    for (int i = 0; i < sizeof(ret) / 16; i++)
      ret1[i] = reinterpret_cast<const __vector int*>(buffer)[i];
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, 16>, flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, 16>> Apply(const T* buffer) {
    return *reinterpret_cast<const typename Simd<
        T, Abi<StoragePolicy::kVsx, 16>>::Traits::ExternalType*>(buffer);
  }
};

}  // namespace detail

template <typename T>
using NativeSimd = Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 16>>;

template <typename T>
using Simd128 = Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 16>>;

template <typename T>
using Simd64 = Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 8>>;

template <>
inline Simd128<int8> abs(Simd128<int8> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<int16> abs(Simd128<int16> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<int32> abs(Simd128<int32> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<int64> abs(Simd128<int64> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<float> abs(Simd128<float> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<double> abs(Simd128<double> simd) {
  return vec_abs(simd.raw());
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return vec_re(simd.raw());
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return vec_sqrt(simd.raw());
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return vec_sqrt(simd.raw());
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
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
inline Simd128<int32> reduce_add<int32, 4>(Simd128<int8> simd) {
  return vec_sum4s(simd.raw(), Simd128<int32>(0).raw());
}

template <>
inline Simd128<uint32> reduce_add<uint32, 4>(Simd128<uint8> simd) {
  return vec_sum4s(simd.raw(), Simd128<uint32>(0).raw());
}

template <>
inline Simd128<int32> reduce_add<int32, 4>(Simd128<int16> simd) {
  return vec_sum4s(simd.raw(), Simd128<int32>(0).raw());
}

template <>
inline Simd128<uint32> reduce_add<uint32, 4>(Simd128<uint16> simd) {
  return vec_msum(simd.raw(), Simd128<uint16>(1).raw(),
                  Simd128<uint32>(0).raw());
}

// Common reduce_add<T, 1> specializations.

// vec_sums(vector int, vector int) (vsumsws) returns saturated sum.
// Note, reduce_add overflow has undefined behavior, so we are free to use this
// saturation semantics.
template <>
inline ResizeTo<Simd128<int32>, 1> reduce_add<int32, 1>(Simd128<int32> simd) {
  return vec_sums(simd.raw(), Simd128<int32>(0).raw())[3];
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
inline float reduce(const Simd128<float>& simd, std::plus<float>) {
  return (simd[0] + simd[2]) + (simd[1] + simd[3]);
}

template <>
inline Simd128<int32> mul_sum(Simd128<int16> lhs, Simd128<int16> rhs,
                              Simd128<int32> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

template <>
inline Simd128<uint32> mul_sum(Simd128<uint16> lhs, Simd128<uint16> rhs,
                               Simd128<uint32> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

// vec_rint calls XVR[SD]PIC which round floating points according to the
// Rounding Control field RN in FPSCR, which defaults to round-to-even.
template <>
inline Simd128<float> round(Simd128<float> simd) {
  return vec_rint(simd.raw());
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return vec_rint(simd.raw());
}

// The alnerative implementation of float-to-int conversion on PPC. Note that
// there is no instruction to directly convert a f.p. vector to an integer
// vector. It is implememented by first rounding it to the nearest representable
// floating-point integers, and then truncating them to integers.
template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return vec_cts(vec_rint(simd.raw()), 0);
}

template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#undef SIMD_SPECIALIZATION

#endif  // DIMSUM_SIMD_VSX_H_
