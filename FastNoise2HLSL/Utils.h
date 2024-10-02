#ifndef FASTNOISEHLSLUTILS_H
#define FASTNOISEHLSLUTILS_H

#include "./Definitions.h"

STATIC INLINE int FS_Floor_f32(float f) { return (f >= 0 ? int(f) : int(f) - 1); }

STATIC INLINE float FS_Round_f32(float f) { return (f >= 0) ? int(f + 0.5f) : int(f - 0.5f); }

STATIC INLINE float FS_Abs_f32(float f) { return f < 0 ? -f : f; }

STATIC INLINE float FS_InvSqrt_f32(float f) { return 1.f / sqrt(f); }

STATIC INLINE float FnUtils_Lerp(float a, float b, float t) { return a + t * (b - a); }

STATIC INLINE float FnUtils_InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

STATIC INLINE float FnUtils_InterpHermite(float t) { return t * t * (3 - 2 * t); }


///////////////////////////////////////////////////////////////////////
///    FnUtils::HashPrimes
///////////////////////////////////////////////////////////////////////

// For coherent noises
STATIC INLINE int HashPrimes2D(int seed, int xPrimed, int yPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    return (hash >> 15) ^ hash;
}

STATIC INLINE int HashPrimes3D(int seed, int xPrimed, int yPrimed, int zPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    return (hash >> 15) ^ hash;
}

STATIC INLINE int HashPrimes4D(int seed, int xPrimed, int yPrimed, int zPrimed, int wPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed ^ wPrimed;

    hash *= 0x27d4eb2d;
    return (hash >> 15) ^ hash;
}

#define FnUtils_HashPrimes2(h, x, y) HashPrimes2D(h, x, y)
#define FnUtils_HashPrimes3(h, x, y, z) HashPrimes3D(h, x, y, z)
#define FnUtils_HashPrimes4(h, x, y, z, w) HashPrimes4D(h, x, y, z, w)
#define FnUtils_HashPrimes(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, FnUtils_HashPrimes4, FnUtils_HashPrimes3, FnUtils_HashPrimes2, NF, NF)(__VA_ARGS__)



///////////////////////////////////////////////////////////////////////
///    FnUtils::HashPrimesHB
///////////////////////////////////////////////////////////////////////

// For value noises
STATIC INLINE int HashPrimesHB2D(int seed, int xPrimed, int yPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    return hash;
}

STATIC INLINE int HashPrimesHB3D(int seed, int xPrimed, int yPrimed, int zPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    return hash;
}

STATIC INLINE int HashPrimesHB4D(int seed, int xPrimed, int yPrimed, int zPrimed, int wPrimed)
{
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed ^ wPrimed;

    hash *= 0x27d4eb2d;
    return hash;
}

#define FnUtils_HashPrimesHB2(h, x, y) HashPrimesHB2D(h, x, y)
#define FnUtils_HashPrimesHB3(h, x, y, z) HashPrimesHB3D(h, x, y, z)
#define FnUtils_HashPrimesHB4(h, x, y, z, w) HashPrimesHB4D(h, x, y, z, w)
#define FnUtils_HashPrimesHB(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, FnUtils_HashPrimesHB4, FnUtils_HashPrimesHB3, FnUtils_HashPrimesHB2, NF, NF)(__VA_ARGS__)



///////////////////////////////////////////////////////////////////////
///    FnUtils::GetGradientDot
///////////////////////////////////////////////////////////////////////

STATIC const float gX2[] = { GRADIENT_VEC_2D_X };
STATIC const float gY2[] = { GRADIENT_VEC_2D_Y };

STATIC INLINE float GetGradientDot2D(int hash, float xd, float yd)
{
    int hasha15 = hash & int( 15 );
    xd *= gX2[hasha15];
    yd *= gY2[hasha15];
    return xd + yd;
}

STATIC const float gX3[] = { GRADIENT_VEC_3D_X };
STATIC const float gY3[] = { GRADIENT_VEC_3D_Y };
STATIC const float gZ3[] = { GRADIENT_VEC_3D_Z };

STATIC INLINE float GetGradientDot3D(int hash, float xd, float yd, float zd)
{
    int hasha15 = hash & int( 15 );
    xd *= gX3[hasha15];
    yd *= gY3[hasha15];
    zd *= gZ3[hasha15];
    return xd + yd + zd;
}

