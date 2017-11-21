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

#include <cassert>
#include <limits>
#include <type_traits>

#include "simulated.h"
#include "gtest/gtest.h"

namespace dimsum {

template <typename T, typename Abi>
bool operator==(const Simd<T, Abi>& lhs, const Simd<T, Abi>& rhs) {
  for (int i = 0; i < Simd<T, Abi>::size(); i++) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, typename Abi>
std::ostream& operator<<(std::ostream& output, Simd<T, Abi> simd) {
  output << "[" << std::to_string(simd[0]);
  for (int i = 1; i < Simd<T, Abi>::size(); i++) {
    output << ", " << std::to_string(simd[i]);
  }
  output << "]";
  return output;
}

namespace {

#define SIMD_UNARY_FREE_FUNC_TEST(TYPE, FUNC, INPUT)                      \
  for (auto(&entry) : INPUT) {                                            \
    auto a = Simd128<TYPE>::list(entry[0], entry[1], entry[2], entry[3]); \
    EXPECT_EQ(simulated::FUNC(a), FUNC(a)) << #FUNC "(" << a << ")";      \
  }

#define SIMD_BINARY_FREE_FUNC_TEST(TYPE, FUNC, INPUT)                       \
  for (auto(&entry) : INPUT) {                                              \
    auto lhs = Simd128<TYPE>::list(entry[0], entry[1], entry[2], entry[3]); \
    auto rhs = Simd128<TYPE>::list(entry[4], entry[5], entry[6], entry[7]); \
    EXPECT_EQ(simulated::FUNC(lhs, rhs), FUNC(lhs, rhs))                    \
        << #FUNC "(" << lhs << ", " << rhs << ")";                          \
  }

#define SIMD_UNARY_OP_TEST(TYPE, FUNC, OP, INPUT)                              \
  for (auto(&entry) : INPUT) {                                                 \
    auto a = Simd128<TYPE>::list(entry[0], entry[1], entry[2], entry[3]);      \
    EXPECT_EQ(simulated::FUNC(a), operator OP(a)) << #FUNC "(" << a << ")";    \
  }

#define SIMD_BINARY_OP_TEST(TYPE, FUNC, OP, INPUT)                             \
  for (auto(&entry) : INPUT) {                                                 \
    auto lhs = Simd128<TYPE>::list(entry[0], entry[1], entry[2], entry[3]);    \
    auto rhs = Simd128<TYPE>::list(entry[4], entry[5], entry[6], entry[7]);    \
    EXPECT_EQ(simulated::FUNC(lhs, rhs), operator OP(lhs, rhs))                \
        << "operator" #OP "(" << lhs << ", " << rhs << ")";                    \
  }

int32 bitconvert(uint32 a) {
  int32 ret;
  memcpy(&ret, &a, sizeof(a));
  return ret;
}

// This function is used to test reciprocal related functions.
template <typename Simd>
bool WithinFraction(const Simd& lhs, const Simd& rhs) {
  for (int i = 0; i < Simd::size(); i++) {
    auto l = lhs[i], r = rhs[i];
    // ARM offers the least precision, the relative error may be slightly
    // greater than 1/512. It is safe to use 1/256 here.
    if (!(l == r ||
          std::abs(l - r) < 1 / 256. * std::max(std::abs(l), std::abs(r))))
      return false;
  }
  return true;
}

int32 boring_unary_op_test[][8] = {
    {1, 2, 3, 4},
    {-1, -2, -3, -4},
    {-1, 2, -3, 4},
    {1, -2, 3, -4},
    {1, 2, 3, 4},
    {-1, -2, -3, -4},
    {bitconvert(0xdeadbeef), bitconvert(0xcccccccc), bitconvert(0xc0c0c0c0),
     bitconvert(0xabababab)},
};

int32 boring_binary_op_test[][8] = {
    {1, 2, 3, 4, 5, 6, 7, 8},
    {-1, -2, -3, -4, -5, -6, -7, -8},
    {-1, 2, -3, 4, -5, 6, -7, 8},
    {1, -2, 3, -4, 5, -6, 7, -8},
    {1, 2, 3, 4, -5, -6, -7, -8},
    {-1, -2, -3, -4, 5, 6, 7, 8},
    {bitconvert(0xdeadbeef), bitconvert(0xcccccccc), bitconvert(0xc0c0c0c0),
     bitconvert(0xabababab), bitconvert(0x1aaaaaaa), bitconvert(0x1bcdefff),
     bitconvert(0x0ccceeff), bitconvert(0x1eadbeef)},
};

float boring_unary_op_test_float[][8] = {
    {1., 2., 3., 4.},
    {-1., -2., -3., -4.},
    {-1., 2., -3., 4.},
    {1., -2., 3., -4.},
    {1., 2., 3., 4.},
    {-1., -2., -3., -4.},
    {static_cast<float>(bitconvert(0xdeadbeef)),
     static_cast<float>(bitconvert(0xcccccccc)),
     static_cast<float>(bitconvert(0xc0c0c0c0)),
     static_cast<float>(bitconvert(0xabababab))},
};

float boring_binary_op_test_float[][8] = {
    {1., 2., 3., 4., 5., 6., 7., 8.},
    {-1., -2., -3., -4., -5., -6., -7., -8.},
    {-1., 2., -3., 4., -5., 6., -7., 8.},
    {1., -2., 3., -4., 5., -6., 7., -8.},
    {1., 2., 3., 4., -5., -6., -7., -8.},
    {-1., -2., -3., -4., 5, 6., 7., 8.},
    {static_cast<float>(bitconvert(0xdeadbeef)),
     static_cast<float>(bitconvert(0xcccccccc)),
     static_cast<float>(bitconvert(0xc0c0c0c0)),
     static_cast<float>(bitconvert(0xabababab)),
     static_cast<float>(bitconvert(0x1aaaaaaa)),
     static_cast<float>(bitconvert(0x1bcdefff)),
     static_cast<float>(bitconvert(0x0ccceeff)),
     static_cast<float>(bitconvert(0x1eadbeef))},
};

int32 elementwise_mul_test[][8] = {
    {1, 2, 3, 4, 5, 6, 7, 8},
    {-1, -2, -3, -4, -5, -6, -7, -8},
    {-1, 2, -3, 4, -5, 6, -7, 8},
    {1, -2, 3, -4, 5, -6, 7, -8},
    {1, 2, 3, 4, -5, -6, -7, -8},
    {-1, -2, -3, -4, 5, 6, 7, 8},
};

uint32 elementwise_mul_test_uint[][8] = {
    {0x0000beef, 0x0000cccc, 0x0000c0c0, 0x0000abab, 0x0000aaaa, 0x0000efff,
     0x0000eeff, 0x0000beef},
};

float elementwise_mul_test_float[][8] = {
    {1., 2., 3., 4., 5., 6., 7., 8.},
    {-1., -2., -3., -4., -5., -6., -7., -8.},
    {-1., 2., -3., 4., -5., 6., -7., 8.},
    {1., -2., 3., -4., 5., -6., 7., -8.},
    {1., 2., 3., 4., -5., -6., -7., -8.},
    {-1., -2., -3., -4., 5, 6., 7., 8.},
    {static_cast<float>(bitconvert(0x0000beef)),
     static_cast<float>(bitconvert(0x0000cccc)),
     static_cast<float>(bitconvert(0x0000c0c0)),
     static_cast<float>(bitconvert(0x0000abab)),
     static_cast<float>(bitconvert(0x0000aaaa)),
     static_cast<float>(bitconvert(0x0000efff)),
     static_cast<float>(bitconvert(0x0000eeff)),
     static_cast<float>(bitconvert(0x0000beef))},
};

TEST(DimsumTest, Load) {
  {
    int32 a[] = {1, 2, 3, 4};
    auto s = Simd128<int32>(a, flags::element_aligned);
    EXPECT_EQ(1, s[0]);
    EXPECT_EQ(2, s[1]);
    EXPECT_EQ(3, s[2]);
    EXPECT_EQ(4, s[3]);
  }
  {
    float a[] = {1., 2., 3., 4.};
    auto s = Simd128<float>(a, flags::element_aligned);
    EXPECT_EQ(1., s[0]);
    EXPECT_EQ(2., s[1]);
    EXPECT_EQ(3., s[2]);
    EXPECT_EQ(4., s[3]);
  }
  {
    alignas(16) float a[] = {1., 2., 3., 4.};
    auto s = Simd128<float>(a, flags::vector_aligned);
    EXPECT_EQ(1., s[0]);
    EXPECT_EQ(2., s[1]);
    EXPECT_EQ(3., s[2]);
    EXPECT_EQ(4., s[3]);
  }
  {
    alignas(8) float a[] = {1., 2.};
    auto s = Simd64<float>(a, flags::vector_aligned);
    EXPECT_EQ(1., s[0]);
    EXPECT_EQ(2., s[1]);
  }
  {
    alignas(32) float a[] = {1., 2., 3., 4., 5., 6., 7., 8.};
    auto s = ResizeBy<Simd128<float>, 2, 1>(a, flags::vector_aligned);
    EXPECT_EQ(1., s[0]);
    EXPECT_EQ(2., s[1]);
    EXPECT_EQ(3., s[2]);
    EXPECT_EQ(4., s[3]);
    EXPECT_EQ(5., s[4]);
    EXPECT_EQ(6., s[5]);
    EXPECT_EQ(7., s[6]);
    EXPECT_EQ(8., s[7]);
  }
}

TEST(DimsumTest, Cast) {
  auto a = bit_cast<uint32>(Simd128<int32>::list(-1, -2, -3, -4));
  auto b =
      Simd128<uint32>::list(0xffffffff, 0xfffffffe, 0xfffffffd, 0xfffffffc);
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(a[i], b[i]) << i;
  }

  auto c = Simd128<float>::list(0.5, 1.5, 2.5, 3.5);
  auto d = bit_cast<float>(bit_cast<int32>(c));
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(c[i], d[i]) << i;
  }

  if (std::numeric_limits<float>::is_iec559) {
    EXPECT_EQ(NativeSimd<uint32>(0x7F800000),
              bit_cast<uint32>(
                  NativeSimd<float>(std::numeric_limits<float>::infinity())));
  }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  EXPECT_EQ(
      (Simd128<uint32>::list(0x00020001, 0x00040003, 0x00060005, 0x00080007)),
      bit_cast<uint32>(Simd128<uint16>::list(1, 2, 3, 4, 5, 6, 7, 8)));
#endif
}

TEST(DimsumTest, Splat) {
  {
    auto s = NativeSimd<int32>(42);
    EXPECT_EQ(42, s[0]);
    EXPECT_EQ(42, s[1]);
    EXPECT_EQ(42, s[2]);
    EXPECT_EQ(42, s[3]);
  }
  {
    auto s = NativeSimd<float>(42.);
    EXPECT_EQ(42., s[0]);
    EXPECT_EQ(42., s[1]);
    EXPECT_EQ(42., s[2]);
    EXPECT_EQ(42., s[3]);
  }
}

TEST(DimsumTest, CopyTo) {
  {
    int32 a[4] = {0};
    Simd128<int32>::list(1, 2, 3, 4).copy_to(a, flags::element_aligned);
    EXPECT_EQ(1, a[0]);
    EXPECT_EQ(2, a[1]);
    EXPECT_EQ(3, a[2]);
    EXPECT_EQ(4, a[3]);
  }
  {
    float a[4] = {0.};
    Simd128<float>::list(1., 2., 3., 4.).copy_to(a, flags::element_aligned);
    EXPECT_EQ(1., a[0]);
    EXPECT_EQ(2., a[1]);
    EXPECT_EQ(3., a[2]);
    EXPECT_EQ(4., a[3]);
  }
}

TEST(DimsumTest, Negate) {
  SIMD_UNARY_OP_TEST(int32, negate, -, boring_unary_op_test);
  SIMD_UNARY_OP_TEST(float, negate, -, boring_unary_op_test_float);
}

TEST(DimsumTest, Abs) {
  SIMD_UNARY_FREE_FUNC_TEST(int32, abs, boring_unary_op_test);
  SIMD_UNARY_FREE_FUNC_TEST(float, abs, boring_unary_op_test_float);
}

TEST(DimsumTest, ReciprocalEstimate) {
  EXPECT_TRUE(WithinFraction(
      reciprocal_estimate(NativeSimd<float>(std::ldexp(-1.1f, -125))),
      NativeSimd<float>(1 / std::ldexp(-1.1f, -125))));
  EXPECT_TRUE(WithinFraction(reciprocal_estimate(NativeSimd<float>(-3e30)),
                             NativeSimd<float>(1 / -3e30)));
  EXPECT_TRUE(WithinFraction(reciprocal_estimate(NativeSimd<float>(9e30)),
                             NativeSimd<float>(1 / 9e30)));
  EXPECT_TRUE(WithinFraction(
      reciprocal_estimate(NativeSimd<float>(std::ldexp(1.1f, 125))),
      NativeSimd<float>(1 / std::ldexp(1.1f, 125))));
}

TEST(DimsumTest, Sqrt) {
  EXPECT_TRUE(WithinFraction(sqrt(NativeSimd<float>(2e-30)),
                             NativeSimd<float>(std::sqrt(2e-30))));
  EXPECT_TRUE(WithinFraction(sqrt(NativeSimd<float>(9e30)),
                             NativeSimd<float>(std::sqrt(9e30))));

  EXPECT_TRUE(WithinFraction(sqrt(NativeSimd<double>(2e-30)),
                             NativeSimd<double>(std::sqrt(2e-30))));
  EXPECT_TRUE(WithinFraction(sqrt(NativeSimd<double>(9e30)),
                             NativeSimd<double>(std::sqrt(9e30))));
}

TEST(DimsumTest, ReciprocalSqrtEstimate) {
  EXPECT_TRUE(WithinFraction(
      reciprocal_sqrt_estimate(NativeSimd<float>(std::ldexp(1.1f, -125))),
      NativeSimd<float>(1 / std::sqrt(std::ldexp(1.1f, -125)))));
  EXPECT_TRUE(WithinFraction(reciprocal_sqrt_estimate(NativeSimd<float>(2e-30)),
                             NativeSimd<float>(1 / std::sqrt(2e-30))));
  EXPECT_TRUE(WithinFraction(reciprocal_sqrt_estimate(NativeSimd<float>(9e30)),
                             NativeSimd<float>(1 / std::sqrt(9e30))));
  EXPECT_TRUE(WithinFraction(
      reciprocal_sqrt_estimate(NativeSimd<float>(std::ldexp(1.3f, 125))),
      NativeSimd<float>(1 / std::sqrt(std::ldexp(1.3f, 125)))));
}

TEST(DimsumTest, Add) {
  SIMD_BINARY_OP_TEST(int32, add, +, boring_binary_op_test);
  SIMD_BINARY_OP_TEST(float, add, +, boring_binary_op_test_float);
}

TEST(DimsumTest, Add_saturated) {
  // test int8
  auto s8_lhs = Simd128<int8>::list(50, 60, 70, 80, 90, 100, 110, 120, -50, -60,
                                    -70, -80, -90, -100, -110, -120);
  auto s8_rhs = Simd128<int8>::list(10, 20, 30, 40, 50, 60, 70, 80, -10, -20,
                                    -30, -40, -50, -60, -70, -80);
  auto s8_valid = Simd128<int8>::list(60, 80, 100, 120, 127, 127, 127, 127, -60,
                                      -80, -100, -120, -128, -128, -128, -128);

  EXPECT_EQ(s8_valid, add_saturated(s8_lhs, s8_rhs));

  // test uint8
  auto u8_lhs = Simd128<uint8>::list(10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                                     110, 120, 130, 140, 150, 160);
  auto u8_rhs = Simd128<uint8>::list(10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                                     110, 120, 130, 140, 150, 160);
  auto u8_valid = Simd128<uint8>::list(20, 40, 60, 80, 100, 120, 140, 160, 180,
                                       200, 220, 240, 255, 255, 255, 255);

  EXPECT_EQ(u8_valid, add_saturated(u8_lhs, u8_rhs));

  // test int16
  auto s16_lhs = Simd128<int16>::list(-30000, -20000, -10000, 0, 32767, 10000,
                                      20000, 30000);
  auto s16_rhs = Simd128<int16>::list(-30000, -20000, -10000, -32768, 0, 10000,
                                      20000, 30000);
  auto s16_valid = Simd128<int16>::list(-32768, -32768, -20000, -32768, 32767,
                                        20000, 32767, 32767);

  EXPECT_EQ(s16_valid, add_saturated(s16_lhs, s16_rhs));

  // test uint16
  auto u16_lhs = Simd128<uint16>::list(0, 1, 2, 1000, 2000, 5000, 40000, 50000);
  auto u16_rhs =
      Simd128<uint16>::list(65535, 65535, 65534, 111, 222, 555, 30000, 40000);
  auto u16_valid = Simd128<uint16>::list(65535, 65535, 65535, 1111, 2222, 5555,
                                         65535, 65535);

  EXPECT_EQ(u16_valid, add_saturated(u16_lhs, u16_rhs));
}

TEST(DimsumTest, Sub) {
  SIMD_BINARY_OP_TEST(int32, sub, -, boring_binary_op_test);
  SIMD_BINARY_OP_TEST(float, sub, -, boring_binary_op_test_float);
}

TEST(DimsumTest, Sub_saturated) {
  // test int8
  auto s8_lhs = Simd128<int8>::list(0, 0, 0, 0, 1, 1, 1, 1, -1, -1, -1, -2,
                                    -128, -127, -127, 127);
  auto s8_rhs = Simd128<int8>::list(0, 1, -128, -127, 0, 1, 127, -127, 127,
                                    -128, 0, 127, 0, 1, 2, -1);
  auto s8_valid = Simd128<int8>::list(0, -1, 127, 127, 1, 0, -126, 127, -128,
                                      127, -1, -128, -128, -128, -128, 127);

  EXPECT_EQ(s8_valid, sub_saturated(s8_lhs, s8_rhs));

  // test uint8
  auto u8_lhs = Simd128<uint8>::list(0, 0, 0, 0, 1, 1, 1, 1, 127, 127, 127, 127,
                                     255, 255, 255, 255);
  auto u8_rhs = Simd128<uint8>::list(0, 1, 127, 255, 0, 1, 127, 255, 0, 1, 127,
                                     255, 0, 1, 127, 255);
  auto u8_valid = Simd128<uint8>::list(0, 0, 0, 0, 1, 0, 0, 0, 127, 126, 0, 0,
                                       255, 254, 128, 0);

  EXPECT_EQ(u8_valid, sub_saturated(u8_lhs, u8_rhs));

  // test int16
  auto s16_lhs = Simd128<int16>::list(0, 0, 0, 1, -32768, -32768, 32767, 32767);
  auto s16_rhs =
      Simd128<int16>::list(-32768, -32767, 0, -32768, 32767, 1, -32768, -1);
  auto s16_valid = Simd128<int16>::list(32767, 32767, 0, 32767, -32768, -32768,
                                        32767, 32767);

  EXPECT_EQ(s16_valid, sub_saturated(s16_lhs, s16_rhs));

  // test uint16
  auto u16_lhs = Simd128<uint16>::list(0, 0, 1, 1, 32767, 32767, 65535, 65535);
  auto u16_rhs = Simd128<uint16>::list(0, 1, 0, 65535, 0, 65535, 0, 65535);
  auto u16_valid = Simd128<uint16>::list(0, 0, 1, 0, 32767, 0, 65535, 0);

  EXPECT_EQ(u16_valid, sub_saturated(u16_lhs, u16_rhs));
}

TEST(DimsumTest, Mul) {
  SIMD_BINARY_OP_TEST(int32, mul, *, elementwise_mul_test);
  SIMD_BINARY_OP_TEST(uint32, mul, *, elementwise_mul_test_uint);
  SIMD_BINARY_OP_TEST(float, mul, *, elementwise_mul_test_float);
}

TEST(DimsumTest, ShiftLeft) {
  int32 input[][5] = {
      {1, 2, 3, 4, 1},
      {1, 2, 3, 4, 2},
      {1, 2, 3, 4, 3},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 1},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 2},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 3},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 4},
  };
  for (auto(&entry) : input) {
    auto op = Simd128<int32>::list(entry[0], entry[1], entry[2], entry[3]);
    auto countv = Simd128<int32>::list(entry[4], 0, entry[4], 1);
    EXPECT_EQ(simulated::shl_simd(op, countv), operator<<(op, countv))
        << "shl_simd(" << op << ", " << countv << ")";
  }
  EXPECT_EQ((Simd128<int32>::list(2, 4, 6, 8)),
            operator<<(Simd128<int32>::list(1, 2, 3, 4), 1));
}

