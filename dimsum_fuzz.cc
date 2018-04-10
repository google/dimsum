// Copyright 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dimsum.h"
#include "dimsum_x86.h"
#include "simulated.h"

namespace {

using ::dimsum::NativeSimd;
using ::dimsum::Simd;
using ::dimsum::Simd128;
using dimsum::detail::CheckAddOverflow;
using dimsum::detail::OverflowType;

template <typename T, typename Abi>
bool Equal(const Simd<T, Abi>& lhs, const Simd<T, Abi>& rhs) {
  for (size_t i = 0; i < lhs.size(); i++) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, typename Abi>
void TrapIfNotEqual(const Simd<T, Abi>& lhs, const Simd<T, Abi>& rhs) {
  for (size_t i = 0; i < lhs.size(); i++) {
    if (lhs[i] != rhs[i]) __builtin_trap();
  }
}

template <typename T, typename Abi>
bool IsNormal(const Simd<T, Abi> simd) {
  if (std::is_floating_point<T>::value) {
    for (size_t i = 0; i < simd.size(); i++) {
      if (!std::isnormal(simd[i])) {
        return false;
      }
    }
  }
  return true;
}

// Returns true if a floating point is in a safe range that is guaranteed to be
// represented precisely by an integral type in every rounding mode. Returns
// false for NaN.
//
// Use float -> int32 to simplify the discussion.
// The bounds are [-2**31, 2**31 - 2**(31-24)].
// The upper bound is 24 ones followed by 31-24 zeros, 24 is
// numeric_limits<float>::digits, 23 mantissa bits plus an implicit hidden bit.
// The lower and upper bounds can be represented by int32 and are thus
// safe.
// For float -> uint32, the bounds are [0, 2**32 - 2**(32-24)].
// The upper bound is 24 ones followed by 32-24 zeros.
template <typename Integral, typename T>
bool IsInSafeBound(T f) {
  static_assert(std::is_floating_point<T>::value, "Unsupported type");
  static_assert(std::is_integral<Integral>::value, "Unsupported type");
  constexpr int sign = std::is_signed<Integral>::value;
  // The least significant one bit of the upper bound.
  // Note it is different for signed/unsigned types.
  constexpr int bit = static_cast<int>(CHAR_BIT * sizeof(Integral) - sign) -
                      std::numeric_limits<T>::digits;
  constexpr T lower = static_cast<T>(std::numeric_limits<Integral>::min());
  // If T(max()) cannot be represented precisely by Integral,
  // the upper bound is a bit pattern with numeric_limits<T>::digits ones.
  // Under the FE_UPWARD rounding mode, larger numbers will be rounded up to
  // be greater or equal to max()+1 which can be represented by T.
  constexpr T upper =
      bit > 0 ? static_cast<T>(std::numeric_limits<Integral>::max() -
                               (Integral{1} << bit) + 1)
              : static_cast<T>(std::numeric_limits<Integral>::max());
  // NaNs are also precluded.
  return lower <= f && f <= upper;
}

// Converts NaN to 0 and clamp non-NaN values to a safe bound that can be
// represented precisely by Integral.
template <typename Integral, typename T, typename Abi>
bool IsSimdInSafeBound(const Simd<T, Abi> simd) {
  for (int i = 0; i < simd.size(); i++) {
    if (!IsInSafeBound<Integral>(simd[i])) return false;
  }
  return true;
}

template <typename T, typename Abi>
void LoadFromRaw(const uint8_t* data, Simd<T, Abi>* simd) {
  T buffer[Simd<T, Abi>::size()];
  memcpy(buffer, data, sizeof(buffer));
  *simd = Simd<T, Abi>(buffer, dimsum::flags::element_aligned);
}

template <typename To, typename From>
void TestFloatingToIntegralStaticSimdCast(const uint8_t* data) {
  NativeSimd<From> simd;
  LoadFromRaw(data, &simd);
  if (IsSimdInSafeBound<To>(simd)) {
    TrapIfNotEqual(dimsum::simulated::static_simd_cast<To>(simd),
                   dimsum::static_simd_cast<To>(simd));
  }
}

template <typename To, typename From>
void TestStaticSimdCast(const uint8_t* data) {
  NativeSimd<From> simd;
  LoadFromRaw(data, &simd);
  TrapIfNotEqual(dimsum::simulated::static_simd_cast<To>(simd),
                 dimsum::static_simd_cast<To>(simd));
}

template <typename T>
void TestReduce(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (!IsNormal(simd)) return;

  // Take abs to avoid catastrophic cancellation.
  simd = dimsum::abs(simd);
  T sum0 = 0;
  for (size_t i = 0; i < simd.size(); i++)
    sum0 += simd[i];
  T sum1 = dimsum::reduce(simd);
  // Trap if `reduce` sum deviates from sum0 too much.
  if (std::isfinite(sum1) && std::abs(sum1 - sum0) / sum0 > 1e-5)
    __builtin_trap();
}

template <typename T>
void TestHMin(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (!IsNormal(simd)) return;
  if (dimsum::simulated::hmin(simd) != dimsum::hmin(simd))
    __builtin_trap();
}

template <typename T>
void TestHMax(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (!IsNormal(simd)) return;
  if (dimsum::simulated::hmax(simd) != dimsum::hmax(simd))
    __builtin_trap();
}

template <typename SimdType, size_t NewSize>
void TestSameTypeReduceAdd(const uint8_t* data) {
  SimdType input;
  LoadFromRaw(data, &input);
  const auto& v = input;
  constexpr size_t kArity = SimdType::size() / NewSize;
  for (int i = 0; i < v.size(); i += kArity) {
    typename SimdType::value_type sum = 0;
    for (int j = 0; j < kArity; j++) {
      if (CheckAddOverflow(sum, v[i + j]) != OverflowType::kNoOverflow) {
        return;
      }
      sum += v[i + j];
    }
  }

  TrapIfNotEqual(
      dimsum::simulated::reduce_add<typename SimdType::value_type, NewSize>(v),
      dimsum::reduce_add<typename SimdType::value_type, NewSize>(v));
}

template <typename SrcType, typename NewType>
void TestSameTotalWidthReduceAdd(const uint8_t* data) {
  NativeSimd<SrcType> input;
  LoadFromRaw(data, &input);
  const auto& v = input;
  constexpr size_t kArity = sizeof(NewType) / sizeof(SrcType);
  constexpr size_t NewSize = input.size() / kArity;
  for (int i = 0; i < v.size(); i += kArity) {
    NewType sum = 0;
    for (int j = 0; j < kArity; j++) {
      if (CheckAddOverflow(sum, NewType{v[i + j]}) !=
          OverflowType::kNoOverflow) {
        return;
      }
      sum += v[i + j];
    }
  }

  TrapIfNotEqual(dimsum::simulated::reduce_add<NewType, NewSize>(v),
                 dimsum::reduce_add<NewType, NewSize>(v));
}

template <typename T, typename Acc>
void TestMulSum(const uint8_t* data) {
  NativeSimd<T> lhs, rhs;
  NativeSimd<Acc> acc;
  LoadFromRaw(data, &lhs);
  LoadFromRaw(data + sizeof(lhs), &rhs);
  LoadFromRaw(data + sizeof(lhs) + sizeof(rhs), &acc);

  TrapIfNotEqual(dimsum::simulated::mul_sum<Acc>(lhs, rhs),
                 dimsum::mul_sum<Acc>(lhs, rhs));
  TrapIfNotEqual(dimsum::simulated::mul_sum(lhs, rhs, acc),
                 dimsum::mul_sum(lhs, rhs, acc));
}

template <typename T>
void TestFma(const uint8_t* data) {
  NativeSimd<T> a, b, c;
  LoadFromRaw(data, &a);
  LoadFromRaw(data + sizeof(a), &b);
  LoadFromRaw(data + sizeof(a) + sizeof(b), &c);

  NativeSimd<T> sim_res = dimsum::simulated::fma(a, b, c);
  if (IsNormal(sim_res))
    TrapIfNotEqual(sim_res, dimsum::fma(a, b, c));
}

void TestMaddubs(const uint8_t* data) {
  NativeSimd<uint8> lhs;
  NativeSimd<int8> rhs;
  LoadFromRaw(data, &lhs);
  LoadFromRaw(data + sizeof(lhs), &rhs);
  TrapIfNotEqual(dimsum::simulated::maddubs(lhs, rhs),
                 dimsum::x86::maddubs(lhs, rhs));
}

template <typename T>
void TestAbs(const uint8_t* data) {
  NativeSimd<T> s, res, sim_res;
  LoadFromRaw(data, &s);
  const auto& simd = s;
  if (std::is_signed<T>::value) {
    for (int i = 0; i < simd.size(); i++) {
      if (simd[i] == std::numeric_limits<T>::min()) {
        return;
      }
    }
  }

  res = dimsum::abs(simd);
  sim_res = dimsum::simulated::abs(simd);
  for (int i = 0; i < simd.size(); i++) {
    if ((std::is_integral<T>::value || !std::isnan(simd[i])) &&
        res[i] != sim_res[i]) {
      __builtin_trap();
    }
  }
}

void TestReciprocalEstimate(const uint8_t* data) {
  NativeSimd<float> simd, sim_res, res;
  LoadFromRaw(data, &simd);

  // Each lane is independent, so we don't reject the vector if any of its lane
  // is infinity, NaN, or denormalized value.
  sim_res = dimsum::simulated::reciprocal_estimate(simd);
  res = dimsum::reciprocal_estimate(simd);
  for (size_t i = 0; i < simd.size(); i++) {
    float x = simd[i], f = res[i], sim_f = sim_res[i];
    // On ARM, POWER and x86, the result is NaN if the operand is NaN.
    // ARM offers the least precision, the relative error may be slightly
    // greater than 1/512. For some number whose
    // absolute value is within a safe bound [2**-125, 2**125], if the results
    // differ by a fraction of more than 1/128 (much larger than 1/512),
    // consider it an error.
    if (!(std::isnan(x) || sim_f == f ||
          std::abs((sim_f - f) / f) < 1 / 128.f ||
          std::abs(x) < std::ldexp(1.f, -125) ||
          std::ldexp(1.f, 125) < std::abs(x))) {
      __builtin_trap();
    }
  }
}

void TestSqrt(const uint8_t* data) {
  NativeSimd<float> simd, res, sim_res;
  LoadFromRaw(data, &simd);

  // Each lane is independent, so we don't reject the vector if any of its lane
  // is infinity, NaN, or denormalized value.
  sim_res = dimsum::simulated::sqrt(simd);
  res = dimsum::sqrt(simd);
  for (int i = 0; i < NativeSimd<float>::size(); i++) {
    float x = simd[i], f = res[i], sim_f = sim_res[i];
    // Treat it as an error if the relative error is too large.
    if (std::isnormal(x) && std::abs((sim_f - f) / f) > 1 / 1024.f) {
      __builtin_trap();
    }
  }
}

void TestReciprocalSqrtEstimate(const uint8_t* data) {
  NativeSimd<float> simd, res, sim_res;
  LoadFromRaw(data, &simd);

  // Each lane is independent, so we don't reject the vector if any of its lane
  // is infinity, NaN, or denormalized value.
  sim_res = dimsum::simulated::reciprocal_sqrt_estimate(simd);
  res = dimsum::reciprocal_sqrt_estimate(simd);
  for (int i = 0; i < simd.size(); i++) {
    float x = simd[i], f = res[i], sim_f = sim_res[i];
    // Treat it as an error if the relative error is too large.
    if (std::isnormal(x) && std::abs(x) < std::ldexp(1.f, -125) &&
        std::ldexp(1.f, 125) < std::abs(x) &&
        std::abs((sim_f - f) / f) > 1 / 128.f) {
      __builtin_trap();
    }
  }
}

template <typename T>
void TestRound(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (IsNormal(simd)) {
    TrapIfNotEqual(dimsum::simulated::round(simd), dimsum::round(simd));
  }
}

template <typename T, typename Integral>
void TestRoundToInteger(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (IsSimdInSafeBound<Integral>(simd)) {
    TrapIfNotEqual(dimsum::simulated::round_to_integer<Integral>(simd),
                   dimsum::round_to_integer<Integral>(simd));
  }
}

template <typename T>
void TestMax(const uint8_t* data) {
  NativeSimd<T> lhs, rhs;
  LoadFromRaw(data, &lhs);
  LoadFromRaw(data + sizeof(lhs), &rhs);
  if (IsNormal(lhs) && IsNormal(rhs)) {
    TrapIfNotEqual(dimsum::simulated::max(lhs, rhs), max(lhs, rhs));
  }
}

template <typename T>
void TestMin(const uint8_t* data) {
  NativeSimd<T> lhs, rhs;
  LoadFromRaw(data, &lhs);
  LoadFromRaw(data + sizeof(lhs), &rhs);
  if (IsNormal(lhs) && IsNormal(rhs)) {
    TrapIfNotEqual(dimsum::simulated::min(lhs, rhs), min(lhs, rhs));
  }
}

template <typename SrcType>
void TestMulWidened(const uint8_t* data) {
  NativeSimd<SrcType> lhs, rhs;
  LoadFromRaw(data, &lhs);
  LoadFromRaw(data + sizeof(lhs), &rhs);
  TrapIfNotEqual(dimsum::simulated::mul_widened(lhs, rhs),
                 dimsum::mul_widened(lhs, rhs));
}

template <typename T>
void TestMovemask(const uint8_t* data) {
  NativeSimd<T> simd;
  LoadFromRaw(data, &simd);
  if (dimsum::simulated::movemask(simd) != dimsum::x86::movemask(simd)) {
    __builtin_trap();
  }
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  constexpr size_t kNativeWidth = NativeSimd<int>::size() * sizeof(int);
  if (size >= kNativeWidth) {
    TestFloatingToIntegralStaticSimdCast<int32, float>(data);
    TestFloatingToIntegralStaticSimdCast<uint32, float>(data);
    TestFloatingToIntegralStaticSimdCast<int64, double>(data);
    TestFloatingToIntegralStaticSimdCast<uint64, double>(data);

    TestStaticSimdCast<float, int32>(data);
    TestStaticSimdCast<float, uint32>(data);
    TestStaticSimdCast<double, int64>(data);
    TestStaticSimdCast<double, uint64>(data);
    TestStaticSimdCast<int8, uint8>(data);
    TestStaticSimdCast<uint8, int8>(data);
    TestStaticSimdCast<int16, uint16>(data);
    TestStaticSimdCast<uint16, int16>(data);
    TestStaticSimdCast<int32, uint32>(data);
    TestStaticSimdCast<uint32, int32>(data);
    TestStaticSimdCast<int64, uint64>(data);
    TestStaticSimdCast<uint64, int64>(data);

    TestAbs<int8>(data);
    TestAbs<int16>(data);
    TestAbs<int32>(data);
    TestAbs<int64>(data);
    TestAbs<float>(data);
    TestAbs<double>(data);

    TestReciprocalEstimate(data);
    TestReciprocalSqrtEstimate(data);
    TestSqrt(data);
    TestRound<float>(data);
    TestRound<double>(data);
    TestRoundToInteger<float, int32>(data);

    TestReduce<float>(data);
    TestReduce<double>(data);

    TestHMin<int8>(data);
    TestHMin<int16>(data);
    TestHMin<int32>(data);
    TestHMin<int64>(data);
    TestHMin<uint8>(data);
    TestHMin<uint16>(data);
    TestHMin<uint32>(data);
    TestHMin<uint64>(data);
    TestHMin<float>(data);
    TestHMin<double>(data);

    TestHMax<int8>(data);
    TestHMax<int16>(data);
    TestHMax<int32>(data);
    TestHMax<int64>(data);
    TestHMax<uint8>(data);
    TestHMax<uint16>(data);
    TestHMax<uint32>(data);
    TestHMax<uint64>(data);
    TestHMax<float>(data);
    TestHMax<double>(data);

    TestSameTypeReduceAdd<Simd128<int8>, 8>(data);
    TestSameTypeReduceAdd<Simd128<int8>, 4>(data);
    TestSameTypeReduceAdd<Simd128<int8>, 2>(data);
    TestSameTypeReduceAdd<Simd128<int8>, 1>(data);
    TestSameTypeReduceAdd<Simd128<int16>, 4>(data);
    TestSameTypeReduceAdd<Simd128<int16>, 2>(data);
    TestSameTypeReduceAdd<Simd128<int16>, 1>(data);
    TestSameTypeReduceAdd<Simd128<int32>, 2>(data);
    TestSameTypeReduceAdd<Simd128<int32>, 1>(data);
    TestSameTypeReduceAdd<Simd128<int64>, 1>(data);
    TestSameTypeReduceAdd<Simd128<uint8>, 8>(data);
    TestSameTypeReduceAdd<Simd128<uint8>, 4>(data);
    TestSameTypeReduceAdd<Simd128<uint8>, 2>(data);
    TestSameTypeReduceAdd<Simd128<uint8>, 1>(data);
    TestSameTypeReduceAdd<Simd128<uint16>, 4>(data);
    TestSameTypeReduceAdd<Simd128<uint16>, 2>(data);
    TestSameTypeReduceAdd<Simd128<uint16>, 1>(data);
    TestSameTypeReduceAdd<Simd128<uint32>, 2>(data);
    TestSameTypeReduceAdd<Simd128<uint32>, 1>(data);
    TestSameTypeReduceAdd<Simd128<uint64>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<int8>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<int16>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<int32>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<int64>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<uint8>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<uint16>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<uint32>, 1>(data);
    TestSameTypeReduceAdd<NativeSimd<uint64>, 1>(data);

    TestSameTotalWidthReduceAdd<int8, int16>(data);
    TestSameTotalWidthReduceAdd<int8, int32>(data);
    TestSameTotalWidthReduceAdd<int8, int64>(data);
    TestSameTotalWidthReduceAdd<int16, int32>(data);
    TestSameTotalWidthReduceAdd<int16, int64>(data);
    TestSameTotalWidthReduceAdd<int32, int64>(data);
    TestSameTotalWidthReduceAdd<uint8, uint16>(data);
    TestSameTotalWidthReduceAdd<uint8, uint32>(data);
    TestSameTotalWidthReduceAdd<uint8, uint64>(data);
    TestSameTotalWidthReduceAdd<uint16, uint32>(data);
    TestSameTotalWidthReduceAdd<uint16, uint64>(data);
    TestSameTotalWidthReduceAdd<uint32, uint64>(data);

    // ----- dimsum::x86::*
    TestMovemask<int8>(data);
    TestMovemask<int32>(data);
    TestMovemask<int64>(data);
    TestMovemask<uint8>(data);
    TestMovemask<uint32>(data);
    TestMovemask<uint64>(data);
  }

  if (size >= kNativeWidth * 2) {
    TestMax<int8>(data);
    TestMax<int16>(data);
    TestMax<int32>(data);
    TestMax<uint8>(data);
    TestMax<uint16>(data);
    TestMax<uint32>(data);
    TestMax<float>(data);
    TestMax<double>(data);

    TestMin<int8>(data);
    TestMin<int16>(data);
    TestMin<int32>(data);
    TestMin<uint8>(data);
    TestMin<uint16>(data);
    TestMin<uint32>(data);
    TestMin<float>(data);
    TestMin<double>(data);

    TestMulWidened<int8>(data);
    TestMulWidened<int16>(data);
    TestMulWidened<int32>(data);
    TestMulWidened<uint8>(data);
    TestMulWidened<uint16>(data);
    TestMulWidened<uint32>(data);

    // ----- dimsum::x86::*
    TestMaddubs(data);
  }

  if (size >= kNativeWidth * 3) {
    TestMulSum<int8, int32>(data);
    TestMulSum<uint8, uint32>(data);
    TestMulSum<int16, int32>(data);

    TestFma<float>(data);
    TestFma<double>(data);
  }

  return 0;
}
