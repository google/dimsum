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

#ifndef DIMSUM_OPERATIONS_H_
#define DIMSUM_OPERATIONS_H_

#include <cmath>
#include <cstdint>
#include <cstring>

#include <array>
#include <functional>
#include <limits>

#include "port.h"
#include "types.h"

#ifdef __clang__
#define DIMSUM_UNROLL _Pragma("unroll")
#else
#define DIMSUM_UNROLL
#endif

namespace dimsum {
namespace detail {

// Returns a DestType with the value clamped into its representable range.
template <typename DestType, typename SrcType>
DestType saturated_convert(SrcType val) {
  return val > std::numeric_limits<DestType>::max()
             ? std::numeric_limits<DestType>::max()
             : (val < std::numeric_limits<DestType>::min()
                    ? std::numeric_limits<DestType>::min()
                    : static_cast<DestType>(val));
}

template <typename To, typename From>
constexpr auto IsNarrowingConversionImpl(From a[[gnu::unused]])
    -> decltype(To{a}, false) {
  return false;
}

template <typename To>
constexpr bool IsNarrowingConversionImpl(...) {
  return true;
}

template <typename To, typename From>
constexpr bool IsNarrowingConversion() {
  static_assert(
      std::is_arithmetic<To>::value && std::is_arithmetic<From>::value, "");
  return IsNarrowingConversionImpl<To>(From{});
}

template <typename T, typename Op>
constexpr bool IsReduceAdd() {
  return std::is_integral<T>::value &&
         (std::is_same<Op, std::plus<T>>::value ||
          std::is_same<Op, std::plus<void>>::value);
}

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

using std::experimental::all_of;
using std::experimental::concat;
using std::experimental::hmax;
using std::experimental::hmin;
using std::experimental::max;
using std::experimental::min;
using std::experimental::reduce;
using std::experimental::simd_cast;
using std::experimental::split_by;
using std::experimental::static_simd_cast;
using std::experimental::to_fixed_size;

// Unlike p0214r5, we don't provide compatible for now. Anything related to
// cross-module calling should be interfaced with a template ABI type, e.g.:
//   template <typename T, typename Abi>
//   void PublicFunction(Simd<T, Abi> a);

// Most functions defined here are generic. Functions
// marked with `= delete` are specialized in separate simd_*.h files for
// different platforms. Others defined inside the class (e.g. abs()) may provide
// a default implementation for common types and have
// instantiations in simd_*.h files.
//
// TODO(timshen): change all pass-by-value parameters to pass-by-const-ref, to
// be compliant to P0214.
//
// ----------------- Primitive Operations -----------------
// Returns r, where r[i] = simd[indices[i]].
template <size_t... indices, typename T, typename SrcAbi>
ResizeTo<Simd<T, SrcAbi>, sizeof...(indices)> shuffle(Simd<T, SrcAbi> simd) {
  return std::experimental::__simd_shuffle<indices...>(simd, simd);
}

// Example: shuffle<5, 1, 6, -1>({0,1,2,3}, {4,5,6,7}) returns {5,1,6,any}.
template <size_t... indices, typename T, typename SrcAbi>
ResizeTo<Simd<T, SrcAbi>, sizeof...(indices)> shuffle(Simd<T, SrcAbi> lhs,
                                                      Simd<T, SrcAbi> rhs) {
  return shuffle<indices...>(concat(lhs, rhs));
}

namespace detail {

template <size_t... indices, typename T, typename Abi>
ResizeBy<Simd<T, Abi>, 2> zip_impl(Simd<T, Abi> lhs, Simd<T, Abi> rhs,
                                   dimsum::index_sequence<indices...>) {
  constexpr auto size = Simd<T, Abi>::size();
  return dimsum::shuffle<(indices / 2 + indices % 2 * size)...>(lhs, rhs);
}

}  // namespace detail

// Returns the element-wise saturated sum of two Simd objects.
template <typename T, typename Abi>
Simd<T, Abi> add_saturated(const Simd<T, Abi> lhs, const Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::SaturatedAdd(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

// Returns the element-wise saturated difference of two Simd objects.
template <typename T, typename Abi>
Simd<T, Abi> sub_saturated(const Simd<T, Abi> lhs, const Simd<T, Abi> rhs) {
  T a[lhs.size()];
  for (size_t i = 0; i < lhs.size(); i++) {
    a[i] = detail::SaturatedSub(lhs[i], rhs[i]);
  }
  return Simd<T, Abi>(a, flags::element_aligned);
}

namespace detail {

template <size_t kArity>
struct ReduceAddImpl;

template <>
struct ReduceAddImpl<1> {
  template <typename T, typename Abi>
  static Simd<T, Abi> Apply(Simd<T, Abi> simd) {
    return simd;
  }
};

template <>
struct ReduceAddImpl<2> {
  template <typename SimdType, size_t... indices>
  static ResizeBy<SimdType, 1, 2> ApplyImpl(
      SimdType simd, dimsum::index_sequence<indices...>) {
    return shuffle<(2 * indices)...>(simd) +
           shuffle<(2 * indices + 1)...>(simd);
  }

  template <typename SimdType>
  static ResizeBy<SimdType, 1, 2> Apply(SimdType simd) {
    return ApplyImpl(simd, dimsum::make_index_sequence<simd.size() / 2>{});
  }
};

template <size_t kArity>
struct ReduceAddImpl {
  template <typename SimdType>
  static ResizeBy<SimdType, 1, kArity> Apply(SimdType simd) {
    return ReduceAddImpl<kArity / 2>::Apply(ReduceAddImpl<2>::Apply(simd));
  }
};

}  // namespace detail

// Returns the element-wise, widening multiplication. For input
// Simd<(u)intn>, returns a Simd object with element type (u)int2n, and with the
// same amount of elements as lhs/rhs.
template <typename T, typename Abi>
ScaleElemBy<Simd<T, Abi>, 2> mul_widened(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return simd_cast<ScaleBy<T, 2>>(lhs) * simd_cast<ScaleBy<T, 2>>(rhs);
}

// Returns the element-wise estimate of reciprocal.
// On x86, the relative error is less or equal than 1.5/4096.
// On POWER, the relative error is less or equal than 1/16384.
// On ARM, the relative error may be slightly greater than 1/512.
template <typename T, typename Abi>
Simd<T, Abi> reciprocal_estimate(Simd<T, Abi> v) {
  Simd<T, Abi> ret;
  for (int i = 0; i < v.size(); i++) {
    ret[i] = T(1) / v[i];
  }
  return ret;
}

// Returns the element-wise square root.
template <typename T, typename Abi>
Simd<T, Abi> sqrt(Simd<T, Abi> v) {
  Simd<T, Abi> ret;
  for (int i = 0; i < v.size(); i++) {
    ret[i] = std::sqrt(v[i]);
  }
  return ret;
}

// Returns the element-wise estimate of reciprocal square root.
template <typename T, typename Abi>
Simd<T, Abi> reciprocal_sqrt_estimate(Simd<T, Abi> v) {
  Simd<T, Abi> ret;
  for (int i = 0; i < v.size(); i++) {
    ret[i] = T(1) / std::sqrt(v[i]);
  }
  return ret;
}

// Element-wise round to integral floating points using the round-toeven rule.
// On x86 and ARM, specific instructions are used to guarantee round-to-even.
// On Power, vec_rint is used which rounds floating points according to Rounding
// Control field RN in FPSCR, which defaults to round-to-even.
template <typename T, typename Abi>
Simd<T, Abi> round(Simd<T, Abi> simd) {
  Simd<T, Abi> ret;
  for (size_t i = 0; i < simd.size(); i++) {
    ret[i] = std::nearbyint(simd[i]);
  }
  return ret;
}

// Casts to another vector type of the same width without changing any bits.
template <typename Dest, typename T, typename Abi>
ReinterpretTo<Simd<T, Abi>, Dest> bit_cast(Simd<T, Abi> simd) {
  ReinterpretTo<Simd<T, Abi>, Dest> ret;
  T src_arr[simd.size()];
  simd.copy_to(src_arr, flags::element_aligned_tag());
  Dest dest_arr[ret.size()];
  static_assert(sizeof(dest_arr) == sizeof(src_arr), "");
  memcpy(dest_arr, src_arr, sizeof(dest_arr));
  return ReinterpretTo<Simd<T, Abi>, Dest>(dest_arr,
                                           flags::element_aligned_tag());
}

// Convert input Simd or SimdMask to the non-portable native type.
template <typename T, typename Abi>
typename Simd<T, Abi>::__native_type to_raw(Simd<T, Abi> v) {
  return v.__raw();
}

template <typename T, typename Abi>
typename SimdMask<T, Abi>::__native_type to_raw(SimdMask<T, Abi> v) {
  return v.__raw();
}

// Returns the element-wise comparison result.
// Each element in the result is 0 for false and non-zero for true.
// The result in each lane is undefined if either of the argument is NaN.
// TODO(maskray) define NaN comparison if it is needed.
//
// On x86_64, powerpc64le and aarch64, there is a stronger commitment that
// each element is either 0 for false or ~0 for true.
//
// For integral element types (e.g. int16), the result element type is the
// corresponding unsigned type (e.g. Simd<uint16>).
// For floating point types, the result element type is the integral type
// with the exact width (float => uint32, double => uint64).
template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_eq(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] == rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_ne(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] != rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_lt(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] < rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_le(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] <= rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_gt(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] > rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, typename Abi>
Simd<detail::ToUnsigned<Tp>, Abi> cmp_ge(Simd<Tp, Abi> lhs, Simd<Tp, Abi> rhs) {
  using Unsigned = detail::ToUnsigned<Tp>;
  Simd<Unsigned, Abi> ret;
  for (int i = 0; i < ret.size(); i++) {
    ret[i] = lhs[i] >= rhs[i] ? ~Unsigned() : 0;
  }
  return ret;
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_eq(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  return to_raw(lhs == rhs);
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_ne(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  return to_raw(lhs != rhs);
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_lt(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  return to_raw(lhs < rhs);
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_le(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  return to_raw(lhs <= rhs);
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_gt(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  // Somehow GCC 4.9.4 on powerpc-linux-gnu decides to warn about "GCC vector
  // returned by reference: non-standard ABI extension with no compatibility
  // guarantee", which is not true. Use __raw instead of to_raw as a workaround.
  //
  // Also, surprisingly, GCC 4.9.4 only warns about cmp_gt, not other comparison
  // functions. It looks like a compiler bug.
  return (lhs > rhs).__raw();
}

template <typename Tp, int N>
NativeSimd<detail::ToUnsigned<Tp>, N> cmp_ge(NativeSimd<Tp, N> lhs,
                                             NativeSimd<Tp, N> rhs) {
  return to_raw(lhs >= rhs);
}

template <size_t N, typename T, typename Abi>
std::array<ResizeBy<Simd<T, Abi>, 1, N>, N> SplitBy(Simd<T, Abi> simd) {
  static_assert(simd.size() % N == 0, "");

  using ArrayElem = ResizeBy<Simd<T, Abi>, 1, N>;
  std::array<ArrayElem, N> ret;
  constexpr size_t size = ArrayElem::size();
  DIMSUM_UNROLL for (int i = 0; i < simd.size(); i++) {
    ret[i / size][i % size] = simd[i];
  }
  return ret;
}

// For input lhs[0], lhs[1], ..., lhs[n-1], rhs[0], rhs[1], ..., rhs[n-1],
// returns lhs[0], rhs[0], lhs[1], rhs[1], ..., lhs[n-1], rhs[n-1].
template <typename T, typename Abi>
ResizeBy<Simd<T, Abi>, 2> zip(Simd<T, Abi> lhs, Simd<T, Abi> rhs) {
  return detail::zip_impl(
      lhs, rhs, dimsum::make_index_sequence<Simd<T, Abi>::size() * 2>{});
}

// Returns the element-wise absolute value.
// The element type can be signed, unsigned or floating point.
// For signed element type, the result in each lane is
// undefined if it is the minimum value.
template <typename T, typename Abi>
typename std::enable_if<std::is_integral<T>::value, Simd<T, Abi>>::type abs(
    Simd<T, Abi> simd) {
  if (std::is_unsigned<T>::value) {
    return simd;
  }
  auto mask = cmp_lt(simd, Simd<T, Abi>(0));
  using Unsigned = detail::ToUnsigned<T>;
  return bit_cast<T>((bit_cast<Unsigned>(simd) ^ mask) - mask);
}

template <typename T, typename Abi>
typename std::enable_if<std::is_floating_point<T>::value, Simd<T, Abi>>::type
abs(Simd<T, Abi> simd) {
  using Unsigned = detail::ToUnsigned<T>;
  return bit_cast<T>(bit_cast<Unsigned>(simd) & bit_cast<Unsigned>(-simd));
}

// TODO(timshen): Add simd_cast_saturated.

// ----------------- Compound Operations -----------------

// Similar to static_simd_cast<Dest>(round(simd)), but possibly faster.
//
// Currently only Simd<int32, Abi> round_to_integer(Simd<float, Abi>) is
// accepted. On Power, vec_rint is used which rounds floating points according
// to Rounding Control field RN in FPSCR, which defaults to round-to-even.
template <typename Dest, typename T, typename Abi>
Simd<Dest, Abi> round_to_integer(Simd<T, Abi> v) {
  Simd<Dest, Abi> ret;
  for (int i = 0; i < v.size(); i++) {
    ret[i] = static_cast<Dest>(std::nearbyint(v[i]));
  }
  return ret;
}

// Partitions the input elements into NewSize groups. For each group, sums up
// the elements in it, and produces a NewType. Return all sums in a Simd object.
//
// Formally, for the return value ret,
//     ret[i] = sum(simd[i * (simd.size() / NewSize)
//                  ... (i + 1) * (simd.size() / NewSize)])
//
// If any overflow occur, the result is undefined.
template <typename NewType, size_t NewSize, typename T, typename Abi>
typename std::enable_if<std::is_same<NewType, T>::value,
                        ResizeTo<Simd<NewType, Abi>, NewSize>>::type
reduce_add(Simd<T, Abi> simd) {
  static_assert(simd.size() % NewSize == 0, "");
  static_assert(std::is_integral<T>::value,
                "The element types needs to be integrals.");
  static_assert(sizeof(NewType) >= sizeof(T),
                "reduce_add's result element is too small.");
  constexpr size_t kArity = simd.size() / NewSize;
  return detail::ReduceAddImpl<kArity>::Apply(simd);
}

template <typename NewType, size_t NewSize, typename T, typename Abi>
typename std::enable_if<!std::is_same<NewType, T>::value,
                        ResizeTo<Simd<NewType, Abi>, NewSize>>::type
reduce_add(Simd<T, Abi> simd) {
  return reduce_add<NewType, NewSize>(static_simd_cast<NewType>(simd));
}

// Equivalent to reduce_add<T, 1>.
template <typename T, typename Abi>
ResizeTo<Simd<T, Abi>, 1> reduce_add(Simd<T, Abi> simd) {
  return reduce_add<T, 1>(simd);
}

// Equivalent to acc + reduce_add<NewType, acc.size()>(mul_widened(lhs, rhs)),
// but probably faster. acc, lhs, and rhs must have the same number of bytes.
//
// Notice that the total number of bytes in acc is constraint to be the same as
// the total number of bytes in lhs/rhs.
//
// e.g. mul_sum<int32>(Simd128<int16>, Simd128<int16>, Simd128<int32>)
// computes 8 int16*int16 products ({s[0], ..., s[7]}) first, then does pairwise
// horizontal summation and gets {s[0]+s[1], s[2]+s[3], s[4]+s[5], s[6]+s[7]}),
// so that the result is still 128-bit, finally returns {acc[0]+s[0]+s[1],
// acc[1]+s[2]+s[3], acc[2]+s[4]+s[5], acc[3]+s[6]+s[7]}.
template <typename Dest, typename T, typename Abi>
ReinterpretTo<Simd<T, Abi>, Dest> mul_sum(
    Simd<T, Abi> lhs, Simd<T, Abi> rhs,
    ReinterpretTo<Simd<T, Abi>, Dest> acc = 0) {
  if (std::is_void<Dest>::value) {
    return mul_sum<typename std::conditional<std::is_void<Dest>::value,
                                             ScaleBy<T, 2>, Dest>::type>(
        lhs, rhs, acc);
  }
  return acc + reduce_add<typename decltype(acc)::value_type, acc.size()>(
                   mul_widened(lhs, rhs));
}

// Element-wise fused multiply-add a * b + c.
//
// If the target supports FMA, i.e.
//    defined(__FMA__) || defined(__VSX__) || defined(__ARM_FEATURE_FMA),
// clang 3.6/GCC 7.1 optimize the for loop into FMA.
template <typename T, typename Abi>
Simd<T, Abi> fma(Simd<T, Abi> a, Simd<T, Abi> b, Simd<T, Abi> c) {
  static_assert(std::is_floating_point<T>::value,
                "Only floating point types are supported");
  Simd<T, Abi> ret;
  for (size_t i = 0; i < a.size(); i++) ret[i] = std::fma(a[i], b[i], c[i]);
  return ret;
}

}  // namespace dimsum

#endif  // DIMSUM_OPERATIONS_H_
