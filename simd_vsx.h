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
namespace detail {

template <typename T>
struct AltivecTraits {};

template <>
struct AltivecTraits<int8> {
  using type = __vector signed char;
};
template <>
struct AltivecTraits<int16> {
  using type = __vector signed short;
};
template <>
struct AltivecTraits<int32> {
  using type = __vector signed int;
};
template <>
struct AltivecTraits<int64> {
  using type = __vector signed long long;
};
template <>
struct AltivecTraits<uint8> {
  using type = __vector unsigned char;
};
template <>
struct AltivecTraits<uint16> {
  using type = __vector unsigned short;
};
template <>
struct AltivecTraits<uint32> {
  using type = __vector unsigned int;
};
template <>
struct AltivecTraits<uint64> {
  using type = __vector unsigned long long;
};
template <>
struct AltivecTraits<float> {
  using type = __vector float;
};
template <>
struct AltivecTraits<double> {
  using type = __vector double;
};

template <typename T>
struct ExternalTypeTraits<T, detail::Abi<StoragePolicy::kVsx, 16 / sizeof(T)>> {
  using type = typename AltivecTraits<T>::type;
};

template <typename T>
typename AltivecTraits<T>::type load_vsx_vector_aligned(const T* buffer);

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, 8 / sizeof(T)>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, 8 / sizeof(T)>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kVsx, 8 / sizeof(T)>> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumElements>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, kNumElements>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, kNumElements>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kVsx, kNumElements>> ret;
    __vector int ret1[sizeof(ret) / 16];
    for (int i = 0; i < sizeof(ret) / 16; i++)
      ret1[i] = reinterpret_cast<const __vector int*>(buffer)[i];
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kVsx, 16 / sizeof(T)>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kVsx, 16 / sizeof(T)>> Apply(
      const T* buffer) {
    return *reinterpret_cast<const typename AltivecTraits<T>::type*>(buffer);
  }
};

}  // namespace detail

template <typename T>
using NativeSimd =
    Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 16 / sizeof(T)>>;

template <typename T>
using Simd128 =
    Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 16 / sizeof(T)>>;

template <typename T>
using Simd64 = Simd<T, detail::Abi<detail::StoragePolicy::kVsx, 8 / sizeof(T)>>;

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

template <typename T>
Simd128<T> min(Simd128<T> lhs, Simd128<T> rhs) {
  return vec_min(lhs.raw(), rhs.raw());
}

template <typename T>
Simd128<T> max(Simd128<T> lhs, Simd128<T> rhs) {
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
inline Simd128<uint32> mul_sum<uint32>(Simd128<uint8> lhs, Simd128<uint8> rhs,
                                       Simd128<uint32> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

template <>
inline Simd128<int32> mul_sum<int32>(Simd128<int16> lhs, Simd128<int16> rhs,
                                     Simd128<int32> acc) {
  return vec_msum(lhs.raw(), rhs.raw(), acc.raw());
}

template <>
inline Simd128<uint32> mul_sum<uint32>(Simd128<uint16> lhs, Simd128<uint16> rhs,
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

#endif  // DIMSUM_SIMD_VSX_H_
