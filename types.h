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

namespace dimsum {
namespace detail {

// Different kinds of supported elements.
enum class NumberKind {
  kUInt,
  kSInt,
  kFloat,
};

template <typename T, typename Abi, typename Flags>
struct LoadImpl;

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

enum class StoragePolicy {
  kSimulated,
  kXmm,
  kYmm,
  kVsx,
  kNeon,
};

template <StoragePolicy storage, size_t num_elements>
struct Abi {
  static constexpr StoragePolicy kStorage = storage;
  static constexpr size_t kNumElements = num_elements;

  static_assert(kNumElements > 0, "Abi should contain more than 0 bytes");
  static_assert(((kNumElements - 1) & kNumElements) == 0,
                "Abi should contain power of 2 elements");
};

template <typename T, size_t num_bytes>
struct GccVecTraits {};

#define GCC_VEC_SPECIALIZATION(T, NUM_BYTES)                \
  template <>                                               \
  struct GccVecTraits<T, NUM_BYTES> {                       \
    using type = T __attribute__((vector_size(NUM_BYTES))); \
  }
#define GCC_VEC_SPECIALIZE_ON_NUM_BYTES(NUM_BYTES) \
  GCC_VEC_SPECIALIZATION(uint8, NUM_BYTES);        \
  GCC_VEC_SPECIALIZATION(uint16, NUM_BYTES);       \
  GCC_VEC_SPECIALIZATION(uint32, NUM_BYTES);       \
  GCC_VEC_SPECIALIZATION(uint64, NUM_BYTES);       \
  GCC_VEC_SPECIALIZATION(int8, NUM_BYTES);         \
  GCC_VEC_SPECIALIZATION(int16, NUM_BYTES);        \
  GCC_VEC_SPECIALIZATION(int32, NUM_BYTES);        \
  GCC_VEC_SPECIALIZATION(int64, NUM_BYTES);        \
  GCC_VEC_SPECIALIZATION(float, NUM_BYTES);        \
  GCC_VEC_SPECIALIZATION(double, NUM_BYTES)

GCC_VEC_SPECIALIZE_ON_NUM_BYTES(8);
GCC_VEC_SPECIALIZE_ON_NUM_BYTES(16);
GCC_VEC_SPECIALIZE_ON_NUM_BYTES(32);
GCC_VEC_SPECIALIZE_ON_NUM_BYTES(64);
GCC_VEC_SPECIALIZE_ON_NUM_BYTES(128);
GCC_VEC_SPECIALIZE_ON_NUM_BYTES(256);

GCC_VEC_SPECIALIZATION(uint8, 4);
GCC_VEC_SPECIALIZATION(uint16, 4);
GCC_VEC_SPECIALIZATION(uint32, 4);
GCC_VEC_SPECIALIZATION(int8, 4);
GCC_VEC_SPECIALIZATION(int16, 4);
GCC_VEC_SPECIALIZATION(int32, 4);
GCC_VEC_SPECIALIZATION(float, 4);

GCC_VEC_SPECIALIZATION(uint8, 2);
GCC_VEC_SPECIALIZATION(uint16, 2);
GCC_VEC_SPECIALIZATION(int8, 2);
GCC_VEC_SPECIALIZATION(int16, 2);

GCC_VEC_SPECIALIZATION(uint8, 1);
GCC_VEC_SPECIALIZATION(int8, 1);

#undef GCC_VEC_SPECIALIZE_ON_NUM_BYTES
#undef GCC_VEC_SPECIALIZATION

// The possible corressponding native type for Simd<T, Abi>.
template <typename T, typename Abi>
struct ExternalTypeTraits {};

// If such type doesn't exist, use __attribute__((vector_size(...))).
template <typename T, StoragePolicy kStorage, size_t kNumElements>
struct ExternalTypeTraits<T, detail::Abi<kStorage, kNumElements>>
    : GccVecTraits<T, kNumElements * sizeof(T)> {};

template <typename T>
using ToUnsigned = detail::Number<sizeof(T), detail::NumberKind::kUInt>;

}  // namespace detail

namespace flags {

struct element_aligned_tag {};

constexpr element_aligned_tag element_aligned{};

struct vector_aligned_tag {};

constexpr vector_aligned_tag vector_aligned{};

}  // namespace flags

template <typename T, typename Abi>
class Simd;

template <typename T, typename Abi>
class SimdMask;

// Returns a Simd type that's based on SimdType, but with a different size.
template <typename SimdType, size_t kNewSize>
using ResizeTo = Simd<typename SimdType::value_type,
                      detail::Abi<SimdType::abi_type::kStorage, kNewSize>>;

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

// Simd provides a portable interface of the vector/simd units in the hardware.
//
// The detailed documentation is on http://wg21.link/P0214
//
// Simd should only be instantiated with detail::Abi.
template <typename T, typename Abi>
class Simd {};

template <typename T, detail::StoragePolicy kStorage, size_t kNumElements>
class Simd<T, detail::Abi<kStorage, kNumElements>> {
  using InternalType =
      typename detail::GccVecTraits<T, kNumElements * sizeof(T)>::type;
  using ExternalType = typename detail::ExternalTypeTraits<
      T, detail::Abi<kStorage, kNumElements>>::type;