TEST(DimsumTest, ShiftRight) {
  int32 input[][5] = {
      {1, 2, 3, 4, 1},
      {1, 2, 3, 4, 2},
      {1, 2, 3, 4, 3},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 1},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 2},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 3},
      {bitconvert(0xffffffff), bitconvert(0x8fffffff), 3, 4, 4},
  };
  for (auto(&entry) : input) {
    auto op = Simd128<int32>::list(entry[0], entry[1], entry[2], entry[3]);
    auto countv = Simd128<int32>::list(entry[4], 0, entry[4], 1);
    EXPECT_EQ(simulated::shr_simd(op, countv), operator>>(op, countv))
        << "shr_simd(" << op << ", " << countv << ")";
  }
  EXPECT_EQ((Simd128<int32>::list(0, 1, 1, 2)),
            operator>>(Simd128<int32>::list(1, 2, 3, 4), 1));
}

TEST(DimsumTest, BitAnd) {
  SIMD_BINARY_OP_TEST(int32, bit_and, &, boring_binary_op_test);
}

TEST(DimsumTest, BitOr) {
  SIMD_BINARY_OP_TEST(int32, bit_or, |, boring_binary_op_test);
}

TEST(DimsumTest, BitXor) {
  SIMD_BINARY_OP_TEST(int32, bit_xor, ^, boring_binary_op_test);
}

