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

#ifndef DIMSUM_DIMSUM_H_
#define DIMSUM_DIMSUM_H_

#include "simd.h"

// The supported specializations of Simd types. InternalType means the actually
// stored type. ExternalType means the convertible type that the user can use to
// construct from and extract to.
#define SIMD_SPECIALIZATION(T, STORAGE, NUM_BYTES, EXTERNAL_TYPE)  \
  template <>                                                      \
  struct SimdTraits<T, detail::Abi<STORAGE, NUM_BYTES>> {          \
    static_assert(NUM_BYTES % sizeof(T) == 0, "");                 \
    using InternalType = detail::GccVecTraits<T, NUM_BYTES>::type; \
    using ExternalType = EXTERNAL_TYPE;                            \
  };

#define SIMD_NON_NATIVE_SPECIALIZATION(STORAGE, NUM_BYTES)      \
  SIMD_SPECIALIZATION(int8, STORAGE, NUM_BYTES, InternalType)   \
  SIMD_SPECIALIZATION(int16, STORAGE, NUM_BYTES, InternalType)  \
  SIMD_SPECIALIZATION(int32, STORAGE, NUM_BYTES, InternalType)  \
  SIMD_SPECIALIZATION(int64, STORAGE, NUM_BYTES, InternalType)  \
  SIMD_SPECIALIZATION(uint8, STORAGE, NUM_BYTES, InternalType)  \
  SIMD_SPECIALIZATION(uint16, STORAGE, NUM_BYTES, InternalType) \
  SIMD_SPECIALIZATION(uint32, STORAGE, NUM_BYTES, InternalType) \
  SIMD_SPECIALIZATION(uint64, STORAGE, NUM_BYTES, InternalType) \
  SIMD_SPECIALIZATION(float, STORAGE, NUM_BYTES, InternalType)  \
  SIMD_SPECIALIZATION(double, STORAGE, NUM_BYTES, InternalType)

#define SIMD_NON_NATIVE_SPECIALIZATION_ALL_SMALL_BYTES(STORAGE) \
  SIMD_SPECIALIZATION(int8, STORAGE, 4, InternalType)           \
  SIMD_SPECIALIZATION(int16, STORAGE, 4, InternalType)          \
  SIMD_SPECIALIZATION(int32, STORAGE, 4, InternalType)          \
  SIMD_SPECIALIZATION(uint8, STORAGE, 4, InternalType)          \
  SIMD_SPECIALIZATION(uint16, STORAGE, 4, InternalType)         \
  SIMD_SPECIALIZATION(uint32, STORAGE, 4, InternalType)         \
  SIMD_SPECIALIZATION(float, STORAGE, 4, InternalType)          \
  SIMD_SPECIALIZATION(int8, STORAGE, 2, InternalType)           \
  SIMD_SPECIALIZATION(int16, STORAGE, 2, InternalType)          \
  SIMD_SPECIALIZATION(uint8, STORAGE, 2, InternalType)          \
  SIMD_SPECIALIZATION(uint16, STORAGE, 2, InternalType)         \
  SIMD_SPECIALIZATION(int8, STORAGE, 1, InternalType)           \
  SIMD_SPECIALIZATION(uint8, STORAGE, 1, InternalType)

#ifdef DIMSUM_USE_SIMULATED
# include "simulated_impl-inl.inc"
#else
# if defined(__SSE4_1__)
#  include "x86_sse_impl-inl.inc"
#  ifdef __AVX2__
#   include "x86_avx_impl-inl.inc"
#  endif  // __AVX2__
# elif defined(__ALTIVEC__)
#  include "ppc_impl-inl.inc"
// TODO(maskray) remove defined(USE_DIMSUM_ARM) after ARM NEON support is
// production ready.
# elif defined(__aarch64__) && defined(__ARM_NEON) && defined(USE_DIMSUM_ARM)
#  include "arm_impl-inl.inc"
# else
#  include "simulated_impl-inl.inc"
# endif  // defined(__SSE4_1__)
#endif  // DIMSUM_USE_SIMULATED

#undef SIMD_SPECIALIZATION
#undef DIMSUM_DELETE

#endif  // DIMSUM_DIMSUM_H_