  using NumberKind = ::dimsum::detail::NumberKind;

  static constexpr NumberKind kKind = ::dimsum::detail::get_number_kind<T>();

  template <size_t element_width, NumberKind kind = kKind>
  using Number = ::dimsum::detail::Number<element_width, kind>;

 public:
  // The element type.
  using value_type = T;

  // The ABI type.
  using abi_type = detail::Abi<kStorage, kNumElements>;

  // The element type that comparison operations return.
  using ComparisonResultType = Number<sizeof(T), NumberKind::kUInt>;

  constexpr Simd() = default;

  // Returns the number of elements in this class.
  static constexpr size_t size() { return kNumElements; }

  // Initialize a Simd object from a function. The function takes a index type,
  // that's convertible from integral_constant<size_t, ...>().
  //
  // Example: Simd128<int32>([](int i) { return 2 * i }) gives [0, 2, 4, 6].
  template <typename Generator,
            typename = decltype(Simd(std::declval<Generator>()(
                std::integral_constant<size_t, 0>())))>
  explicit Simd(const Generator& gen) {
    GeneratorInit(gen, dimsum::make_index_sequence<size()>{});
  }

  // Constructs a Simd object from an arch-specific raw type.
  // It exists only to help transition and refactoring.
  /* implicit */ Simd(ExternalType storage) {  // NOLINT
    static_assert(sizeof(storage) == sizeof(storage_), "Simd width mismatch");
    memcpy(&storage_, &storage, sizeof(storage));
  }

  ExternalType raw() const {
    ExternalType ret;
    static_assert(sizeof(ret) == sizeof(storage_), "Simd width mismatch");
    memcpy(&ret, &storage_, sizeof(ret));
    return ret;
  }

  // Constructs a Simd object from the first size() elements of the buffer.
  template <typename Flags>
  explicit Simd(const T* buffer, Flags flags) {
    copy_from(buffer, flags);
  }

  // Constructs a Simd object, using a single value for all elements.
  Simd(T value) {  // NOLINT
    for (int i = 0; i < size(); i++) {
      storage_[i] = value;
    }
  }

  // Returns the ith element.
  value_type operator[](size_t i) const { return storage_[i]; }

  // Changes the current object to Simd(buffer).
  template <typename Flags>
  void copy_from(const T* buffer, Flags flags);

  // Stores the Simd object to the buffer.
  template <typename Flags>
  void copy_to(T* buffer, Flags) const {
    constexpr size_t bytes = sizeof(storage_);
    if (std::is_same<Flags, flags::vector_aligned_tag>::value) {
      memcpy(__builtin_assume_aligned(buffer, bytes), &storage_, bytes);
    } else {
      memcpy(buffer, &storage_, bytes);
    }
  }

  template <typename Flags>
  __attribute__((deprecated("Use copy_from instead."))) void memload(
      const T* buffer, Flags flags) {
    copy_from(buffer, flags);
  }

  template <typename Flags>
  __attribute__((deprecated("Use copy_to instead."))) void memstore(
      T* buffer, Flags flags) {
    copy_to(buffer, flags);
  }

  // Sets the ith element.
  void set(size_t i, T value) { storage_[i] = value; }

  template <typename Tp, typename Ap>
  friend class Simd;

  template <typename Tp, typename Abi, typename Flags>
  friend struct detail::LoadImpl;

  template <int... indices, typename Tp, typename Ap>
  friend ResizeTo<Simd<Tp, Ap>, sizeof...(indices)> shuffle(Simd<Tp, Ap> lhs,
                                                            Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> add_saturated(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> sub_saturated(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_eq(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_ne(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_lt(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_le(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_gt(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<typename Simd<Tp, Ap>::ComparisonResultType, Ap> cmp_ge(
      Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> min(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> max(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator-(Simd<Tp, Ap> simd);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator~(Simd<Tp, Ap> simd);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator+(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator-(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator*(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator<<(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator>>(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator&(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator|(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Tp, typename Ap>
  friend Simd<Tp, Ap> operator^(Simd<Tp, Ap> lhs, Simd<Tp, Ap> rhs);

  template <typename Dp, typename Tp, typename Ap>
  friend ReinterpretTo<Simd<Tp, Ap>, Dp> bit_cast(Simd<Tp, Ap> simd);

  template <typename Dp, typename Sp, typename Ap>
  friend Simd<Dp, Ap> static_simd_cast(Simd<Sp, Ap> simd);

 private:
  static Simd from_storage(InternalType storage) {
    Simd ret;
    ret.storage_ = storage;
    return ret;
  }

  template <typename Generator, size_t... indices>
  void GeneratorInit(const Generator& gen, dimsum::index_sequence<indices...>);

  InternalType storage_;
};

}  // namespace dimsum

#endif  // DIMSUM_TYPES_H_