TEST(DimsumTest, BitNot) {
  SIMD_UNARY_OP_TEST(int32, bit_not, ~, boring_unary_op_test);
}

TEST(DimsumTest, CmpEq) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_eq, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_eq, boring_binary_op_test_float);
}

TEST(DimsumTest, CmpNe) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_ne, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_ne, boring_binary_op_test_float);
}

TEST(DimsumTest, CmpLt) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_lt, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_lt, boring_binary_op_test_float);
}

TEST(DimsumTest, CmpLe) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_le, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_le, boring_binary_op_test_float);
}

TEST(DimsumTest, CmpGt) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_gt, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_gt, boring_binary_op_test_float);
}

TEST(DimsumTest, CmpGe) {
  SIMD_BINARY_FREE_FUNC_TEST(int32, cmp_ge, boring_binary_op_test);
  SIMD_BINARY_FREE_FUNC_TEST(float, cmp_ge, boring_binary_op_test_float);
}

TEST(DimsumTest, Shuffle) {
  {
    auto lhs = Simd128<int32>::list(1, 2, 3, 4);
    auto rhs = Simd128<int32>::list(5, 6, 7, 8);

    EXPECT_EQ(2, (shuffle<-1, 1, -1, -1>(lhs, rhs))[1]);
    EXPECT_EQ(6, (shuffle<-1, -1, 5, -1>(lhs, rhs))[2]);

    EXPECT_EQ((simulated::shuffle<1, 2, 3, 4>(lhs, rhs)),
              (shuffle<1, 2, 3, 4>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<4, 3, 2, 1>(lhs, rhs)),
              (shuffle<4, 3, 2, 1>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<1, 3, 5, 7>(lhs, rhs)),
              (shuffle<1, 3, 5, 7>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<0, 2, 4, 6>(lhs, rhs)),
              (shuffle<0, 2, 4, 6>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<0, 2, 4, 6>(lhs, rhs)),
              (shuffle<0, 2, 4, 6>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<2, 2, 1, 5>(lhs, rhs)),
              (shuffle<2, 2, 1, 5>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<6, 3, 0, 7>(lhs, rhs)),
              (shuffle<6, 3, 0, 7>(lhs, rhs)));
  }

  {
    auto lhs = Simd128<float>::list(1., 2., 3., 4.);
    auto rhs = Simd128<float>::list(5., 6., 7., 8.);

    EXPECT_EQ((simulated::shuffle<1, 2, 3, 4>(lhs, rhs)),
              (shuffle<1, 2, 3, 4>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<4, 3, 2, 1>(lhs, rhs)),
              (shuffle<4, 3, 2, 1>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<1, 3, 5, 7>(lhs, rhs)),
              (shuffle<1, 3, 5, 7>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<0, 2, 4, 6>(lhs, rhs)),
              (shuffle<0, 2, 4, 6>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<0, 2, 4, 6>(lhs, rhs)),
              (shuffle<0, 2, 4, 6>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<2, 2, 1, 5>(lhs, rhs)),
              (shuffle<2, 2, 1, 5>(lhs, rhs)));

    EXPECT_EQ((simulated::shuffle<6, 3, 0, 7>(lhs, rhs)),
              (shuffle<6, 3, 0, 7>(lhs, rhs)));
  }
}

