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
using YMM = detail::Abi<detail::StoragePolicy::kYmm, 32>;
using HalfYMM = detail::Abi<detail::StoragePolicy::kYmm, 16>;

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kYmm, 16, __m128i)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kYmm, 16, __m128)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kYmm, 16, __m128d)

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kYmm, 32, __m256i)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kYmm, 32, __m256)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kYmm, 32, __m256d)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(detail::StoragePolicy::kYmm);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kYmm, 8);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kYmm, 64);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kYmm, 128);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kYmm, 256);

template <typename T>
struct LoadImpl<T, detail::HalfYMM, flags::vector_aligned_tag> {
  static Simd<T, detail::HalfYMM> Apply(const T* buffer) {
    Simd<T, detail::HalfYMM> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumBytes>
struct LoadImpl<T, detail::Abi<detail::StoragePolicy::kYmm, kNumBytes>,
                flags::vector_aligned_tag> {
  static Simd<T, detail::Abi<detail::StoragePolicy::kYmm, kNumBytes>> Apply(
      const T* buffer) {
    Simd<T, detail::Abi<detail::StoragePolicy::kYmm, kNumBytes>> ret;
    __m256i ret1[sizeof(ret) / 32];
    for (int i = 0; i < sizeof(ret) / 32; i++)
      ret1[i] = _mm256_load_si256(reinterpret_cast<const __m256i*>(buffer) + i);
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

}  // namespace detail

template <typename T>
using NativeSimd = Simd<T, detail::YMM>;

// Some instructions (align, pack, unpack, permutation types, e.g.
// _mm256_alignr_epi8 _mm256_packs_epi32) operate on 128-bit lanes rather the
// whole register, elements do not cross the 128-bit middle boundary.
// Extra shuffles are used to make them intuitional. Refer to Intel Intrinsics
// Guide for details.

template <>
inline Simd<int8, detail::YMM> abs(Simd<int8, detail::YMM> simd) {
  return _mm256_abs_epi8(simd.raw());
}

template <>
inline Simd<int16, detail::YMM> abs(Simd<int16, detail::YMM> simd) {
  return _mm256_abs_epi16(simd.raw());
}

template <>
inline Simd<int32, detail::YMM> abs(Simd<int32, detail::YMM> simd) {
  return _mm256_abs_epi32(simd.raw());
}

#ifdef __AVX512VL__
template <>
inline Simd<int64, detail::YMM> abs(Simd<int32, detail::YMM> simd) {
  return _mm256_abs_epi64(simd.raw());
}
#else
template <>
inline Simd<int64, detail::YMM> abs(Simd<int64, detail::YMM> simd) {
  return Simd<int64, detail::YMM>::list(std::abs(simd[0]), std::abs(simd[1]),
                                        std::abs(simd[2]), std::abs(simd[3]));
}
#endif

// ::abs is implemented by bitand each lane with shr(-1, 1) to clear the
// sign bit (the most significant bit).
template <>
inline Simd<float, detail::YMM> abs(Simd<float, detail::YMM> simd) {
  return bit_cast<float>(bit_cast<uint32>(simd) &
                         Simd<uint32, detail::YMM>(~(1u << 31)));
}

template <>
inline Simd<double, detail::YMM> abs(Simd<double, detail::YMM> simd) {
  return bit_cast<double>(bit_cast<uint64>(simd) &
                          Simd<uint64, detail::YMM>(~(1ull << 63)));
}

template <>
inline Simd<float, detail::YMM> reciprocal_estimate(
    Simd<float, detail::YMM> simd) {
  return _mm256_rcp_ps(simd.raw());
}

template <>
inline Simd<float, detail::YMM> sqrt(Simd<float, detail::YMM> simd) {
  return _mm256_sqrt_ps(simd.raw());
}

template <>
inline Simd<double, detail::YMM> sqrt(Simd<double, detail::YMM> simd) {
  return _mm256_sqrt_pd(simd.raw());
}

template <>
inline Simd<float, detail::YMM> reciprocal_sqrt_estimate(
    Simd<float, detail::YMM> simd) {
  return _mm256_rsqrt_ps(simd.raw());
}

template <>
inline Simd<int8, detail::YMM> add_saturated(Simd<int8, detail::YMM> lhs,
                                             Simd<int8, detail::YMM> rhs) {
  return _mm256_adds_epi8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint8, detail::YMM> add_saturated(Simd<uint8, detail::YMM> lhs,
                                              Simd<uint8, detail::YMM> rhs) {
  return _mm256_adds_epu8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int16, detail::YMM> add_saturated(Simd<int16, detail::YMM> lhs,
                                              Simd<int16, detail::YMM> rhs) {
  return _mm256_adds_epi16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint16, detail::YMM> add_saturated(Simd<uint16, detail::YMM> lhs,
                                               Simd<uint16, detail::YMM> rhs) {
  return _mm256_adds_epu16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int8, detail::YMM> sub_saturated(Simd<int8, detail::YMM> lhs,
                                             Simd<int8, detail::YMM> rhs) {
  return _mm256_subs_epi8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint8, detail::YMM> sub_saturated(Simd<uint8, detail::YMM> lhs,
                                              Simd<uint8, detail::YMM> rhs) {
  return _mm256_subs_epu8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int16, detail::YMM> sub_saturated(Simd<int16, detail::YMM> lhs,
                                              Simd<int16, detail::YMM> rhs) {
  return _mm256_subs_epi16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint16, detail::YMM> sub_saturated(Simd<uint16, detail::YMM> lhs,
                                               Simd<uint16, detail::YMM> rhs) {
  return _mm256_subs_epu16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int8, detail::YMM> min(Simd<int8, detail::YMM> lhs,
                                   Simd<int8, detail::YMM> rhs) {
  return _mm256_min_epi8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int16, detail::YMM> min(Simd<int16, detail::YMM> lhs,
                                    Simd<int16, detail::YMM> rhs) {
  return _mm256_min_epi16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int32, detail::YMM> min(Simd<int32, detail::YMM> lhs,
                                    Simd<int32, detail::YMM> rhs) {
  return _mm256_min_epi32(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint8, detail::YMM> min(Simd<uint8, detail::YMM> lhs,
                                    Simd<uint8, detail::YMM> rhs) {
  return _mm256_min_epu8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint16, detail::YMM> min(Simd<uint16, detail::YMM> lhs,
                                     Simd<uint16, detail::YMM> rhs) {
  return _mm256_min_epu16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint32, detail::YMM> min(Simd<uint32, detail::YMM> lhs,
                                     Simd<uint32, detail::YMM> rhs) {
  return _mm256_min_epu32(lhs.raw(), rhs.raw());
}

template <>
inline Simd<float, detail::YMM> min(Simd<float, detail::YMM> lhs,
                                    Simd<float, detail::YMM> rhs) {
  return _mm256_min_ps(lhs.raw(), rhs.raw());
}

template <>
inline Simd<double, detail::YMM> min(Simd<double, detail::YMM> lhs,
                                     Simd<double, detail::YMM> rhs) {
  return _mm256_min_pd(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int8, detail::YMM> max(Simd<int8, detail::YMM> lhs,
                                   Simd<int8, detail::YMM> rhs) {
  return _mm256_max_epi8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int16, detail::YMM> max(Simd<int16, detail::YMM> lhs,
                                    Simd<int16, detail::YMM> rhs) {
  return _mm256_max_epi16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int32, detail::YMM> max(Simd<int32, detail::YMM> lhs,
                                    Simd<int32, detail::YMM> rhs) {
  return _mm256_max_epi32(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint8, detail::YMM> max(Simd<uint8, detail::YMM> lhs,
                                    Simd<uint8, detail::YMM> rhs) {
  return _mm256_max_epu8(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint16, detail::YMM> max(Simd<uint16, detail::YMM> lhs,
                                     Simd<uint16, detail::YMM> rhs) {
  return _mm256_max_epu16(lhs.raw(), rhs.raw());
}

template <>
inline Simd<uint32, detail::YMM> max(Simd<uint32, detail::YMM> lhs,
                                     Simd<uint32, detail::YMM> rhs) {
  return _mm256_max_epu32(lhs.raw(), rhs.raw());
}

template <>
inline Simd<float, detail::YMM> max(Simd<float, detail::YMM> lhs,
                                    Simd<float, detail::YMM> rhs) {
  return _mm256_max_ps(lhs.raw(), rhs.raw());
}

template <>
inline Simd<double, detail::YMM> max(Simd<double, detail::YMM> lhs,
                                     Simd<double, detail::YMM> rhs) {
  return _mm256_max_pd(lhs.raw(), rhs.raw());
}

template <>
inline Simd<int8, detail::YMM> pack_saturated(Simd<int16, detail::YMM> lhs,
                                              Simd<int16, detail::YMM> rhs) {
  Simd<int64, detail::YMM> res(_mm256_packs_epi16(lhs.raw(), rhs.raw()));
  return bit_cast<int8, int64, detail::YMM>(shuffle<0, 2, 1, 3>(res, res));
}

template <>
inline Simd<int16, detail::YMM> pack_saturated(Simd<int32, detail::YMM> lhs,
                                               Simd<int32, detail::YMM> rhs) {
  Simd<int64, detail::YMM> res(_mm256_packs_epi32(lhs.raw(), rhs.raw()));
  return bit_cast<int16, int64, detail::YMM>(shuffle<0, 2, 1, 3>(res, res));
}

template <>
inline Simd<uint8, detail::YMM> packu_saturated(Simd<int16, detail::YMM> lhs,
                                                Simd<int16, detail::YMM> rhs) {
  Simd<uint64, detail::YMM> res(_mm256_packus_epi16(lhs.raw(), rhs.raw()));
  return bit_cast<uint8, uint64, detail::YMM>(shuffle<0, 2, 1, 3>(res, res));
}

template <>
inline Simd<uint16, detail::YMM> packu_saturated(Simd<int32, detail::YMM> lhs,
                                                 Simd<int32, detail::YMM> rhs) {
  Simd<uint64, detail::YMM> res(_mm256_packus_epi32(lhs.raw(), rhs.raw()));
  return bit_cast<uint16, uint64, detail::YMM>(shuffle<0, 2, 1, 3>(res, res));
}

template <>
inline Simd<uint64, detail::YMM> reduce_add<uint64, 4>(
    Simd<uint8, detail::YMM> simd) {
  return _mm256_sad_epu8(simd.raw(), Simd<uint8, detail::YMM>(0).raw());
}

template <>
inline Simd<int32, detail::YMM> reduce_add<int32, 8>(
    Simd<int16, detail::YMM> simd) {
  return _mm256_madd_epi16(simd.raw(), Simd<int16, detail::YMM>(1).raw());
}

template <>
inline ResizeTo<Simd<uint64, detail::YMM>, 1> reduce_add<uint64, 1>(
    Simd<uint8, detail::YMM> simd) {
  return reduce_add<uint64, 1>(reduce_add<uint64, 4>(simd));
}

template <>
inline ResizeTo<Simd<uint64, detail::YMM>, 1> reduce_add<uint64, 1>(
    Simd<uint16, detail::YMM> simd) {
  uint64 even_sum = reduce_add<uint64, 1>(
      bit_cast<uint8>(simd & Simd<uint16, detail::YMM>(0x00ff)))[0];
  uint64 odd_sum = reduce_add<uint64, 1>(
      bit_cast<uint8>(simd & Simd<uint16, detail::YMM>(0xff00)))[0];
  return (odd_sum << 8) + even_sum;
}

template <>
inline Simd<int32, detail::YMM> mul_sum(Simd<int16, detail::YMM> lhs,
                                        Simd<int16, detail::YMM> rhs,
                                        Simd<int32, detail::YMM> acc) {
  return _mm256_add_epi32(acc.raw(), _mm256_madd_epi16(lhs.raw(), rhs.raw()));
}

// _MM_FROUND_TO_NEAREST_INT specifies round-to-even.
template <>
inline Simd<float, detail::YMM> round(Simd<float, detail::YMM> simd) {
  return _mm256_round_ps(simd.raw(),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline Simd<double, detail::YMM> round(Simd<double, detail::YMM> simd) {
  return _mm256_round_pd(simd.raw(),
                         _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
}

template <>
inline Simd<int32, detail::YMM> round_to_integer(
    Simd<float, detail::YMM> simd) {
  return _mm256_cvtps_epi32(simd.raw());
}

template <typename T>
Simd<ScaleBy<T, 2>, detail::YMM> mul_widened(Simd<T, detail::HalfYMM> lhs,
                                             Simd<T, detail::HalfYMM> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#undef SIMD_SPECIALIZATION

#endif  // DIMSUM_SIMD_AVX_H_
