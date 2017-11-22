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

#include "simd.h"

namespace dimsum {
namespace detail {

SIMD_SPECIALIZATION(int8, StoragePolicy::kNeon, 8, int8x8_t)
SIMD_SPECIALIZATION(int16, StoragePolicy::kNeon, 8, int16x4_t)
SIMD_SPECIALIZATION(int32, StoragePolicy::kNeon, 8, int32x2_t)
SIMD_SPECIALIZATION(int64, StoragePolicy::kNeon, 8, int64x1_t)
SIMD_SPECIALIZATION(uint8, StoragePolicy::kNeon, 8, uint8x8_t)
SIMD_SPECIALIZATION(uint16, StoragePolicy::kNeon, 8, uint16x4_t)
SIMD_SPECIALIZATION(uint32, StoragePolicy::kNeon, 8, uint32x2_t)
SIMD_SPECIALIZATION(uint64, StoragePolicy::kNeon, 8, uint64x1_t)
SIMD_SPECIALIZATION(float, StoragePolicy::kNeon, 8, float32x2_t)
SIMD_SPECIALIZATION(double, StoragePolicy::kNeon, 8, float64x1_t)

SIMD_SPECIALIZATION(int8, StoragePolicy::kNeon, 16, int8x16_t)
SIMD_SPECIALIZATION(int16, StoragePolicy::kNeon, 16, int16x8_t)
SIMD_SPECIALIZATION(int32, StoragePolicy::kNeon, 16, int32x4_t)
SIMD_SPECIALIZATION(int64, StoragePolicy::kNeon, 16, int64x2_t)
SIMD_SPECIALIZATION(uint8, StoragePolicy::kNeon, 16, uint8x16_t)
SIMD_SPECIALIZATION(uint16, StoragePolicy::kNeon, 16, uint16x8_t)
SIMD_SPECIALIZATION(uint32, StoragePolicy::kNeon, 16, uint32x4_t)
SIMD_SPECIALIZATION(uint64, StoragePolicy::kNeon, 16, uint64x2_t)
SIMD_SPECIALIZATION(float, StoragePolicy::kNeon, 16, float32x4_t)
SIMD_SPECIALIZATION(double, StoragePolicy::kNeon, 16, float64x2_t)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(StoragePolicy::kNeon);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kNeon, 32);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kNeon, 64);
SIMD_NON_NATIVE_SPECIALIZATION(StoragePolicy::kNeon, 128);

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kNeon, 8 / sizeof(T)>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kNeon, 8 / sizeof(T)>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kNeon, 8 / sizeof(T)>> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumElements>
struct LoadImpl<T, Abi<StoragePolicy::kNeon, kNumElements>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kNeon, kNumElements>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kNeon, kNumElements>> ret;
    uint64x2_t ret1[sizeof(ret) / 16];
    for (int i = 0; i < sizeof(ret) / 16; i++) {
      ret1[i] = vld1q_u64(reinterpret_cast<const uint64_t*>(buffer) + 2 * i);
    }
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
typename SimdTraits<T, Abi<StoragePolicy::kNeon, 16 / sizeof(T)>>::ExternalType
load_neon_vector_aligned(const T* buffer);

template <typename T>
struct LoadImpl<T, Abi<StoragePolicy::kNeon, 16 / sizeof(T)>,
                flags::vector_aligned_tag> {
  static Simd<T, Abi<StoragePolicy::kNeon, 16 / sizeof(T)>> Apply(
      const T* buffer) {
    Simd<T, Abi<StoragePolicy::kNeon, 16 / sizeof(T)>> ret;
    auto value = load_neon_vector_aligned(buffer);
    memcpy(&ret.storage_, &value, sizeof(ret));
    return ret;
  }
};

template <>
inline typename SimdTraits<int8, Abi<StoragePolicy::kNeon, 16>>::ExternalType
load_neon_vector_aligned(const int8* buffer) {
  return vld1q_s8(buffer);
}

