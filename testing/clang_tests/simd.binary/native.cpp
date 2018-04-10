// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %clang -std=c++11 -S -O2 -mavx2 -o - %s | FileCheck -check-prefix=CHECK-AVX2 %s
#include "dimsum.h"
using namespace dimsum;

extern "C" {

// CHECK-AVX2-LABEL: operatorADD_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorADD_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorADD_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddd %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorADD_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddq %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorADD_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorADD_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorADD_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddd %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorADD_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpaddq %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorADD_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_8:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vaddps %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<float> operatorADD_8(NativeSimd<float> a, NativeSimd<float> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorADD_9:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vaddpd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<double> operatorADD_9(NativeSimd<double> a, NativeSimd<double> b) {
  return operator+(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubb %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorSUB_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubw %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorSUB_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorSUB_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorSUB_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubb %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorSUB_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubw %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorSUB_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorSUB_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsubq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorSUB_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_8:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vsubps %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<float> operatorSUB_8(NativeSimd<float> a, NativeSimd<float> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorSUB_9:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vsubpd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<double> operatorSUB_9(NativeSimd<double> a, NativeSimd<double> b) {
  return operator-(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm0, %xmm2
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm2, %ymm2
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm1, %xmm3
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm3, %ymm3
// CHECK-AVX2-NEXT:    vpmullw %ymm2, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm2, %xmm3
// CHECK-AVX2-NEXT:    vmovdqa {{.*#+}} xmm4 = <0,2,4,6,8,10,12,14,u,u,u,u,u,u,u,u>
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm3, %xmm3
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpcklqdq {{.*#+}} xmm2 = xmm2[0],xmm3[0]
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm0, %ymm0
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpmullw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm0, %xmm1
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm1, %xmm1
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm0, %xmm0
// CHECK-AVX2-NEXT:    vpunpcklqdq {{.*#+}} xmm0 = xmm0[0],xmm1[0]
// CHECK-AVX2-NEXT:    vinserti128 $1, %xmm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorMUL_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpmullw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorMUL_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpmulld %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorMUL_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsrlq $32, %ymm1, %ymm2
// CHECK-AVX2-NEXT:    vpmuludq %ymm0, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpsrlq $32, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpmuludq %ymm3, %ymm1, %ymm3
// CHECK-AVX2-NEXT:    vpaddq %ymm2, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vpsllq $32, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpmuludq %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    vpaddq %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorMUL_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm0, %xmm2
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm2, %ymm2
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm1, %xmm3
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm3, %ymm3
// CHECK-AVX2-NEXT:    vpmullw %ymm2, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm2, %xmm3
// CHECK-AVX2-NEXT:    vmovdqa {{.*#+}} xmm4 = <0,2,4,6,8,10,12,14,u,u,u,u,u,u,u,u>
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm3, %xmm3
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpcklqdq {{.*#+}} xmm2 = xmm2[0],xmm3[0]
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm0, %ymm0
// CHECK-AVX2-NEXT:    vpmovsxbw %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpmullw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    vextracti128 $1, %ymm0, %xmm1
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm1, %xmm1
// CHECK-AVX2-NEXT:    vpshufb %xmm4, %xmm0, %xmm0
// CHECK-AVX2-NEXT:    vpunpcklqdq {{.*#+}} xmm0 = xmm0[0],xmm1[0]
// CHECK-AVX2-NEXT:    vinserti128 $1, %xmm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorMUL_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpmullw %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorMUL_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpmulld %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorMUL_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsrlq $32, %ymm1, %ymm2
// CHECK-AVX2-NEXT:    vpmuludq %ymm0, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpsrlq $32, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpmuludq %ymm3, %ymm1, %ymm3
// CHECK-AVX2-NEXT:    vpaddq %ymm2, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vpsllq $32, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpmuludq %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    vpaddq %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorMUL_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_8:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmulps %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<float> operatorMUL_8(NativeSimd<float> a, NativeSimd<float> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorMUL_9:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmulpd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<double> operatorMUL_9(NativeSimd<double> a, NativeSimd<double> b) {
  return operator*(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorAND_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorAND_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorAND_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorAND_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorAND_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorAND_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorAND_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorAND_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vandps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorAND_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator&(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorXOR_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorXOR_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorXOR_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorXOR_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorXOR_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorXOR_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorXOR_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorXOR_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vxorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorXOR_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator^(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorOR_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorOR_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorOR_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorOR_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorOR_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorOR_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorOR_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorOR_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vorps %ymm0, %ymm1, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorOR_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator|(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $4, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsllw $2, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorSHL_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpxor %xmm2, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm3 = ymm1[4],ymm2[4],ymm1[5],ymm2[5],ymm1[6],ymm2[6],ymm1[7],ymm2[7],ymm1[12],ymm2[12],ymm1[13],ymm2[13],ymm1[14],ymm2[14],ymm1[15],ymm2[15]
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm4 = ymm2[4],ymm0[4],ymm2[5],ymm0[5],ymm2[6],ymm0[6],ymm2[7],ymm0[7],ymm2[12],ymm0[12],ymm2[13],ymm0[13],ymm2[14],ymm0[14],ymm2[15],ymm0[15]
// CHECK-AVX2-NEXT:    vpsllvd %ymm3, %ymm4, %ymm3
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm1 = ymm1[0],ymm2[0],ymm1[1],ymm2[1],ymm1[2],ymm2[2],ymm1[3],ymm2[3],ymm1[8],ymm2[8],ymm1[9],ymm2[9],ymm1[10],ymm2[10],ymm1[11],ymm2[11]
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm0 = ymm2[0],ymm0[0],ymm2[1],ymm0[1],ymm2[2],ymm0[2],ymm2[3],ymm0[3],ymm2[8],ymm0[8],ymm2[9],ymm0[9],ymm2[10],ymm0[10],ymm2[11],ymm0[11]
// CHECK-AVX2-NEXT:    vpsllvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackusdw %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorSHL_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorSHL_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllvq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorSHL_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $4, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsllw $2, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorSHL_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpxor %xmm2, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm3 = ymm1[4],ymm2[4],ymm1[5],ymm2[5],ymm1[6],ymm2[6],ymm1[7],ymm2[7],ymm1[12],ymm2[12],ymm1[13],ymm2[13],ymm1[14],ymm2[14],ymm1[15],ymm2[15]
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm4 = ymm2[4],ymm0[4],ymm2[5],ymm0[5],ymm2[6],ymm0[6],ymm2[7],ymm0[7],ymm2[12],ymm0[12],ymm2[13],ymm0[13],ymm2[14],ymm0[14],ymm2[15],ymm0[15]
// CHECK-AVX2-NEXT:    vpsllvd %ymm3, %ymm4, %ymm3
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm1 = ymm1[0],ymm2[0],ymm1[1],ymm2[1],ymm1[2],ymm2[2],ymm1[3],ymm2[3],ymm1[8],ymm2[8],ymm1[9],ymm2[9],ymm1[10],ymm2[10],ymm1[11],ymm2[11]
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm0 = ymm2[0],ymm0[0],ymm2[1],ymm0[1],ymm2[2],ymm0[2],ymm2[3],ymm0[3],ymm2[8],ymm0[8],ymm2[9],ymm0[9],ymm2[10],ymm0[10],ymm2[11],ymm0[11]
// CHECK-AVX2-NEXT:    vpsllvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackusdw %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorSHL_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorSHL_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllvq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorSHL_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_0:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpunpckhbw {{.*#+}} ymm2 = ymm0[8],ymm1[8],ymm0[9],ymm1[9],ymm0[10],ymm1[10],ymm0[11],ymm1[11],ymm0[12],ymm1[12],ymm0[13],ymm1[13],ymm0[14],ymm1[14],ymm0[15],ymm1[15],ymm0[24],ymm1[24],ymm0[25],ymm1[25],ymm0[26],ymm1[26],ymm0[27],ymm1[27],ymm0[28],ymm1[28],ymm0[29],ymm1[29],ymm0[30],ymm1[30],ymm0[31],ymm1[31]
// CHECK-AVX2-NEXT:    vpunpckhbw {{.*#+}} ymm3 = ymm0[8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31]
// CHECK-AVX2-NEXT:    vpsraw $4, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpsraw $2, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpaddw %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpsraw $1, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpaddw %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vpsrlw $8, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpunpcklbw {{.*#+}} ymm1 = ymm0[0],ymm1[0],ymm0[1],ymm1[1],ymm0[2],ymm1[2],ymm0[3],ymm1[3],ymm0[4],ymm1[4],ymm0[5],ymm1[5],ymm0[6],ymm1[6],ymm0[7],ymm1[7],ymm0[16],ymm1[16],ymm0[17],ymm1[17],ymm0[18],ymm1[18],ymm0[19],ymm1[19],ymm0[20],ymm1[20],ymm0[21],ymm1[21],ymm0[22],ymm1[22],ymm0[23],ymm1[23]
// CHECK-AVX2-NEXT:    vpunpcklbw {{.*#+}} ymm0 = ymm0[0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23]
// CHECK-AVX2-NEXT:    vpsraw $4, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsraw $2, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpaddw %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsraw $1, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpaddw %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $8, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackuswb %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorSHR_0(NativeSimd<int8> a, NativeSimd<int8> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_1:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpxor %xmm2, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm3 = ymm1[4],ymm2[4],ymm1[5],ymm2[5],ymm1[6],ymm2[6],ymm1[7],ymm2[7],ymm1[12],ymm2[12],ymm1[13],ymm2[13],ymm1[14],ymm2[14],ymm1[15],ymm2[15]
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm4 = ymm2[4],ymm0[4],ymm2[5],ymm0[5],ymm2[6],ymm0[6],ymm2[7],ymm0[7],ymm2[12],ymm0[12],ymm2[13],ymm0[13],ymm2[14],ymm0[14],ymm2[15],ymm0[15]
// CHECK-AVX2-NEXT:    vpsravd %ymm3, %ymm4, %ymm3
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm1 = ymm1[0],ymm2[0],ymm1[1],ymm2[1],ymm1[2],ymm2[2],ymm1[3],ymm2[3],ymm1[8],ymm2[8],ymm1[9],ymm2[9],ymm1[10],ymm2[10],ymm1[11],ymm2[11]
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm0 = ymm2[0],ymm0[0],ymm2[1],ymm0[1],ymm2[2],ymm0[2],ymm2[3],ymm0[3],ymm2[8],ymm0[8],ymm2[9],ymm0[9],ymm2[10],ymm0[10],ymm2[11],ymm0[11]
// CHECK-AVX2-NEXT:    vpsravd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackusdw %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorSHR_1(NativeSimd<int16> a, NativeSimd<int16> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_2:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsravd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorSHR_2(NativeSimd<int32> a, NativeSimd<int32> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_3:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpbroadcastq {{.*#+}} ymm2 = [9223372036854775808,9223372036854775808,9223372036854775808,9223372036854775808]
// CHECK-AVX2-NEXT:    vpsrlvq %ymm1, %ymm2, %ymm3
// CHECK-AVX2-NEXT:    vpxor %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlvq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsubq %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorSHR_3(NativeSimd<int64> a, NativeSimd<int64> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_4:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpsrlw $4, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $2, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $1, %ymm0, %ymm2
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorSHR_4(NativeSimd<uint8> a, NativeSimd<uint8> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_5:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpxor %xmm2, %xmm2, %xmm2
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm3 = ymm1[4],ymm2[4],ymm1[5],ymm2[5],ymm1[6],ymm2[6],ymm1[7],ymm2[7],ymm1[12],ymm2[12],ymm1[13],ymm2[13],ymm1[14],ymm2[14],ymm1[15],ymm2[15]
// CHECK-AVX2-NEXT:    vpunpckhwd {{.*#+}} ymm4 = ymm2[4],ymm0[4],ymm2[5],ymm0[5],ymm2[6],ymm0[6],ymm2[7],ymm0[7],ymm2[12],ymm0[12],ymm2[13],ymm0[13],ymm2[14],ymm0[14],ymm2[15],ymm0[15]
// CHECK-AVX2-NEXT:    vpsrlvd %ymm3, %ymm4, %ymm3
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm1 = ymm1[0],ymm2[0],ymm1[1],ymm2[1],ymm1[2],ymm2[2],ymm1[3],ymm2[3],ymm1[8],ymm2[8],ymm1[9],ymm2[9],ymm1[10],ymm2[10],ymm1[11],ymm2[11]
// CHECK-AVX2-NEXT:    vpunpcklwd {{.*#+}} ymm0 = ymm2[0],ymm0[0],ymm2[1],ymm0[1],ymm2[2],ymm0[2],ymm2[3],ymm0[3],ymm2[8],ymm0[8],ymm2[9],ymm0[9],ymm2[10],ymm0[10],ymm2[11],ymm0[11]
// CHECK-AVX2-NEXT:    vpsrlvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrld $16, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackusdw %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorSHR_5(NativeSimd<uint16> a, NativeSimd<uint16> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_6:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsrlvd %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorSHR_6(NativeSimd<uint32> a, NativeSimd<uint32> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_7:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vpsrlvq %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorSHR_7(NativeSimd<uint64> a, NativeSimd<uint64> b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_8:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpbroadcastb %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm2
// CHECK-AVX2-NEXT:    vpsllw $4, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsllw $2, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorSHL_8(NativeSimd<int8> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_9:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpmovzxwq {{.*#+}} xmm1 = xmm1[0],zero,zero,zero,xmm1[1],zero,zero,zero
// CHECK-AVX2-NEXT:    vpsllw %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorSHL_9(NativeSimd<int16> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_10:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpslld %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorSHL_10(NativeSimd<int32> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_11:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    movslq %edi, %rax
// CHECK-AVX2-NEXT:    vmovq %rax, %xmm1
// CHECK-AVX2-NEXT:    vpsllq %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorSHL_11(NativeSimd<int64> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_12:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpbroadcastb %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm2
// CHECK-AVX2-NEXT:    vpsllw $4, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsllw $2, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpaddb %ymm0, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorSHL_12(NativeSimd<uint8> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_13:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpmovzxwq {{.*#+}} xmm1 = xmm1[0],zero,zero,zero,xmm1[1],zero,zero,zero
// CHECK-AVX2-NEXT:    vpsllw %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorSHL_13(NativeSimd<uint16> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_14:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpslld %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorSHL_14(NativeSimd<uint32> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHL_15:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    movslq %edi, %rax
// CHECK-AVX2-NEXT:    vmovq %rax, %xmm1
// CHECK-AVX2-NEXT:    vpsllq %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorSHL_15(NativeSimd<uint64> a, int b) {
  return operator<<(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_8:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpbroadcastb %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpunpckhbw {{.*#+}} ymm2 = ymm0[8],ymm1[8],ymm0[9],ymm1[9],ymm0[10],ymm1[10],ymm0[11],ymm1[11],ymm0[12],ymm1[12],ymm0[13],ymm1[13],ymm0[14],ymm1[14],ymm0[15],ymm1[15],ymm0[24],ymm1[24],ymm0[25],ymm1[25],ymm0[26],ymm1[26],ymm0[27],ymm1[27],ymm0[28],ymm1[28],ymm0[29],ymm1[29],ymm0[30],ymm1[30],ymm0[31],ymm1[31]
// CHECK-AVX2-NEXT:    vpunpckhbw {{.*#+}} ymm3 = ymm0[8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31]
// CHECK-AVX2-NEXT:    vpsraw $4, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpsraw $2, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpaddw %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpsraw $1, %ymm3, %ymm4
// CHECK-AVX2-NEXT:    vpaddw %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm4, %ymm3, %ymm2
// CHECK-AVX2-NEXT:    vpsrlw $8, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpunpcklbw {{.*#+}} ymm1 = ymm0[0],ymm1[0],ymm0[1],ymm1[1],ymm0[2],ymm1[2],ymm0[3],ymm1[3],ymm0[4],ymm1[4],ymm0[5],ymm1[5],ymm0[6],ymm1[6],ymm0[7],ymm1[7],ymm0[16],ymm1[16],ymm0[17],ymm1[17],ymm0[18],ymm1[18],ymm0[19],ymm1[19],ymm0[20],ymm1[20],ymm0[21],ymm1[21],ymm0[22],ymm1[22],ymm0[23],ymm1[23]
// CHECK-AVX2-NEXT:    vpunpcklbw {{.*#+}} ymm0 = ymm0[0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23]
// CHECK-AVX2-NEXT:    vpsraw $4, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsraw $2, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpaddw %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsraw $1, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpaddw %ymm1, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $8, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpackuswb %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int8> operatorSHR_8(NativeSimd<int8> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_9:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpmovzxwq {{.*#+}} xmm1 = xmm1[0],zero,zero,zero,xmm1[1],zero,zero,zero
// CHECK-AVX2-NEXT:    vpsraw %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int16> operatorSHR_9(NativeSimd<int16> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_10:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpsrad %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int32> operatorSHR_10(NativeSimd<int32> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_11:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    movslq %edi, %rax
// CHECK-AVX2-NEXT:    vmovq %rax, %xmm1
// CHECK-AVX2-NEXT:    vpbroadcastq {{.*#+}} ymm2 = [9223372036854775808,9223372036854775808,9223372036854775808,9223372036854775808]
// CHECK-AVX2-NEXT:    vpsrlq %xmm1, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpsrlq %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpxor %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsubq %ymm2, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<int64> operatorSHR_11(NativeSimd<int64> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_12:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpbroadcastb %xmm1, %ymm1
// CHECK-AVX2-NEXT:    vpsllw $5, %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm1, %ymm1, %ymm2
// CHECK-AVX2-NEXT:    vpsrlw $4, %ymm0, %ymm3
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm3, %ymm3
// CHECK-AVX2-NEXT:    vpblendvb %ymm1, %ymm3, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $2, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    vpsrlw $1, %ymm0, %ymm1
// CHECK-AVX2-NEXT:    vpand {{.*}}(%rip), %ymm1, %ymm1
// CHECK-AVX2-NEXT:    vpaddb %ymm2, %ymm2, %ymm2
// CHECK-AVX2-NEXT:    vpblendvb %ymm2, %ymm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint8> operatorSHR_12(NativeSimd<uint8> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_13:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpmovzxwq {{.*#+}} xmm1 = xmm1[0],zero,zero,zero,xmm1[1],zero,zero,zero
// CHECK-AVX2-NEXT:    vpsrlw %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint16> operatorSHR_13(NativeSimd<uint16> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_14:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    vmovd %edi, %xmm1
// CHECK-AVX2-NEXT:    vpsrld %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint32> operatorSHR_14(NativeSimd<uint32> a, int b) {
  return operator>>(a, b);
}

// CHECK-AVX2-LABEL: operatorSHR_15:
// CHECK-AVX2:       # %bb.0: # %entry
// CHECK-AVX2-NEXT:    movslq %edi, %rax
// CHECK-AVX2-NEXT:    vmovq %rax, %xmm1
// CHECK-AVX2-NEXT:    vpsrlq %xmm1, %ymm0, %ymm0
// CHECK-AVX2-NEXT:    retq
NativeSimd<uint64> operatorSHR_15(NativeSimd<uint64> a, int b) {
  return operator>>(a, b);
}

}