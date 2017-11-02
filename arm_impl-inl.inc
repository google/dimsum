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

#include <arm_neon.h>

namespace dimsum {
namespace detail {

using NEON = detail::Abi<detail::StoragePolicy::kNeon, 16>;
using HalfNEON = detail::Abi<detail::StoragePolicy::kNeon, 8>;

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kNeon, 8, int8x8_t)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kNeon, 8, int16x4_t)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kNeon, 8, int32x2_t)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kNeon, 8, int64x1_t)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kNeon, 8, uint8x8_t)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kNeon, 8, uint16x4_t)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kNeon, 8, uint32x2_t)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kNeon, 8, uint64x1_t)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kNeon, 8, float32x2_t)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kNeon, 8, float64x1_t)

SIMD_SPECIALIZATION(int8, detail::StoragePolicy::kNeon, 16, int8x16_t)
SIMD_SPECIALIZATION(int16, detail::StoragePolicy::kNeon, 16, int16x8_t)
SIMD_SPECIALIZATION(int32, detail::StoragePolicy::kNeon, 16, int32x4_t)
SIMD_SPECIALIZATION(int64, detail::StoragePolicy::kNeon, 16, int64x2_t)
SIMD_SPECIALIZATION(uint8, detail::StoragePolicy::kNeon, 16, uint8x16_t)
SIMD_SPECIALIZATION(uint16, detail::StoragePolicy::kNeon, 16, uint16x8_t)
SIMD_SPECIALIZATION(uint32, detail::StoragePolicy::kNeon, 16, uint32x4_t)
SIMD_SPECIALIZATION(uint64, detail::StoragePolicy::kNeon, 16, uint64x2_t)
SIMD_SPECIALIZATION(float, detail::StoragePolicy::kNeon, 16, float32x4_t)
SIMD_SPECIALIZATION(double, detail::StoragePolicy::kNeon, 16, float64x2_t)

SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(detail::StoragePolicy::kNeon);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kNeon, 32);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kNeon, 64);
SIMD_NON_NATIVE_SPECIALIZATION(detail::StoragePolicy::kNeon, 128);

template <typename T>
struct LoadImpl<T, detail::HalfNEON, flags::vector_aligned_tag> {
  static Simd<T, detail::HalfNEON> Apply(const T* buffer) {
    Simd<T, detail::HalfNEON> ret;
    memcpy(&ret.storage_, buffer, sizeof(ret));
    return ret;
  }
};

template <typename T, size_t kNumBytes>
struct LoadImpl<T, detail::Abi<detail::StoragePolicy::kNeon, kNumBytes>,
                flags::vector_aligned_tag> {
  static Simd<T, detail::Abi<detail::StoragePolicy::kNeon, kNumBytes>> Apply(
      const T* buffer) {
    Simd<T, detail::Abi<detail::StoragePolicy::kNeon, kNumBytes>> ret;
    uint64x2_t ret1[sizeof(ret) / 16];
    for (int i = 0; i < sizeof(ret) / 16; i++) {
      ret1[i] = vld1q_u64(reinterpret_cast<const uint64_t*>(buffer) +
                          16 / sizeof(T) * i);
    }
    memcpy(&ret.storage_, &ret1, sizeof(ret1));
    return ret;
  }
};

template <typename T>
typename detail::SimdTraits<T, detail::NEON>::ExternalType
load_neon_vector_aligned(const T* buffer);

template <typename T>
struct LoadImpl<T, detail::NEON, flags::vector_aligned_tag> {
  static Simd<T, detail::NEON> Apply(const T* buffer) {
    Simd<T, detail::NEON> ret;
    auto value = load_neon_vector_aligned(buffer);
    memcpy(&ret.storage_, &value, sizeof(ret));
    return ret;
  }
};

template <>
inline typename detail::SimdTraits<int8, detail::NEON>::ExternalType
load_neon_vector_aligned(const int8* buffer) {
  return vld1q_s8(buffer);
}

