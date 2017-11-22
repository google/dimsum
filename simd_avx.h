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

#include <immintrin.h>

#include "simd.h"

namespace dimsum {
namespace detail {

// AVX (32 bytes)
SIMD_SPECIALIZATION(int8, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int16, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int32, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int64, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint8, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint16, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint32, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint64, StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(float, StoragePolicy::kYmm, 16, __m128)
SIMD_SPECIALIZATION(double, StoragePolicy::kYmm, 16, __m128d)

SIMD_SPECIALIZATION(int8, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int16, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int32, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int64, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint8, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint16, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint32, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint64, StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(float, StoragePolicy::kYmm, 32, __m256)
SIMD_SPECIALIZATION(double, StoragePolicy::kYmm, 32, __m256d)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(StoragePolicy::kYmm);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kYmm, 8);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kYmm, 64);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kYmm, 128);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kYmm, 256);

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kYmm, 16 / sizeof(T)>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kYmm, 16 / sizeof(T)>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kYmm, 16 / sizeof(T)>> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumElements>
struct LoadImpl<T, Abi<StoragePolicy::kYmm, kNumElements>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kYmm, kNumElements>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kYmm, kNumElements>> ret;
    __m256i ret1[sizeof(ret) / 32];
    for (int i = 0; i < sizeof(ret) / 32; i++)
      ret1[i] = _mm256_load_si256(reinterpret_cast<const __m256i*>(buffer) + i);
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
using Simd256 =
    Simd<T, detail::Abi<detail::StoragePolicy::kYmm, 32 / sizeof(T)>>;

}  // namespace detail

template <typename T>
using NativeSimd =
    Simd<T, detail::Abi<detail::StoragePolicy::kYmm, 32 / sizeof(T)>>;

// Some instructions (align, pack, unpack, permutation types, e.g.
// _mm256_alignr_epi8 _mm256_packs_epi32) operate on 128-bit lanes rather the
// whole register, elements do not cross the 128-bit middle boundary.
// Extra shuffles are used to make them intuitional. Refer to Intel Intrinsics
// Guide for details.

template <>
inline detail::Simd256<int8> abs(detail::Simd256<int8> simd) {
  return _mm256_abs_epi8(simd.raw());
}

template <>
inline detail::Simd256<int16> abs(detail::Simd256<int16> simd) {
  return _mm256_abs_epi16(simd.raw());
}

template <>
inline detail::Simd256<int32> abs(detail::Simd256<int32> simd) {
  return _mm256_abs_epi32(simd.raw());
}

#ifdef __AVX512VL__
template <>
inline detail::Simd256<int64> abs(detail::Simd256<int32> simd) {
  return _mm256_abs_epi64(simd.raw());
}
#else
template <>
inline detail::Simd256<int64> abs(detail::Simd256<int64> simd) {
  return detail::Simd256<int64>::list(std::abs(simd[0]), std::abs(simd[1]),
                                      std::abs(simd[2]), std::abs(simd[3]));
}
#endif

// ::abs is implemented by bitand each lane with shr(-1, 1) to clear the
// sign bit (the most significant bit).
template <>
inline detail::Simd256<float> abs(detail::Simd256<float> simd) {
  return bit_cast<float>(bit_cast<uint32>(simd) &
                         detail::Simd256<uint32>(~(1u << 31)));
}

template <>
inline detail::Simd256<double> abs(detail::Simd256<double> simd) {
  return bit_cast<double>(bit_cast<uint64>(simd) &
                          detail::Simd256<uint64>(~(1ull << 63)));
}

template <>
inline detail::Simd256<float> reciprocal_estimate(detail::Simd256<float> simd) {
  return _mm256_rcp_ps(simd.raw());
}

template <>
inline detail::Simd256<float> sqrt(detail::Simd256<float> simd) {
  return _mm256_sqrt_ps(simd.raw());
}

template <>
inline detail::Simd256<double> sqrt(detail::Simd256<double> simd) {
  return _mm256_sqrt_pd(simd.raw());
}

template <>
inline detail::Simd256<float> reciprocal_sqrt_estimate(
    detail::Simd256<float> simd) {
  return _mm256_rsqrt_ps(simd.raw());
}