TEST(DimsumTest, MulSum) {
  EXPECT_EQ((Simd128<int32>::list(1, 14, 43, 88)),
            (mul_sum(Simd128<int16>::list(0, 1, 2, 3, 4, 5, 6, 7),
                     Simd128<int16>::list(0, 1, 2, 3, 4, 5, 6, 7),
                     Simd128<int32>::list(0, 1, 2, 3))));
}

TEST(DimsumTest, Max) {
  EXPECT_EQ(
      (Simd128<int32>::list(2, 3, 5, 6)),
      max(Simd128<int32>::list(2, 3, 2, 6), Simd128<int32>::list(0, 1, 5, 3)));
}

TEST(DimsumTest, Min) {
  EXPECT_EQ(
      (Simd128<int32>::list(0, 1, 2, 3)),
      min(Simd128<int32>::list(2, 3, 2, 6), Simd128<int32>::list(0, 1, 5, 3)));
}

TEST(DimsumTest, HorizontalSum) {
  EXPECT_EQ((Simd128<int64>::list(3, 7)),
            (reduce_add<int64, 2>(Simd128<int32>::list(1, 2, 3, 4))));
  EXPECT_EQ((Simd128<int64>::list(1, 1)),
            (reduce_add<int64, 2>(Simd128<int32>::list(-1, 2, -3, 4))));
  EXPECT_EQ((Simd128<int64>::list(-3, 7)),
            (reduce_add<int64, 2>(Simd128<int32>::list(-1, -2, 3, 4))));
  EXPECT_EQ((Simd128<uint64>::list(60, 220)),
            (reduce_add<uint64, 2>(Simd128<uint8>::list(
                1, 2, 3, 4, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34))));
  EXPECT_EQ((Simd128<uint32>::list(10, 50, 90, 130)),
            (reduce_add<uint32, 4>(Simd128<uint8>::list(
                1, 2, 3, 4, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34))));
}

