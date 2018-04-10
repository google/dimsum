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

#include "dimsum_x86.h"
#include <numeric>
#include "simulated.h"
#include "gtest/gtest.h"

namespace dimsum {
namespace {

TEST(DimsumX86Test, Maddubs) {
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(510) ==
             simulated::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(1))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(510) ==
             x86::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(1))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(-510) ==
             simulated::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(-1))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(-510) ==
             x86::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(-1))));
  EXPECT_TRUE(all_of(
      NativeSimd<int16>(32767) ==
      simulated::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(127))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(32767) ==
             x86::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(127))));
  EXPECT_TRUE(all_of(
      NativeSimd<int16>(-32768) ==
      simulated::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(-128))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(-32768) ==
             x86::maddubs(NativeSimd<uint8>(255), NativeSimd<int8>(-128))));
  EXPECT_TRUE(all_of(
      NativeSimd<int16>(32258) ==
      simulated::maddubs(NativeSimd<uint8>(127), NativeSimd<int8>(127))));
  EXPECT_TRUE(
      all_of(NativeSimd<int16>(32258) ==
             x86::maddubs(NativeSimd<uint8>(127), NativeSimd<int8>(127))));
}

TEST(DimsumX86Test, Movemask) {
  {
    auto a = NativeSimd<int16>(1);
    EXPECT_EQ(0, simulated::movemask(a));
    EXPECT_EQ(0, x86::movemask(a));
    a[0] = -32768;
    EXPECT_EQ(1, simulated::movemask(a));
    EXPECT_EQ(1, x86::movemask(a));
    a[1] = -32768;
    EXPECT_EQ(3, simulated::movemask(a));
    EXPECT_EQ(3, x86::movemask(a));
  }
  {
    auto a = NativeSimd<int8>(-2);
    // There are no _mm512_movemask_epi8 so a.size() cannot be 64.
    if (a.size() == 16) {
      EXPECT_EQ(65536 - 1, simulated::movemask(a));
      EXPECT_EQ(65536 - 1, x86::movemask(a));
      a[0] = 0;
      EXPECT_EQ(65536 - 2, simulated::movemask(a));
      EXPECT_EQ(65536 - 2, x86::movemask(a));
    } else if (a.size() == 32) {
      EXPECT_EQ(-1, simulated::movemask(a));
      EXPECT_EQ(-1, x86::movemask(a));
      a[0] = 0;
      EXPECT_EQ(-2, simulated::movemask(a));
      EXPECT_EQ(-2, x86::movemask(a));
    }
  }
  {
    auto a = NativeSimd<int32>(-2);
    EXPECT_EQ((1 << a.size()) - 1, simulated::movemask(a));
    EXPECT_EQ((1 << a.size()) - 1, x86::movemask(a));
  }
  {
    auto a = NativeSimd<uint64>(-3);
    EXPECT_EQ((1 << a.size()) - 1, simulated::movemask(a));
    EXPECT_EQ((1 << a.size()) - 1, x86::movemask(a));
  }
}

}  // namespace
}  // namespace dimsum
