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
#pragma GCC diagnostic ignored "-Wpsabi"
#endif

#include "simd"

#ifdef DIMSUM_USE_SIMULATED
#include "operations.h"
#include "types.h"
#else
# if defined(DIMSUM_USE_ARM)
#  if defined(__aarch64__) && defined(__ARM_NEON)
// TODO(maskray) remove defined(DIMSUM_USE_ARM) after ARM NEON support is
// production ready.
#   include "simd_neon.h"
#  else
#   error "DIMSUM_USE_ARM is defined, but target doesn't support ARMv8 NEON."
#  endif
# elif defined(__SSE3__)
#  include "simd_sse.h"
#  ifdef __AVX2__
#   include "simd_avx.h"
#  endif  // __AVX2__
# elif defined(__VSX__)
#  include "simd_vsx.h"
# else
#  error \
    "Dimsum doesn't support this target. You may try emulation mode by defining the macro DIMSUM_USE_SIMULATED before including the header, but emulation mode is as good as loops over scalars can get." // NOLINT
# endif
#endif  // DIMSUM_USE_SIMULATED

#endif  // DIMSUM_DIMSUM_H_