TEST(DimsumTest, Fma) {
  EXPECT_EQ((Simd128<float>::list(2, -2, 2, 8)),
            fma(Simd128<float>::list(1, 2, -1, -2),
                Simd128<float>::list(1, -2, 1, -2),
                Simd128<float>::list(1, 2, 3, 4)));
}

TEST(DimsumTest, RoundToEven) {
  EXPECT_EQ(NativeSimd<float>(-1.0), round(NativeSimd<float>(-1.499)));
  EXPECT_EQ(NativeSimd<float>(-2.0), round(NativeSimd<float>(-1.5)));
  EXPECT_EQ(NativeSimd<float>(-2.0), round(NativeSimd<float>(-2.5)));
  EXPECT_EQ(NativeSimd<float>(0.0), round(NativeSimd<float>(0.5)));
  EXPECT_EQ(NativeSimd<float>(2.0), round(NativeSimd<float>(1.51)));
}

TEST(DimsumTest, RoundEvenToInteger) {
  EXPECT_EQ(NativeSimd<int32>(-1),
            round_to_integer<int32>(NativeSimd<float>(-1.499)));
  EXPECT_EQ(NativeSimd<int32>(-2),
            round_to_integer<int32>(NativeSimd<float>(-1.5)));
  EXPECT_EQ(NativeSimd<int32>(0),
            round_to_integer<int32>(NativeSimd<float>(0.499)));
  EXPECT_EQ(NativeSimd<int32>(2),
            round_to_integer<int32>(NativeSimd<float>(1.51)));
}

