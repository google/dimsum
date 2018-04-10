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

#ifndef DIMSUM_SIMULATED_H_
#define DIMSUM_SIMULATED_H_

#include <climits>
#include <cmath>

#include <array>
#include <limits>

#include "operations.h"
#include "types.h"

namespace dimsum {
namespace simulated {

// dimsum::simulated contains two kinds of functions:
// 1. portable implementation (for loops) of dimsum::* SIMD functions.
// 2. portable implementation of dimsum::x86::* (x86 intrinsic functions). We
// prefer using non-x86 intrinsics to simulate x86 intrinsics. This serves as a
// cross validation test and a fallback when the non-x86 intrinsics versions are
// not available.

// Refers to dimsum.h for functionality.

template <size_t... indices, typename T, typename Abi>
Simd<T, Abi> shuffle(const Simd<T, Abi> lhs, const Simd<T, Abi> rhs = {}) {
  T a[lhs.size()];
  int i = 0;
  for (auto index : {indices...}) {
    a[i++] = index < lhs.size() ? lhs[index] : rhs[index - lhs.size()];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
typename std::enable_if<std::is_integral<T>::value, Simd<T, Abi>>::type abs(
    Simd<T, Abi> simd) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    if (simd[i] < 0) {
      a[i] = -simd[i];
    } else {
      a[i] = simd[i];
    }
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
typename std::enable_if<std::is_floating_point<T>::value, Simd<T, Abi>>::type
abs(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = std::abs(simd[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> negate(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = -simd[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> reciprocal_estimate(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = static_cast<T>(1) / simd[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> sqrt(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = std::sqrt(simd[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> reciprocal_sqrt_estimate(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = static_cast<T>(1) / std::sqrt(simd[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

// On x86, ARM and Power, the default floating point environment uses the
// round-to-even mode and nearbyint rounds a floating point to nearest integer
// (choosing the even one on a tie).
template <typename T, typename Abi>
Simd<T, Abi> round(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = std::nearbyint(simd[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> fma(Simd<T, Abi> a, Simd<T, Abi> b, Simd<T, Abi> c) {
  static_assert(std::is_floating_point<T>::value,
                "Only floating point types are supported");
  T s[a.size()];
  for (size_t i = 0; i < a.size(); i++) {
    s[i] = std::fma(a[i], b[i], c[i]);
  }
  return Simd<T, Abi>(s, flags::element_aligned);
}

template <typename Dest, typename T, typename Abi>
Simd<Dest, Abi> round_to_integer(Simd<T, Abi> simd) {
  Dest a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = static_cast<Dest>(std::nearbyint(simd[i]));
  }
  return Simd<Dest, Abi>(a, flags::element_aligned);
}

template <typename Dest, typename T, typename Abi>
Simd<Dest, Abi> static_simd_cast(Simd<T, Abi> simd) {
  Dest a[simd.size()];
  for (int i = 0; i < simd.size(); i++) {
    a[i] = static_cast<Dest>(simd[i]);
  }
  return Simd<Dest, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> add(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] + rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> add_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::SaturatedAdd(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> sub(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] - rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> sub_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::SaturatedSub(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> mul(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] * rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> shl_simd(Simd<T, Abi> simd, Simd<T, Abi> count) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = simd[i] << count[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> shl(Simd<T, Abi> simd, int count) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = simd[i] << count;
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> shr_simd(Simd<T, Abi> simd, Simd<T, Abi> count) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = simd[i] >> count[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> shr(Simd<T, Abi> simd, int count) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = simd[i] >> count;
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> bit_and(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] & rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> bit_or(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] | rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> bit_xor(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = lhs[i] ^ rhs[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> bit_not(Simd<T, Abi> simd) {
  T a[simd.size()];
  for (size_t i = 0; i < simd.size(); i++) {
    a[i] = ~simd[i];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_eq(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] == rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_ne(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] != rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_lt(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] < rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_le(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] <= rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_gt(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] > rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::ToUnsigned<T>, Abi> cmp_ge(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  detail::ToUnsigned<T> a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] >= rhs[i] ? ~0 : 0;
  }
  return Simd<detail::ToUnsigned<T>, Abi>(a, flags::element_aligned);
}

template <typename NewType, size_t NewSize, typename T, typename Abi>
ResizeTo<Simd<NewType, Abi>, NewSize> reduce_add(Simd<T, Abi> simd) {
  ResizeTo<Simd<NewType, Abi>, NewSize> ret = {};
  constexpr size_t kArity = simd.size() / NewSize;
  for (size_t i = 0; i < simd.size(); i++) {
    ret[i / kArity] = ret[i / kArity] + simd[i];
  }
  return ret;
}

template <typename T, typename Abi>
ResizeTo<Simd<T, Abi>, 1> reduce_add(Simd<T, Abi> simd) {
  return simulated::reduce_add<T, 1>(simd);
}

template <typename T, typename Abi>
T hmin(const Simd<T, Abi>& simd) {
  T ret = simd[0];
  for (size_t i = 1; i < simd.size(); i++)
    ret = std::min(ret, simd[i]);
  return ret;
}

template <typename T, typename Abi>
T hmax(const Simd<T, Abi>& simd) {
  T ret = simd[0];
  for (size_t i = 1; i < simd.size(); i++)
    ret = std::max(ret, simd[i]);
  return ret;
}

// TODO(maskray) Remove `= void`.
template <typename Dest = void, typename T, typename Abi>
ReinterpretTo<Simd<T, Abi>, Dest> mul_sum(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs,
    ReinterpretTo<Simd<T, Abi>, Dest> acc = 0) {
  using Dest1 = typename std::conditional<std::is_void<Dest>::value,
                                          ScaleBy<T, 2>, Dest>::type;
  decltype(acc) ret{};
  size_t way = lhs.size() / acc.size();
  for (size_t i = 0; i < acc.size(); i++) {
    // Cast intermediate result to unsigned to avoid addition overflow.
    // e.g. For 2-way mul_sum of int32's: INT32_MIN * INT32_MIN + INT32_MIN *
    // INT32_MIN = 2**63 is not representable by int64.
    typename std::make_unsigned<Dest1>::type t = acc[i];
    for (size_t j = 0; j < way; j++)
      t += Dest1{lhs[i * way + j]} * rhs[i * way + j];
    ret[i] = t;
  }
  return ret;
}

template <typename Abi>
ResizeBy<Simd<int16, Abi>, 1, 2> maddubs(Simd<uint8, Abi> lhs,
                                         Simd<int8, Abi> rhs) {
  int16 a[lhs.size() / 2];
  for (size_t i = 0; i < lhs.size(); i += 2) {
    a[i / 2] = detail::SaturatedAdd<int16>(int16{lhs[i]} * rhs[i],
                                           int16{lhs[i + 1]} * rhs[i + 1]);
  }
  return ResizeBy<Simd<int16, Abi>, 1, 2>(a, flags::element_aligned);
}

template <typename T, typename Abi>
ScaleElemBy<Simd<T, Abi>, 2> mul_widened(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  ScaleElemBy<Simd<T, Abi>, 2> ret;
  for (size_t i = 0; i < lhs.size(); i++) {
    ScaleBy<T, 2> tmp = lhs[i];
    tmp *= rhs[i];
    ret[i] = tmp;
  }
  return ret;
}

template <typename T, typename Abi>
Simd<T, Abi> max(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = std::max(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<T, Abi> min(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = std::min(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

// TODO(maskray) {int,uint}{8,32,64} specializations are provided.
// Provide float/double specializations if there are use cases to check sign
// bits
template <typename T, typename Abi>
int movemask(Simd<T, Abi> simd) {
  static_assert(std::is_integral<T>::value,
                "Only integral types are supported");
  int res = 0;
  for (size_t i = 0; i < simd.size(); i++) {
    res |= (simd[i] >> (sizeof(T) * CHAR_BIT - 1) & 1) << i;
  }
  return res;
}

}  // namespace simulated
}  // namespace dimsum

#endif  // DIMSUM_SIMULATED_H_