template <>
inline detail::Simd256<int8> add_saturated(detail::Simd256<int8> lhs,
                                           detail::Simd256<int8> rhs) {
  return _mm256_adds_epi8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint8> add_saturated(detail::Simd256<uint8> lhs,
                                            detail::Simd256<uint8> rhs) {
  return _mm256_adds_epu8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int16> add_saturated(detail::Simd256<int16> lhs,
                                            detail::Simd256<int16> rhs) {
  return _mm256_adds_epi16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint16> add_saturated(detail::Simd256<uint16> lhs,
                                             detail::Simd256<uint16> rhs) {
  return _mm256_adds_epu16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int8> sub_saturated(detail::Simd256<int8> lhs,
                                           detail::Simd256<int8> rhs) {
  return _mm256_subs_epi8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint8> sub_saturated(detail::Simd256<uint8> lhs,
                                            detail::Simd256<uint8> rhs) {
  return _mm256_subs_epu8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int16> sub_saturated(detail::Simd256<int16> lhs,
                                            detail::Simd256<int16> rhs) {
  return _mm256_subs_epi16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint16> sub_saturated(detail::Simd256<uint16> lhs,
                                             detail::Simd256<uint16> rhs) {
  return _mm256_subs_epu16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int8> min(detail::Simd256<int8> lhs,
                                 detail::Simd256<int8> rhs) {
  return _mm256_min_epi8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int16> min(detail::Simd256<int16> lhs,
                                  detail::Simd256<int16> rhs) {
  return _mm256_min_epi16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int32> min(detail::Simd256<int32> lhs,
                                  detail::Simd256<int32> rhs) {
  return _mm256_min_epi32(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint8> min(detail::Simd256<uint8> lhs,
                                  detail::Simd256<uint8> rhs) {
  return _mm256_min_epu8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint16> min(detail::Simd256<uint16> lhs,
                                   detail::Simd256<uint16> rhs) {
  return _mm256_min_epu16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint32> min(detail::Simd256<uint32> lhs,
                                   detail::Simd256<uint32> rhs) {
  return _mm256_min_epu32(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<float> min(detail::Simd256<float> lhs,
                                  detail::Simd256<float> rhs) {
  return _mm256_min_ps(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<double> min(detail::Simd256<double> lhs,
                                   detail::Simd256<double> rhs) {
  return _mm256_min_pd(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int8> max(detail::Simd256<int8> lhs,
                                 detail::Simd256<int8> rhs) {
  return _mm256_max_epi8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int16> max(detail::Simd256<int16> lhs,
                                  detail::Simd256<int16> rhs) {
  return _mm256_max_epi16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<int32> max(detail::Simd256<int32> lhs,
                                  detail::Simd256<int32> rhs) {
  return _mm256_max_epi32(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint8> max(detail::Simd256<uint8> lhs,
                                  detail::Simd256<uint8> rhs) {
  return _mm256_max_epu8(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint16> max(detail::Simd256<uint16> lhs,
                                   detail::Simd256<uint16> rhs) {
  return _mm256_max_epu16(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint32> max(detail::Simd256<uint32> lhs,
                                   detail::Simd256<uint32> rhs) {
  return _mm256_max_epu32(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<float> max(detail::Simd256<float> lhs,
                                  detail::Simd256<float> rhs) {
  return _mm256_max_ps(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<double> max(detail::Simd256<double> lhs,
                                   detail::Simd256<double> rhs) {
  return _mm256_max_pd(lhs.raw(), rhs.raw());
}

template <>
inline detail::Simd256<uint64> reduce_add<uint64, 4>(
    detail::Simd256<uint8> simd) {
  return _mm256_sad_epu8(simd.raw(), detail::Simd256<uint8>(0).raw());
}

template <>
inline detail::Simd256<int32> reduce_add<int32, 8>(
    detail::Simd256<int16> simd) {
  return _mm256_madd_epi16(simd.raw(), detail::Simd256<int16>(1).raw());
}

template <>
inline ResizeTo<detail::Simd256<uint64>, 1> reduce_add<uint64, 1>(
    detail::Simd256<uint8> simd) {
  return reduce_add<uint64, 1>(reduce_add<uint64, 4>(simd));
}

template <>
inline ResizeTo<detail::Simd256<uint64>, 1> reduce_add<uint64, 1>(
    detail::Simd256<uint16> simd) {
  uint64 even_sum = reduce_add<uint64, 1>(
      bit_cast<uint8>(simd & detail::Simd256<uint16>(0x00ff)))[0];
  uint64 odd_sum = reduce_add<uint64, 1>(
      bit_cast<uint8>(simd & detail::Simd256<uint16>(0xff00)))[0];
  return (odd_sum << 8) + even_sum;
}

template <>
inline float reduce(const detail::Simd256<float>& simd, std::plus<float>) {
  return ((simd[0] + simd[4]) + (simd[1] + simd[5])) +
         ((simd[2] + simd[6]) + (simd[3] + simd[7]));
}

template <>
inline double reduce(const detail::Simd256<double>& simd, std::plus<double>) {
  return (simd[0] + simd[2]) + (simd[1] + simd[3]);
}

template <>
inline detail::Simd256<int32> mul_sum<int32>(detail::Simd256<int16> lhs,
                                             detail::Simd256<int16> rhs,
                                             detail::Simd256<int32> acc) {
  return _mm256_add_epi32(acc.raw(), _mm256_madd_epi16(lhs.raw(), rhs.raw()));
}

// _MM_FROUND_TO_NEAREST_INT specifies round-to-even.
template <>
inline detail::Simd256<float> round(detail::Simd256<float> simd) {
  return _mm256_round_ps(simd.raw(),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline detail::Simd256<double> round(detail::Simd256<double> simd) {
  return _mm256_round_pd(simd.raw(),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline detail::Simd256<int32> round_to_integer(detail::Simd256<float> simd) {
  return _mm256_cvtps_epi32(simd.raw());
}

template <typename T>
detail::Simd256<ScaleBy<T, 2>> mul_widened(
    Simd<T, detail::Abi<detail::StoragePolicy::kYmm, 16 / sizeof(T)>> lhs,
    Simd<T, detail::Abi<detail::StoragePolicy::kYmm, 16 / sizeof(T)>> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#undef SIMD_SPECIALIZATION

#endif  // DIMSUM_SIMD_AVX_H_
