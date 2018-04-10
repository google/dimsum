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

#ifndef DIMSUM_TYPES_H_
#define DIMSUM_TYPES_H_

#include <cstddef>

#include <type_traits>

#include "index_sequence.h"
#include "port.h"
#include "simd"

namespace dimsum {
namespace detail {

// Different kinds of supported elements.
enum class NumberKind {
  kUInt,
  kSInt,
  kFloat,
};

// Returns the NumberKind of T.
template <typename T>
constexpr NumberKind get_number_kind() {
  static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                "Unexpected type");
  return std::is_signed<T>::value
             ? NumberKind::kSInt
             : (std::is_unsigned<T>::value ? NumberKind::kUInt
                                           : NumberKind::kFloat);
}

// NumberTraits maps (width, kind) to a primitive integer type.
template <size_t width, NumberKind kind>
struct NumberTraits;

template <>
struct NumberTraits<1, NumberKind::kSInt> {
  using type = int8;
};
template <>
struct NumberTraits<2, NumberKind::kSInt> {
  using type = int16;
};
template <>
struct NumberTraits<4, NumberKind::kSInt> {
  using type = int32;
};
template <>
struct NumberTraits<8, NumberKind::kSInt> {
  using type = int64;
};
template <>
struct NumberTraits<1, NumberKind::kUInt> {
  using type = uint8;
};
template <>
struct NumberTraits<2, NumberKind::kUInt> {
  using type = uint16;
};
template <>
struct NumberTraits<4, NumberKind::kUInt> {
  using type = uint32;
};
template <>
struct NumberTraits<8, NumberKind::kUInt> {
  using type = uint64;
};
template <>
struct NumberTraits<4, NumberKind::kFloat> {
  using type = float;
};
template <>
struct NumberTraits<8, NumberKind::kFloat> {
  using type = double;
};

template <size_t width, NumberKind kind>
using Number = typename NumberTraits<width, kind>::type;

template <typename T>
using ToUnsigned = detail::Number<sizeof(T), detail::NumberKind::kUInt>;

}  // namespace detail

namespace flags {

using std::experimental::element_aligned;
using std::experimental::element_aligned_tag;
using std::experimental::vector_aligned;
using std::experimental::vector_aligned_tag;

}  // namespace flags

template <typename T, typename Abi>
using Simd = std::experimental::simd<T, Abi>;

template <typename T, int N = std::experimental::native_simd<T>::size()>
using NativeSimd =
    std::experimental::simd<T, std::experimental::simd_abi::__native<T, N>>;

template <typename T>
using Simd64 = NativeSimd<T, 8 / sizeof(T)>;

template <typename T>
using Simd128 = NativeSimd<T, 16 / sizeof(T)>;

template <typename T>
using Simd256 = NativeSimd<T, 32 / sizeof(T)>;

template <typename T, typename Abi>
using SimdMask = std::experimental::simd_mask<T, Abi>;

template <typename T, int N = std::experimental::native_simd<T>::size()>
using NativeSimdMask =
    std::experimental::simd_mask<T,
                                 std::experimental::simd_abi::__native<T, N>>;

template <typename T>
using SimdMask64 = NativeSimdMask<T, 8 / sizeof(T)>;

template <typename T>
using SimdMask128 = NativeSimdMask<T, 16 / sizeof(T)>;

template <typename T>
using SimdMask256 = NativeSimd<T, 32 / sizeof(T)>;

// Returns a Simd type that's based on SimdType, but with a different size.
template <typename SimdType, size_t kNewSize>
using ResizeTo = Simd<
    typename SimdType::value_type,
    std::experimental::simd_abi::deduce_t<
        typename SimdType::value_type, kNewSize, typename SimdType::abi_type>>;

// Returns a Simd type that's based on SimdType, but with the size
// SimdType::size() * kNumerator / kDenominator.
template <typename SimdType, size_t kNumerator, size_t kDenominator = 1>
using ResizeBy =
    ResizeTo<SimdType, SimdType::size() * kNumerator / kDenominator>;

// Returns a Simd type of the same width, but with the element type changed to
// T.
template <typename SimdType, typename NewElementType>
using ReinterpretTo =
    Simd<NewElementType,
         typename ResizeBy<SimdType, sizeof(typename SimdType::value_type),
                           sizeof(NewElementType)>::abi_type>;

// Returns a scalar type with width sizeof(T) * kNumerator / kDenominator.
// For example, ScaleBy<int16, 2> gives int32, ScaleBy<uint16, 1, 2> gives
// uint8.
template <typename T, size_t kNumerator, size_t kDenominator = 1>
using ScaleBy = detail::Number<sizeof(T) * kNumerator / kDenominator,
                               detail::get_number_kind<T>()>;

// Returns a numeric type that's based on T, but with a different width by the
// ratio kNumerator / kDenominator.
template <typename SimdType, size_t kNumerator, size_t kDenominator = 1>
using ScaleElemBy =
    Simd<ScaleBy<typename SimdType::value_type, kNumerator, kDenominator>,
         typename SimdType::abi_type>;

}  // namespace dimsum

#endif  // DIMSUM_TYPES_H_