template <typename IntSimd, typename FloatSimd>
void TestIntFloatStaticSimdCast() {
  using IntType = typename IntSimd::value_type;
  using FloatType = typename FloatSimd::value_type;

  // int to float
  if (std::is_signed<IntType>::value) {
    EXPECT_EQ(FloatSimd(-16.), static_simd_cast<FloatType>(IntSimd(-16)));
    EXPECT_EQ(FloatSimd(-17.), static_simd_cast<FloatType>(IntSimd(-17)));
    EXPECT_EQ(FloatSimd(0.), static_simd_cast<FloatType>(IntSimd(-0)));
  }
  EXPECT_EQ(FloatSimd(16.), static_simd_cast<FloatType>(IntSimd(16)));
  EXPECT_EQ(FloatSimd(17.), static_simd_cast<FloatType>(IntSimd(17)));
  EXPECT_EQ(FloatSimd(0.), static_simd_cast<FloatType>(IntSimd(0)));

  constexpr auto min_int = std::numeric_limits<IntType>::min();
  constexpr auto max_int = std::numeric_limits<IntType>::max();
  EXPECT_EQ(FloatSimd(min_int), static_simd_cast<FloatType>(IntSimd(min_int)));
  EXPECT_EQ(FloatSimd(max_int), static_simd_cast<FloatType>(IntSimd(max_int)));

  // float to int
  if (std::is_signed<IntType>::value) {
    EXPECT_EQ(IntSimd(-15), static_simd_cast<IntType>(FloatSimd(-15.9)));
    EXPECT_EQ(IntSimd(-16), static_simd_cast<IntType>(FloatSimd(-16.0)));
    EXPECT_EQ(IntSimd(-16), static_simd_cast<IntType>(FloatSimd(-16.1)));
    EXPECT_EQ(IntSimd(-16), static_simd_cast<IntType>(FloatSimd(-16.4)));
    EXPECT_EQ(IntSimd(-16), static_simd_cast<IntType>(FloatSimd(-16.5)));
    EXPECT_EQ(IntSimd(-16), static_simd_cast<IntType>(FloatSimd(-16.9)));
    EXPECT_EQ(IntSimd(-17), static_simd_cast<IntType>(FloatSimd(-17.0)));
  }
  EXPECT_EQ(IntSimd(15), static_simd_cast<IntType>(FloatSimd(15.9)));
  EXPECT_EQ(IntSimd(16), static_simd_cast<IntType>(FloatSimd(16.0)));
  EXPECT_EQ(IntSimd(16), static_simd_cast<IntType>(FloatSimd(16.1)));
  EXPECT_EQ(IntSimd(16), static_simd_cast<IntType>(FloatSimd(16.4)));
  EXPECT_EQ(IntSimd(16), static_simd_cast<IntType>(FloatSimd(16.5)));
  EXPECT_EQ(IntSimd(16), static_simd_cast<IntType>(FloatSimd(16.9)));
  EXPECT_EQ(IntSimd(17), static_simd_cast<IntType>(FloatSimd(17.0)));

  EXPECT_EQ(IntSimd(0), static_simd_cast<IntType>(FloatSimd(0.)));
  EXPECT_EQ(IntSimd(0), static_simd_cast<IntType>(FloatSimd(-0.)));
}

template <typename ToSimd, typename FromSimd>
void TestSignStaticSimdCast() {
  using FromType = typename FromSimd::value_type;
  using ToType = typename ToSimd::value_type;

  EXPECT_EQ(ToSimd(0), static_simd_cast<ToType>(FromSimd(0)));
  EXPECT_EQ(ToSimd(20), static_simd_cast<ToType>(FromSimd(20)));

  if (std::is_signed<FromType>::value) {
    constexpr auto max_int = std::numeric_limits<FromType>::max();
    EXPECT_EQ(ToSimd(max_int), static_simd_cast<ToType>(FromSimd(max_int)));
  }
}