template <>
inline typename detail::SimdTraits<int16, detail::NEON>::ExternalType
load_neon_vector_aligned(const int16* buffer) {
  return vld1q_s16(buffer);
}

template <>
inline typename detail::SimdTraits<int32, detail::NEON>::ExternalType
load_neon_vector_aligned(const int32* buffer) {
  return vld1q_s32(buffer);
}

template <>
inline typename detail::SimdTraits<int64, detail::NEON>::ExternalType
load_neon_vector_aligned(const int64* buffer) {
  return vld1q_s64(reinterpret_cast<const int64_t*>(buffer));
}

template <>
inline typename detail::SimdTraits<uint8, detail::NEON>::ExternalType
load_neon_vector_aligned(const uint8* buffer) {
  return vld1q_u8(buffer);
}

template <>
inline typename detail::SimdTraits<uint16, detail::NEON>::ExternalType
load_neon_vector_aligned(const uint16* buffer) {
  return vld1q_u16(buffer);
}

template <>
inline typename detail::SimdTraits<uint32, detail::NEON>::ExternalType
load_neon_vector_aligned(const uint32* buffer) {
  return vld1q_u32(buffer);
}

template <>
inline typename detail::SimdTraits<uint64, detail::NEON>::ExternalType
load_neon_vector_aligned(const uint64* buffer) {
  return vld1q_u64(reinterpret_cast<const uint64_t*>(buffer));
}

template <>
inline typename detail::SimdTraits<float, detail::NEON>::ExternalType
load_neon_vector_aligned(const float* buffer) {
  return vld1q_f32(buffer);
}

template <>
inline typename detail::SimdTraits<double, detail::NEON>::ExternalType
load_neon_vector_aligned(const double* buffer) {
  return vld1q_f64(buffer);
}

// TODO(dimsum): Optimize horizontal_sum().
// llvm.experimental.vector.reduce.add.* already lowers to single instructions
// like addv, but I'm not sure if there are intrinsics for them.
}  // namespace detail

template <typename T>
using NativeSimd = Simd<T, detail::NEON>;

template <typename T>
using Simd128 = Simd<T, detail::NEON>;

template <typename T>
using Simd64 = Simd<T, detail::HalfNEON>;

template <>
inline Simd<int8, detail::NEON> abs(Simd<int8, detail::NEON> simd) {
  return vabsq_s8(simd);
}

template <>
inline Simd<int16, detail::NEON> abs(Simd<int16, detail::NEON> simd) {
  return vabsq_s16(simd);
}

template <>
inline Simd<int32, detail::NEON> abs(Simd<int32, detail::NEON> simd) {
  return vabsq_s32(simd);
}

template <>
inline Simd<int64, detail::NEON> abs(Simd<int64, detail::NEON> simd) {
  return vabsq_s64(simd);
}

template <>
inline Simd<float, detail::NEON> abs(Simd<float, detail::NEON> simd) {
  return vabsq_f32(simd);
}

template <>
inline Simd<double, detail::NEON> abs(Simd<double, detail::NEON> simd) {
  return vabsq_f64(simd);
}

template <>
inline Simd<float, detail::NEON> reciprocal_estimate(
    Simd<float, detail::NEON> simd) {
  return vrecpeq_f32(simd);
}

template <>
inline Simd<float, detail::NEON> sqrt(Simd<float, detail::NEON> simd) {
  return vsqrtq_f32(simd);
}

template <>
inline Simd<double, detail::NEON> sqrt(Simd<double, detail::NEON> simd) {
  return vsqrtq_f64(simd);
}

template <>
inline Simd<float, detail::NEON> reciprocal_sqrt_estimate(
    Simd<float, detail::NEON> simd) {
  return vrsqrteq_f32(simd);
}

template <>
inline Simd<int8, detail::NEON> add_saturated(Simd<int8, detail::NEON> lhs,
                                              Simd<int8, detail::NEON> rhs) {
  return vqaddq_s8(lhs, rhs);
}