STATIC const float gX41[] = { GRADIENT_VEC_4D_X1 };
STATIC const float gX42[] = { GRADIENT_VEC_4D_X2 };
STATIC const float gY41[] = { GRADIENT_VEC_4D_Y1 };
STATIC const float gY42[] = { GRADIENT_VEC_4D_Y2 };
STATIC const float gZ41[] = { GRADIENT_VEC_4D_Z1 };
STATIC const float gZ42[] = { GRADIENT_VEC_4D_Z2 };
STATIC const float gW41[] = { GRADIENT_VEC_4D_W1 };
STATIC const float gW42[] = { GRADIENT_VEC_4D_W2 };

STATIC INLINE float GetGradientDot4D(int hash, float xd, float yd, float zd, float wd)
{
    int hasha15 = hash & int( 15 );
    xd *= _mm512_permutex2var_ps(gX41[hasha15], hasha15, gX42[hasha15]);
    yd *= _mm512_permutex2var_ps(gY41[hasha15], hasha15, gY42[hasha15]);
    zd *= _mm512_permutex2var_ps(gZ41[hasha15], hasha15, gZ42[hasha15]);
    wd *= _mm512_permutex2var_ps(gW41[hasha15], hasha15, gW42[hasha15]);
    return xd + yd + zd + wd;
}

#define FnUtils_GetGradientDot2(h, x, y) GetGradientDot2D(h, x, y)
#define FnUtils_GetGradientDot3(h, x, y, z) GetGradientDot3D(h, x, y, z)
#define FnUtils_GetGradientDot4(h, x, y, z, w) GetGradientDot4D(h, x, y, z, w)
#define FnUtils_GetGradientDot(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, FnUtils_GetGradientDot4, FnUtils_GetGradientDot3, FnUtils_GetGradientDot2, NF, NF)(__VA_ARGS__)



///////////////////////////////////////////////////////////////////////
///    FnUtils::GetGradientDotFancy
///////////////////////////////////////////////////////////////////////

STATIC const float gX_Fancy[] = { GRADIENT_FANCY_VEC_2D_X };
STATIC const float gY_Fancy[] = { GRADIENT_FANCY_VEC_2D_Y };

STATIC INLINE float32v GetGradientDotFancy( int32v hash, float32v fX, float32v fY )
{
    int32v index = FS_Convertf32_i32( FS_Converti32_f32( hash & int32v( 0x3FFFFF ) ) * float32v( 1.3333333333333333f ) ) & 0xF;

    float32v gX = gX_Fancy[index];
    float32v gY = gY_Fancy[index];

    return FS_FMulAdd_f32( gX, fX, fY * gY );
}

#define FnUtils_GetGradientDotFancy(h, x, y) GetGradientDotFancy(h, x, y)



///////////////////////////////////////////////////////////////////////
///    FnUtils::GetValueCoord
///////////////////////////////////////////////////////////////////////

STATIC INLINE float32v GetValueCoord2D( int32v seed, int32v fX, int32v fY )
{
    int32v hash = seed ^ fX ^ fY;
    hash *= hash * int32v( 0x27d4eb2d );
    return FS_Converti32_f32( hash ) * float32v( 1.0f / float(INT_MAX) );
}

STATIC INLINE float32v GetValueCoord3D( int32v seed, int32v fX, int32v fY, int32v fZ )
{
    int32v hash = seed ^ fX ^ fY ^ fZ;
    hash *= hash * int32v( 0x27d4eb2d );
    return FS_Converti32_f32( hash ) * float32v( 1.0f / float(INT_MAX) );
}

STATIC INLINE float32v GetValueCoord4D( int32v seed, int32v fX, int32v fY, int32v fZ, int32v fW )
{
    int32v hash = seed ^ fX ^ fY ^ fZ ^ fW;
    hash *= hash * int32v( 0x27d4eb2d );
    return FS_Converti32_f32( hash ) * float32v( 1.0f / float(INT_MAX) );
}

#define FnUtils_GetValueCoord2(h, x, y) GetValueCoord2D(h, x, y)
#define FnUtils_GetValueCoord3(h, x, y, z) GetValueCoord3D(h, x, y, z)
#define FnUtils_GetValueCoord4(h, x, y, z, w) GetValueCoord4D(h, x, y, z, w)
#define FnUtils_GetValueCoord(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, FnUtils_GetValueCoord4, FnUtils_GetValueCoord3, FnUtils_GetValueCoord2, NF, NF)(__VA_ARGS__)



#endif //FASTNOISEHLSLUTILS_H