TEST(DimsumTest, StaticSimdCast) {
  TestIntFloatStaticSimdCast<NativeSimd<int32>, NativeSimd<float>>();
  TestIntFloatStaticSimdCast<NativeSimd<uint32>, NativeSimd<float>>();
  TestIntFloatStaticSimdCast<NativeSimd<int64>, NativeSimd<double>>();
  TestIntFloatStaticSimdCast<NativeSimd<uint64>, NativeSimd<double>>();

  TestSignStaticSimdCast<NativeSimd<int8>, NativeSimd<uint8>>();
  TestSignStaticSimdCast<NativeSimd<uint8>, NativeSimd<int8>>();
  TestSignStaticSimdCast<NativeSimd<int16>, NativeSimd<uint16>>();
  TestSignStaticSimdCast<NativeSimd<uint16>, NativeSimd<int16>>();
  TestSignStaticSimdCast<NativeSimd<int32>, NativeSimd<uint32>>();
  TestSignStaticSimdCast<NativeSimd<uint32>, NativeSimd<int32>>();
  TestSignStaticSimdCast<NativeSimd<int64>, NativeSimd<uint64>>();
  TestSignStaticSimdCast<NativeSimd<uint64>, NativeSimd<int64>>();
}

template <typename SimdType, size_t num_elements>
struct TestZipImpl {
  static void Apply() {}
};

template <typename T, typename Abi>
struct TestZipImpl<Simd<T, Abi>, 2> {
  static void Apply() {
    using SrcSimdType = Simd<T, Abi>;
    using DestSimdType = ResizeBy<Simd<T, Abi>, 2>;
    EXPECT_EQ(DestSimdType::list(0, 2, 1, 3),
              zip(SrcSimdType::list(0, 1), SrcSimdType::list(2, 3)));
  }
};

template <typename T, typename Abi>
struct TestZipImpl<Simd<T, Abi>, 4> {
  using SrcSimdType = Simd<T, Abi>;
  using DestSimdType = ResizeBy<Simd<T, Abi>, 2>;
  static void Apply() {
    EXPECT_EQ(
        DestSimdType::list(0, 4, 1, 5, 2, 6, 3, 7),
        zip(SrcSimdType::list(0, 1, 2, 3), SrcSimdType::list(4, 5, 6, 7)));
  }
};

template <typename T, typename Abi>
struct TestZipImpl<Simd<T, Abi>, 8> {
  static void Apply() {
    using SrcSimdType = Simd<T, Abi>;
    using DestSimdType = ResizeBy<Simd<T, Abi>, 2>;
    EXPECT_EQ(DestSimdType::list(0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14,
                                 7, 15),
              zip(SrcSimdType::list(0, 1, 2, 3, 4, 5, 6, 7),
                  SrcSimdType::list(8, 9, 10, 11, 12, 13, 14, 15)));
  }
};

template <typename T, typename Abi>
struct TestZipImpl<Simd<T, Abi>, 16> {
  static void Apply() {
    using SrcSimdType = Simd<T, Abi>;
    using DestSimdType = ResizeBy<Simd<T, Abi>, 2>;
    EXPECT_EQ(DestSimdType::list(0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6,
                                 22, 7, 23, 8, 24, 9, 25, 10, 26, 11, 27, 12,
                                 28, 13, 29, 14, 30, 15, 31),
              zip(SrcSimdType::list(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                    13, 14, 15),
                  SrcSimdType::list(16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                                    27, 28, 29, 30, 31)));
  }
};

template <typename T, typename Abi>
struct TestZipImpl<Simd<T, Abi>, 32> {
  static void Apply() {
    using SrcSimdType = Simd<T, Abi>;
    using DestSimdType = ResizeBy<Simd<T, Abi>, 2>;
    EXPECT_EQ(DestSimdType::list(
                  0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39, 8, 40,
                  9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47, 16, 48,
                  17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55, 24,
                  56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63),
              zip(SrcSimdType::list(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                    24, 25, 26, 27, 28, 29, 30, 31),
                  SrcSimdType::list(32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                    43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                    54, 55, 56, 57, 58, 59, 60, 61, 62, 63)));
  }
};

template <typename SimdType>
void TestZip() {
  TestZipImpl<SimdType, SimdType::size()>::Apply();
}

TEST(DimsumTest, Zip) {
  TestZip<NativeSimd<int8>>();
  TestZip<NativeSimd<int16>>();
  TestZip<NativeSimd<int32>>();
  TestZip<NativeSimd<int64>>();
  TestZip<NativeSimd<uint8>>();
  TestZip<NativeSimd<uint16>>();
  TestZip<NativeSimd<uint32>>();
  TestZip<NativeSimd<uint64>>();
  TestZip<NativeSimd<float>>();
  TestZip<NativeSimd<double>>();
}

template <typename SrcSimdType>
void TestMulWidened() {
  using T = typename SrcSimdType::value_type;
  using Abi = typename SrcSimdType::abi_type;
  using DestSimdType = ScaleElemBy<Simd<T, Abi>, 2>;

  EXPECT_EQ(DestSimdType(15), mul_widened(SrcSimdType(3), SrcSimdType(5)));
  {
    auto mx = std::numeric_limits<T>::max();
    ScaleBy<T, 2> ans = mx;
    ans *= mx;
    EXPECT_EQ(DestSimdType(ans), mul_widened(SrcSimdType(mx), SrcSimdType(mx)));
  }
  if (std::is_signed<T>::value) {
    auto mi = std::numeric_limits<T>::min();
    auto mx = std::numeric_limits<T>::max();
    ScaleBy<T, 2> ans = mx;
    ans *= mi;
    EXPECT_EQ(DestSimdType(ans), mul_widened(SrcSimdType(mi), SrcSimdType(mx)));
  }
}

TEST(DimsumTest, MulWidened) {
  TestMulWidened<NativeSimd<int8>>();
  TestMulWidened<NativeSimd<int16>>();
  TestMulWidened<NativeSimd<int32>>();
  TestMulWidened<NativeSimd<uint8>>();
  TestMulWidened<NativeSimd<uint16>>();
  TestMulWidened<NativeSimd<uint32>>();
}