template <>
inline Simd<uint8, detail::NEON> add_saturated(Simd<uint8, detail::NEON> lhs,
                                               Simd<uint8, detail::NEON> rhs) {
  return vqaddq_u8(lhs, rhs);
}

template <>
inline Simd<int16, detail::NEON> add_saturated(Simd<int16, detail::NEON> lhs,
                                               Simd<int16, detail::NEON> rhs) {
  return vqaddq_s16(lhs, rhs);
}

template <>
inline Simd<uint16, detail::NEON> add_saturated(
    Simd<uint16, detail::NEON> lhs, Simd<uint16, detail::NEON> rhs) {
  return vqaddq_u16(lhs, rhs);
}

template <>
inline Simd<int8, detail::NEON> sub_saturated(Simd<int8, detail::NEON> lhs,
                                              Simd<int8, detail::NEON> rhs) {
  return vqsubq_s8(lhs, rhs);
}

template <>
inline Simd<uint8, detail::NEON> sub_saturated(Simd<uint8, detail::NEON> lhs,
                                               Simd<uint8, detail::NEON> rhs) {
  return vqsubq_u8(lhs, rhs);
}

template <>
inline Simd<int16, detail::NEON> sub_saturated(Simd<int16, detail::NEON> lhs,
                                               Simd<int16, detail::NEON> rhs) {
  return vqsubq_s16(lhs, rhs);
}

template <>
inline Simd<uint16, detail::NEON> sub_saturated(
    Simd<uint16, detail::NEON> lhs, Simd<uint16, detail::NEON> rhs) {
  return vqsubq_u16(lhs, rhs);
}

template <>
inline Simd<int8, detail::NEON> min(Simd<int8, detail::NEON> lhs,
                                    Simd<int8, detail::NEON> rhs) {
  return vminq_s8(lhs, rhs);
}

template <>
inline Simd<int16, detail::NEON> min(Simd<int16, detail::NEON> lhs,
                                     Simd<int16, detail::NEON> rhs) {
  return vminq_s16(lhs, rhs);
}

template <>
inline Simd<int32, detail::NEON> min(Simd<int32, detail::NEON> lhs,
                                     Simd<int32, detail::NEON> rhs) {
  return vminq_s32(lhs, rhs);
}

template <>
inline Simd<uint8, detail::NEON> min(Simd<uint8, detail::NEON> lhs,
                                     Simd<uint8, detail::NEON> rhs) {
  return vminq_u8(lhs, rhs);
}

template <>
inline Simd<uint16, detail::NEON> min(Simd<uint16, detail::NEON> lhs,
                                      Simd<uint16, detail::NEON> rhs) {
  return vminq_u16(lhs, rhs);
}

template <>
inline Simd<uint32, detail::NEON> min(Simd<uint32, detail::NEON> lhs,
                                      Simd<uint32, detail::NEON> rhs) {
  return vminq_u32(lhs, rhs);
}

template <>
inline Simd<float, detail::NEON> min(Simd<float, detail::NEON> lhs,
                                     Simd<float, detail::NEON> rhs) {
  return vminq_f32(lhs, rhs);
}

template <>
inline Simd<double, detail::NEON> min(Simd<double, detail::NEON> lhs,
                                      Simd<double, detail::NEON> rhs) {
  return vminq_f64(lhs, rhs);
}

template <>
inline Simd<int8, detail::NEON> max(Simd<int8, detail::NEON> lhs,
                                    Simd<int8, detail::NEON> rhs) {
  return vmaxq_s8(lhs, rhs);
}

template <>
inline Simd<int16, detail::NEON> max(Simd<int16, detail::NEON> lhs,
                                     Simd<int16, detail::NEON> rhs) {
  return vmaxq_s16(lhs, rhs);
}

template <>
inline Simd<int32, detail::NEON> max(Simd<int32, detail::NEON> lhs,
                                     Simd<int32, detail::NEON> rhs) {
  return vmaxq_s32(lhs, rhs);
}

template <>
inline Simd<uint8, detail::NEON> max(Simd<uint8, detail::NEON> lhs,
                                     Simd<uint8, detail::NEON> rhs) {
  return vmaxq_u8(lhs, rhs);
}

