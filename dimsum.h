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

// On x86, when __attribute__((vector_size(32))) types are used but -mavx is not
// enabled. GCC warnings "AVX vector return without AVX enabled changes the ABI
// [-Werror=psabi]\nThe ABI for passing parameters with 32-byte alignment has
// changed in GCC 4.6"
// We have declarations of SIMD_NON_NATIVE_SPECIALIZATION of 32 bytes and
// larger, which triggers the warning. GCC < 4.6 is not supported by dimsum,
// so the warning can be safely ignored.
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"
#endif

#ifdef DIMSUM_USE_SIMULATED
# include "simd_simulated.h"
#else
# if defined(__aarch64__) && defined(__ARM_NEON) && defined(USE_DIMSUM_ARM)
// TODO(maskray) remove defined(USE_DIMSUM_ARM) after ARM NEON support is
// production ready.
#  include "simd_neon.h"
# elif defined(__SSE4_1__)
#  include "simd_sse.h"
#  ifdef __AVX2__
#   include "simd_avx.h"
#  endif  // __AVX2__
# elif defined(__VSX__)
#  include "simd_vsx.h"
# else
#  include "simd_simulated.h"
# endif
#endif  // DIMSUM_USE_SIMULATED

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif  // DIMSUM_DIMSUM_H_