template <>
inline typename SimdTraits<int16, Abi<StoragePolicy::kNeon, 8>>::ExternalType
load_neon_vector_aligned(const int16* buffer) {
  return vld1q_s16(buffer);
}

template <>
inline typename SimdTraits<int32, Abi<StoragePolicy::kNeon, 4>>::ExternalType
load_neon_vector_aligned(const int32* buffer) {
  return vld1q_s32(buffer);
}

template <>
inline typename SimdTraits<int64, Abi<StoragePolicy::kNeon, 2>>::ExternalType
load_neon_vector_aligned(const int64* buffer) {
  return vld1q_s64(reinterpret_cast<const int64_t*>(buffer));
}

template <>
inline typename SimdTraits<uint8, Abi<StoragePolicy::kNeon, 16>>::ExternalType
load_neon_vector_aligned(const uint8* buffer) {
  return vld1q_u8(buffer);
}

template <>
inline typename SimdTraits<uint16, Abi<StoragePolicy::kNeon, 8>>::ExternalType
load_neon_vector_aligned(const uint16* buffer) {
  return vld1q_u16(buffer);
}

template <>
inline typename SimdTraits<uint32, Abi<StoragePolicy::kNeon, 4>>::ExternalType
load_neon_vector_aligned(const uint32* buffer) {
  return vld1q_u32(buffer);
}

template <>
inline typename SimdTraits<uint64, Abi<StoragePolicy::kNeon, 2>>::ExternalType
load_neon_vector_aligned(const uint64* buffer) {
  return vld1q_u64(reinterpret_cast<const uint64_t*>(buffer));
}

template <>
inline typename SimdTraits<float, Abi<StoragePolicy::kNeon, 4>>::ExternalType
load_neon_vector_aligned(const float* buffer) {
  return vld1q_f32(buffer);
}

template <>
inline typename SimdTraits<double, Abi<StoragePolicy::kNeon, 2>>::ExternalType
load_neon_vector_aligned(const double* buffer) {
  return vld1q_f64(buffer);
}

// TODO(dimsum): Optimize horizontal_sum().
// llvm.experimental.vector.reduce.add.* already lowers to single instructions
// like addv, but I'm not sure if there are intrinsics for them.
}  // namespace detail

template <typename T>
using NativeSimd =
    Simd<T, detail::Abi<detail::StoragePolicy::kNeon, 16 / sizeof(T)>>;

template <typename T>
using Simd128 =
    Simd<T, detail::Abi<detail::StoragePolicy::kNeon, 16 / sizeof(T)>>;

template <typename T>
using Simd64 =
    Simd<T, detail::Abi<detail::StoragePolicy::kNeon, 8 / sizeof(T)>>;

template <>
inline Simd128<int8> abs(Simd128<int8> simd) {
  return vabsq_s8(simd.raw());
}

template <>
inline Simd128<int16> abs(Simd128<int16> simd) {
  return vabsq_s16(simd.raw());
}

template <>
inline Simd128<int32> abs(Simd128<int32> simd) {
  return vabsq_s32(simd.raw());
}

template <>
inline Simd128<int64> abs(Simd128<int64> simd) {
  return vabsq_s64(simd.raw());
}

template <>
inline Simd128<float> abs(Simd128<float> simd) {
  return vabsq_f32(simd.raw());
}

template <>
inline Simd128<double> abs(Simd128<double> simd) {
  return vabsq_f64(simd.raw());
}

template <>
inline Simd128<float> reciprocal_estimate(Simd128<float> simd) {
  return vrecpeq_f32(simd.raw());
}

template <>
inline Simd128<float> sqrt(Simd128<float> simd) {
  return vsqrtq_f32(simd.raw());
}

template <>
inline Simd128<double> sqrt(Simd128<double> simd) {
  return vsqrtq_f64(simd.raw());
}