template <>
inline Simd<uint16, detail::NEON> max(Simd<uint16, detail::NEON> lhs,
                                      Simd<uint16, detail::NEON> rhs) {
  return vmaxq_u16(lhs, rhs);
}

template <>
inline Simd<uint32, detail::NEON> max(Simd<uint32, detail::NEON> lhs,
                                      Simd<uint32, detail::NEON> rhs) {
  return vmaxq_u32(lhs, rhs);
}

template <>
inline Simd<float, detail::NEON> max(Simd<float, detail::NEON> lhs,
                                     Simd<float, detail::NEON> rhs) {
  return vmaxq_f32(lhs, rhs);
}

template <>
inline Simd<double, detail::NEON> max(Simd<double, detail::NEON> lhs,
                                      Simd<double, detail::NEON> rhs) {
  return vmaxq_f64(lhs, rhs);
}

template <>
inline Simd<int8, detail::NEON> pack_saturated(Simd<int16, detail::NEON> lhs,
                                               Simd<int16, detail::NEON> rhs) {
  return vcombine_s8(vqmovn_s16(lhs), vqmovn_s16(rhs));
}

template <>
inline Simd<int16, detail::NEON> pack_saturated(Simd<int32, detail::NEON> lhs,
                                                Simd<int32, detail::NEON> rhs) {
  return vcombine_s16(vqmovn_s32(lhs), vqmovn_s32(rhs));
}

template <>
inline Simd<uint8, detail::NEON> packu_saturated(
    Simd<int16, detail::NEON> lhs, Simd<int16, detail::NEON> rhs) {
  return vcombine_u8(vqmovun_s16(lhs), vqmovun_s16(rhs));
}

template <>
inline Simd<uint16, detail::NEON> packu_saturated(
    Simd<int32, detail::NEON> lhs, Simd<int32, detail::NEON> rhs) {
  return vcombine_u16(vqmovun_s32(lhs), vqmovun_s32(rhs));
}

// An alternative implementation of the SSE intrinsic function _mm_madd_epi16
// on ARM. It breaks a Simd object into the low and high parts. Then values in
// each part are multiplied and summed pairwisely before being concatenated.
template <>
inline Simd<int32, detail::NEON> mul_sum(Simd<int16, detail::NEON> lhs,
                                         Simd<int16, detail::NEON> rhs,
                                         Simd<int32, detail::NEON> acc) {
  int16x8_t lhs_raw = lhs;
  int16x8_t rhs_raw = rhs;
  int32x4_t mullo = vmull_s16(vget_low_s16(lhs_raw), vget_low_s16(rhs_raw));
  int32x4_t mulhi = vmull_s16(vget_high_s16(lhs_raw), vget_high_s16(rhs_raw));
  int32x2_t addlo = vpadd_s32(vget_low_s32(mullo), vget_high_s32(mullo));
  int32x2_t addhi = vpadd_s32(vget_low_s32(mulhi), vget_high_s32(mulhi));
  return vpaddq_s32(acc, vcombine_s32(addlo, addhi));
}

// vrndnq_f{32,64} translate to VRINTN.F{16,32}, which round floating points
// using the round-to-even rule (Round to Nearest rounding mode in ARM
// parlance).
template <>
inline Simd<float, detail::NEON> round(Simd<float, detail::NEON> simd) {
  return vrndnq_f32(simd);
}

template <>
inline Simd<double, detail::NEON> round(Simd<double, detail::NEON> simd) {
  return vrndnq_f64(simd);
}

template <>
inline Simd<int32, detail::NEON> round_to_integer(
    Simd<float, detail::NEON> simd) {
  return vcvtnq_s32_f32(simd);
}

template <typename T>
Simd<ScaleBy<T, 2>, detail::NEON> mul_widened(Simd<T, detail::HalfNEON> lhs,
                                              Simd<T, detail::HalfNEON> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

}  // namespace dimsum
