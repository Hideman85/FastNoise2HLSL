#ifndef FASTNOISEHLSLHELPERS_H
#define FASTNOISEHLSLHELPERS_H

#include "FastNoiseConstants.h"

// Using Variadic macros for function overloading rewrite
#define NF(...) NOT_A_FUNCTION(__VA_ARGS__)
#define _GET_OVERRIDE(_1, _2, _3, _4, _5, _6, _7, NAME,...) NAME
/* Usage
float myFunc2Args(float a, float b) { return a * b }
float myFunc4Args(float a, float b, float c, float d) { return a * b + c * d }

#define MY_MACRO2(a, b) myFunc2Args(a, b)
#define MY_MACRO4(a, b, c, d) myFunc4Args(a, b)
#define MY_MACRO(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, NF, MY_MACRO4, NF, MY_MACRO2)(__VA_ARGS__)

MY_MACRO(a			) // Expends to NOT_A_FUNCTION(a)  		=> Will throw compile time
MY_MACRO(a, b		) // Expends to MY_MACRO2(a, b)
MY_MACRO(a, b, c	) // Expends to NOT_A_FUNCTION(a, b, c) => Will throw compile time
MY_MACRO(a, b, c, d	) // Expends to MY_MACRO4(a, b, c, d)
*/

// Define macros used by FastNoise2 implementation
#define FnPrimes_X 501125321
#define FnPrimes_Y 1136930381
#define FnPrimes_Z 1720413743
#define FnPrimes_W 1066037191

#define float32v float
#define int32v int
#define size_t int
#define mask32v bool

#define FS_Convertf32_i32 int
#define FS_Converti32_f32 float

#define FS_MaskedAdd_f32(a, b, m) ((m) ? ((a) + (b)) : (a))
#define FS_MaskedAdd_i32(a, b, m) FS_MaskedAdd_f32(a, b, m)
#define FS_NMaskedAdd_i32(a, b, m) ((m) ? (a) : ((a) + (b)))
#define FS_MaskedSub_f32(a, b, m) ((m) ? ((a) - (b)) : (a))
#define FS_NMaskedSub_f32(a, b, m) ((m) ? (a) : ((a) - (b)))
#define FS_FMulAdd_f32(a, b, c) (((a) * (b)) + (c))
#define FS_FNMulAdd_f32(a, b, c) (-((a) * (b)) + (c))
#define FS_Max_f32(a, b) ((a) > (b) ? (a) : (b))
#define FS_BitwiseAndNot_m32(a, b) ((a) && !(b))

#define _mm512_permutex2var_ps(a, idx, b) ((idx & 0x80000000) != 0 ? b : a)

#endif //FASTNOISEHLSLHELPERS_H