template <>
inline Simd128<float> reciprocal_sqrt_estimate(Simd128<float> simd) {
  return vrsqrteq_f32(simd.raw());
}

template <>
inline Simd128<int8> add_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vqaddq_s8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint8> add_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vqaddq_u8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int16> add_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vqaddq_s16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint16> add_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vqaddq_u16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int8> sub_saturated(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vqsubq_s8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint8> sub_saturated(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vqsubq_u8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int16> sub_saturated(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vqsubq_s16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint16> sub_saturated(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vqsubq_u16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int8> min(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vminq_s8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int16> min(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vminq_s16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int32> min(Simd128<int32> lhs, Simd128<int32> rhs) {
  return vminq_s32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint8> min(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vminq_u8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint16> min(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vminq_u16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint32> min(Simd128<uint32> lhs, Simd128<uint32> rhs) {
  return vminq_u32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<float> min(Simd128<float> lhs, Simd128<float> rhs) {
  return vminq_f32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<double> min(Simd128<double> lhs, Simd128<double> rhs) {
  return vminq_f64(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int8> max(Simd128<int8> lhs, Simd128<int8> rhs) {
  return vmaxq_s8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int16> max(Simd128<int16> lhs, Simd128<int16> rhs) {
  return vmaxq_s16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<int32> max(Simd128<int32> lhs, Simd128<int32> rhs) {
  return vmaxq_s32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint8> max(Simd128<uint8> lhs, Simd128<uint8> rhs) {
  return vmaxq_u8(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint16> max(Simd128<uint16> lhs, Simd128<uint16> rhs) {
  return vmaxq_u16(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<uint32> max(Simd128<uint32> lhs, Simd128<uint32> rhs) {
  return vmaxq_u32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<float> max(Simd128<float> lhs, Simd128<float> rhs) {
  return vmaxq_f32(lhs.raw(), rhs.raw());
}

template <>
inline Simd128<double> max(Simd128<double> lhs, Simd128<double> rhs) {
  return vmaxq_f64(lhs.raw(), rhs.raw());
}

// An alternative implementation of the SSE intrinsic function _mm_madd_epi16
// on ARM. It breaks a Simd object into the low and high parts. Then values in
// each part are multiplied and summed pairwisely before being concatenated.
template <>
inline Simd128<int32> mul_sum(Simd128<int16> lhs, Simd128<int16> rhs,
                              Simd128<int32> acc) {
  int16x8_t lhs_raw = lhs.raw();
  int16x8_t rhs_raw = rhs.raw();
  int32x4_t mullo = vmull_s16(vget_low_s16(lhs_raw), vget_low_s16(rhs_raw));
  int32x4_t mulhi = vmull_s16(vget_high_s16(lhs_raw), vget_high_s16(rhs_raw));
  int32x2_t addlo = vpadd_s32(vget_low_s32(mullo), vget_high_s32(mullo));
  int32x2_t addhi = vpadd_s32(vget_low_s32(mulhi), vget_high_s32(mulhi));
  return vaddq_s32(acc.raw(), vcombine_s32(addlo, addhi));
}

// vrndnq_f{32,64} translate to VRINTN.F{16,32}, which round floating points
// using the round-to-even rule (Round to Nearest rounding mode in ARM
// parlance).
template <>
inline Simd128<float> round(Simd128<float> simd) {
  return vrndnq_f32(simd.raw());
}

template <>
inline Simd128<double> round(Simd128<double> simd) {
  return vrndnq_f64(simd.raw());
}

template <>
inline Simd128<int32> round_to_integer(Simd128<float> simd) {
  return vcvtnq_s32_f32(simd.raw());
}

template <typename T>
Simd128<ScaleBy<T, 2>> mul_widened(Simd64<T> lhs, Simd64<T> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum

#undef SIMD_SPECIALIZATION

#endif  // DIMSUM_SIMD_NEON_H_