template <typename SimdType>
void TestConcat() {
  using T = typename SimdType::value_type;
  using Abi = typename SimdType::abi_type;
  constexpr auto size = SimdType::size();
  auto get_iota = [](int start) {
    return [start](int index) { return start + index; };
  };

  EXPECT_EQ((ResizeBy<Simd<T, Abi>, 2>(get_iota(0))),
            concat(SimdType(get_iota(0)), SimdType(get_iota(size))));
  EXPECT_EQ((ResizeBy<Simd<T, Abi>, 4>(get_iota(0))),
            concat(SimdType(get_iota(0)), SimdType(get_iota(size)),
                   SimdType(get_iota(size * 2)), SimdType(get_iota(size * 3))));
}

TEST(DimsumTest, Concat) {
  TestConcat<NativeSimd<int8>>();
  TestConcat<NativeSimd<int16>>();
  TestConcat<NativeSimd<int32>>();
  TestConcat<NativeSimd<int64>>();
  TestConcat<NativeSimd<uint8>>();
  TestConcat<NativeSimd<uint16>>();
  TestConcat<NativeSimd<uint32>>();
  TestConcat<NativeSimd<uint64>>();
  TestConcat<NativeSimd<float>>();
  TestConcat<NativeSimd<double>>();
}

template <typename SimdType>
void TestSplit() {
  using T = typename SimdType::value_type;
  using Abi = typename SimdType::abi_type;
  constexpr auto size = SimdType::size();
  auto get_iota = [](int start) {
    return [start](int index) { return start + index; };
  };

  {
    using ResType = ResizeBy<Simd<T, Abi>, 1, 2>;
    auto arr = split(SimdType(get_iota(0)));
    EXPECT_EQ(ResType(get_iota(0)), arr[0]);
    EXPECT_EQ(ResType(get_iota(size / 2)), arr[1]);
  }
  {
    using InputType = ResizeBy<Simd<T, Abi>, 4>;
    auto arr = split<4>(InputType(get_iota(0)));
    EXPECT_EQ(SimdType(get_iota(0)), arr[0]);
    EXPECT_EQ(SimdType(get_iota(size * 1)), arr[1]);
    EXPECT_EQ(SimdType(get_iota(size * 2)), arr[2]);
    EXPECT_EQ(SimdType(get_iota(size * 3)), arr[3]);
  }
}

TEST(DimsumTest, Split) {
  TestSplit<NativeSimd<int8>>();
  TestSplit<NativeSimd<int16>>();
  TestSplit<NativeSimd<int32>>();
  TestSplit<NativeSimd<int64>>();
  TestSplit<NativeSimd<uint8>>();
  TestSplit<NativeSimd<uint16>>();
  TestSplit<NativeSimd<uint32>>();
  TestSplit<NativeSimd<uint64>>();
  TestSplit<NativeSimd<float>>();
  TestSplit<NativeSimd<double>>();
}

TEST(DimsumTest, TestSimd64) {
  static_assert(
      std::is_same<ResizeBy<Simd128<int32>, 1, 2>, Simd64<int32>>::value, "");
  static_assert(std::is_same<ResizeBy<Simd64<int32>, 2>, Simd128<int32>>::value,
                "");
}

TEST(DimsumTest, Reduce) {
  EXPECT_EQ(10.f, reduce(Simd128<float>::list(1.f, 2.f, 3.f, 4.f)));
  EXPECT_EQ(10, reduce(Simd128<int32>::list(1, 2, 3, 4)));
  // TODO(maskray) After dropping C++11 support, change bit_xor<int32> to
  // bit_xor<>.
  EXPECT_EQ(15,
            reduce(Simd128<int32>::list(1, 2, 4, 8), std::bit_xor<int32>()));
}

TEST(DimsumTest, ReduceAdd) {
  EXPECT_EQ(36, reduce_add(Simd128<int16>::list(1, 2, 3, 4, 5, 6, 7, 8))[0]);
  EXPECT_EQ(10, reduce_add(Simd128<int32>::list(1, 2, 3, 4))[0]);
  EXPECT_EQ(Simd128<int32>::list(3, 7, 11, 15),
            (reduce_add<int32, 4>(concat(Simd128<int32>::list(1, 2, 3, 4),
                                         Simd128<int32>::list(5, 6, 7, 8)))));

  EXPECT_EQ(
      Simd128<int32>::list(10, 26, 42, 58),
      (reduce_add<int32, 4>(concat(Simd128<int32>::list(1, 2, 3, 4),
                                   Simd128<int32>::list(5, 6, 7, 8),
                                   Simd128<int32>::list(9, 10, 11, 12),
                                   Simd128<int32>::list(13, 14, 15, 16)))));

  EXPECT_EQ(-16, (reduce_add<int64, 1>(Simd128<int8>(-1))[0]));
  EXPECT_EQ(255 * 16, (reduce_add<uint64, 1>(Simd128<uint8>(255))[0]));
  EXPECT_EQ(-8, (reduce_add<int64, 1>(Simd128<int16>(-1))[0]));
  EXPECT_EQ(65535 * 8, (reduce_add<uint64, 1>(Simd128<uint16>(65535))[0]));
  EXPECT_EQ(-4, (reduce_add<int64, 1>(Simd128<int32>(-1))[0]));
  EXPECT_EQ(4294967295LL * 4,
            (reduce_add<uint64, 1>(Simd128<uint32>(4294967295))[0]));
}

#undef SIMD_BINARY_OP_ASSIGN_TEST
#undef SIMD_BINARY_OP_TEST
#undef SIMD_BINARY_FUNC_TEST

}  // namespace
}  // namespace dimsum
