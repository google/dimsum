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

#include <array>
#include <cmath>
#include <limits>

#include "dimsum.h"

namespace dimsum {
namespace detail {
enum class OverflowType {
  kNoOverflow,
  kPositiveOverflow,
  kNegativeOverflow,
};

template <typename T>
OverflowType CheckAddOverflow(T lhs, T rhs) {
  static_assert(std::is_integral<T>::value,
                "Only integral types are supported");
  if (lhs >= 0 && rhs >= 0) {
    if (lhs > std::numeric_limits<T>::max() - rhs)
      return OverflowType::kPositiveOverflow;
  } else if (lhs < 0 && rhs < 0) {
    if (lhs < std::numeric_limits<T>::min() - rhs)
      return OverflowType::kNegativeOverflow;
  }
  return OverflowType::kNoOverflow;
}

template <typename T>
OverflowType CheckSubOverflow(T lhs, T rhs) {
  static_assert(std::is_integral<T>::value,
                "Only integral types are supported");
  if (std::is_unsigned<T>::value || (lhs < 0 && rhs >= 0)) {
    if (lhs < std::numeric_limits<T>::min() + rhs)
      return OverflowType::kNegativeOverflow;
  } else if (lhs >= 0 && rhs < 0) {
    if (lhs > std::numeric_limits<T>::max() + rhs)
      return OverflowType::kPositiveOverflow;
  }
  return OverflowType::kNoOverflow;
}

template <typename T>
T SaturatedAdd(T lhs, T rhs) {
  switch (CheckAddOverflow(lhs, rhs)) {
    case OverflowType::kPositiveOverflow:
      return std::numeric_limits<T>::max();
    case OverflowType::kNegativeOverflow:
      return std::numeric_limits<T>::min();
    case OverflowType::kNoOverflow:
      return lhs + rhs;
  }
}

template <typename T>
T SaturatedSub(T lhs, T rhs) {
  switch (CheckSubOverflow(lhs, rhs)) {
    case OverflowType::kPositiveOverflow:
      return std::numeric_limits<T>::max();
    case OverflowType::kNegativeOverflow:
      return std::numeric_limits<T>::min();
    case OverflowType::kNoOverflow:
      return lhs - rhs;
  }
}
}  // namespace detail

namespace simulated {

// dimsum::simulated contains two kinds of functions:
// 1. portable implementation (for loops) of dimsum::* SIMD functions.
// 2. portable implementation of dimsum::x86::* (x86 intrinsic functions). We
// prefer using non-x86 intrinsics to simulate x86 intrinsics. This serves as a
// cross validation test and a fallback when the non-x86 intrinsics versions are
// not available.

// Refers to dimsum.h for functionality.

template <size_t... indices, typename T, typename Abi>
Simd<T, Abi> shuffle(Simd<T, Abi> lhs, Simd<T, Abi> rhs = {}) {
  T a[lhs.size()];
  int i = 0;
  for (auto index : {indices...}) {
    a[i++] = index < lhs.size() ? lhs[index] : rhs[index - lhs.size()];
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::Number<sizeof(T) / 2, detail::NumberKind::kSInt>, Abi>
pack_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  using ResultElement =
      detail::Number<sizeof(T) / 2, detail::NumberKind::kSInt>;
  ResultElement a[lhs.size() * 2];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::saturated_convert<ResultElement>(lhs[i]);
    a[i + lhs.size()] = detail::saturated_convert<ResultElement>(rhs[i]);
  }
  return Simd<ResultElement, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<detail::Number<sizeof(T) / 2, detail::NumberKind::kUInt>, Abi>
packu_saturated(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  using ResultElement =
      detail::Number<sizeof(T) / 2, detail::NumberKind::kUInt>;
  ResultElement a[lhs.size() * 2];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::saturated_convert<ResultElement>(lhs[i]);
    a[i + lhs.size()] = detail::saturated_convert<ResultElement>(rhs[i]);
  }
  return Simd<ResultElement, Abi>(a, flags::element_aligned);
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
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_eq(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] == rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_ne(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] != rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_lt(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] < rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_le(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] <= rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_gt(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] > rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <typename T, typename Abi>
Simd<typename Simd<T, Abi>::ComparisonResultType, Abi> cmp_ge(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  typename Simd<T, Abi>::ComparisonResultType a[lhs.size()];
  for (size_t i = 0; i < Simd<T, Abi>::size(); i++) {
    a[i] = lhs[i] >= rhs[i] ? ~0 : 0;
  }
  return Simd<typename Simd<T, Abi>::ComparisonResultType, Abi>(
      a, flags::element_aligned);
}

template <size_t kArity, typename T, typename Abi>
ResizeBy<Simd<T, Abi>, 1, kArity> reduce_add(Simd<T, Abi> simd) {
  ResizeBy<Simd<T, Abi>, 1, kArity> ret = {};
  for (size_t i = 0; i < simd.size(); i++) {
    ret.set(i / kArity, ret[i / kArity] + simd[i]);
  }
  return ret;
}

template <typename T, typename Abi>
ResizeTo<Simd<T, Abi>, 1> reduce_add(Simd<T, Abi> simd) {
  return simulated::reduce_add<simd.size()>(simd);
}

template <size_t kArity, typename T, typename Abi>
ResizeBy<ScaleElemBy<Simd<T, Abi>, kArity>, 1, kArity> reduce_add_widened(
    Simd<T, Abi> simd) {
  using DestType = ResizeBy<ScaleElemBy<Simd<T, Abi>, kArity>, 1, kArity>;
  DestType ret{};
  for (size_t i = 0; i < simd.size(); i++) {
    ret.set(i / kArity, ret[i / kArity] + simd[i]);
  }
  return ret;
}

template <typename T, typename Abi>
ResizeBy<ScaleElemBy<Simd<T, Abi>, 2>, 1, 2> mul_sum(Simd<T, Abi> lhs,
                                                     Simd<T, Abi> rhs) {
  using DestType = ResizeBy<ScaleElemBy<Simd<T, Abi>, 2>, 1, 2>;
  using DestElem = typename DestType::value_type;
  DestType ret{};
  // Cast intermediate result to unsigned because there is an add overflow case
  // for each signed type: e.g. INT32_MIN * INT32_MIN + INT32_MIN * INT32_MIN =
  // 2**63 is not representable by int64.
  for (size_t i = 0; i < lhs.size(); i += 2) {
    ret.set(i / 2, static_cast<typename std::make_unsigned<DestElem>::type>(
                       DestElem{lhs[i]} * rhs[i]) +
                       DestElem{lhs[i + 1]} * rhs[i + 1]);
  }
  return ret;
}

template <typename Abi>
Simd<int16, Abi> maddubs(Simd<uint8, Abi> lhs, Simd<int8, Abi> rhs) {
  int16 a[lhs.size() / 2];
  for (size_t i = 0; i < lhs.size(); i += 2) {
    a[i / 2] = detail::SaturatedAdd<int16>(int16{lhs[i]} * rhs[i],
                                           int16{lhs[i + 1]} * rhs[i + 1]);
  }
  return Simd<int16, Abi>(a, flags::element_aligned);
}

template <typename T, typename Abi>
ScaleElemBy<Simd<T, Abi>, 2> mul_widened(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  ScaleElemBy<Simd<T, Abi>, 2> ret;
  for (size_t i = 0; i < lhs.size(); i++) {
    ScaleBy<T, 2> tmp = lhs[i];
    tmp *= rhs[i];
    ret.set(i, tmp);
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
