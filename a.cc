#include "a.pre.h"

using uint8x16 = dimsum::NativeSimd<uint8_t>;

void Foo(uint8x16 z, uint8x16 v0, uint8x16 v1, unsigned char* p) {
  dimsum::where(cmp_eq(z, v0) || cmp_eq(z, v1), z) ^= v0 ^ v1;
  z.copy_to(p, dimsum::flags::vector_aligned);
}

void FooX86(__m128i z, __m128i v0, __m128i v1, uint8_t* p) {
  auto mask = _mm_or_si128(_mm_cmpeq_epi8(z, v0), _mm_cmpeq_epi8(z, v1));
  _mm_store_si128((__m128i*)p, _mm_xor_si128(z, _mm_and_si128(mask, _mm_xor_si128(v0, v1))));
}
