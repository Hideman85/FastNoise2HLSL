#ifndef DOMAINWARP_H
#define DOMAINWARP_H

#include "../Utils.h"
#include "../NodeTreeGlobals.h"

STATIC INLINE float32v _Warp2D( int32v seed, float32v warpAmp, float32v x, float32v y, REF(float32v, xOut), REF(float32v, yOut) )
{
    float32v xs = FS_Floor_f32( x );
    float32v ys = FS_Floor_f32( y );

    int32v x0 = FS_Convertf32_i32( xs ) * int32v( FnPrimes_X );
    int32v y0 = FS_Convertf32_i32( ys ) * int32v( FnPrimes_Y );
    int32v x1 = x0 + int32v( FnPrimes_X );
    int32v y1 = y0 + int32v( FnPrimes_Y );

    xs = FnUtils_InterpHermite( x - xs );
    ys = FnUtils_InterpHermite( y - ys );

#define GRADIENT_COORD( _x, _y )\
    int32v hash##_x##_y = FnUtils_HashPrimesHB(seed, x##_x, y##_y );\
    float32v x##_x##_y = FS_Converti32_f32( hash##_x##_y & int32v( 0xffff ) );\
    float32v y##_x##_y = FS_Converti32_f32( (hash##_x##_y >> 16) & int32v( 0xffff ) );

    GRADIENT_COORD( 0, 0 );
    GRADIENT_COORD( 1, 0 );
    GRADIENT_COORD( 0, 1 );
    GRADIENT_COORD( 1, 1 );

#undef GRADIENT_COORD

    float32v normalise = float32v( 1.0f / (0xffff / 2.0f) );

    float32v xWarp = (FnUtils_Lerp( FnUtils_Lerp( x00, x10, xs ), FnUtils_Lerp( x01, x11, xs ), ys ) - float32v( 0xffff / 2.0f )) * normalise;
    float32v yWarp = (FnUtils_Lerp( FnUtils_Lerp( y00, y10, xs ), FnUtils_Lerp( y01, y11, xs ), ys ) - float32v( 0xffff / 2.0f )) * normalise;

    xOut = FS_FMulAdd_f32( xWarp, warpAmp, xOut );
    yOut = FS_FMulAdd_f32( yWarp, warpAmp, yOut );

    float32v warpLengthSq = FS_FMulAdd_f32( xWarp, xWarp, yWarp * yWarp );

    return warpLengthSq * FS_InvSqrt_f32( warpLengthSq );
}
        
STATIC INLINE float32v _Warp3D( int32v seed, float32v warpAmp, float32v x, float32v y, float32v z, REF(float32v, xOut), REF(float32v, yOut), REF(float32v, zOut) )
{
    float32v xs = FS_Floor_f32( x );
    float32v ys = FS_Floor_f32( y );
    float32v zs = FS_Floor_f32( z );

    int32v x0 = FS_Convertf32_i32( xs ) * int32v( FnPrimes_X );
    int32v y0 = FS_Convertf32_i32( ys ) * int32v( FnPrimes_Y );
    int32v z0 = FS_Convertf32_i32( zs ) * int32v( FnPrimes_Z );
    int32v x1 = x0 + int32v( FnPrimes_X );
    int32v y1 = y0 + int32v( FnPrimes_Y );
    int32v z1 = z0 + int32v( FnPrimes_Z );

    xs = FnUtils_InterpHermite( x - xs );
    ys = FnUtils_InterpHermite( y - ys );
    zs = FnUtils_InterpHermite( z - zs );

#define GRADIENT_COORD( _x, _y, _z )\
    int32v hash##_x##_y##_z = FnUtils_HashPrimesHB( seed, x##_x, y##_y, z##_z );\
    float32v x##_x##_y##_z = FS_Converti32_f32( hash##_x##_y##_z & int32v( 0x3ff ) );\
    float32v y##_x##_y##_z = FS_Converti32_f32( (hash##_x##_y##_z >> 10) & int32v( 0x3ff ) );\
    float32v z##_x##_y##_z = FS_Converti32_f32( (hash##_x##_y##_z >> 20) & int32v( 0x3ff ) );

    GRADIENT_COORD( 0, 0, 0 );
    GRADIENT_COORD( 1, 0, 0 );
    GRADIENT_COORD( 0, 1, 0 );
    GRADIENT_COORD( 1, 1, 0 );
    GRADIENT_COORD( 0, 0, 1 );
    GRADIENT_COORD( 1, 0, 1 );
    GRADIENT_COORD( 0, 1, 1 );
    GRADIENT_COORD( 1, 1, 1 );

#undef GRADIENT_COORD

    float32v x0z = FnUtils_Lerp( FnUtils_Lerp( x000, x100, xs ), FnUtils_Lerp( x010, x110, xs ), ys );
    float32v y0z = FnUtils_Lerp( FnUtils_Lerp( y000, y100, xs ), FnUtils_Lerp( y010, y110, xs ), ys );
    float32v z0z = FnUtils_Lerp( FnUtils_Lerp( z000, z100, xs ), FnUtils_Lerp( z010, z110, xs ), ys );
               
    float32v x1z = FnUtils_Lerp( FnUtils_Lerp( x001, x101, xs ), FnUtils_Lerp( x011, x111, xs ), ys );
    float32v y1z = FnUtils_Lerp( FnUtils_Lerp( y001, y101, xs ), FnUtils_Lerp( y011, y111, xs ), ys );
    float32v z1z = FnUtils_Lerp( FnUtils_Lerp( z001, z101, xs ), FnUtils_Lerp( z011, z111, xs ), ys );

    float32v normalise = float32v( 1.0f / (0x3ff / 2.0f) );

    float32v xWarp = (FnUtils_Lerp( x0z, x1z, zs ) - float32v( 0x3ff / 2.0f )) * normalise;
    float32v yWarp = (FnUtils_Lerp( y0z, y1z, zs ) - float32v( 0x3ff / 2.0f )) * normalise;
    float32v zWarp = (FnUtils_Lerp( z0z, z1z, zs ) - float32v( 0x3ff / 2.0f )) * normalise;

    xOut = FS_FMulAdd_f32( xWarp, warpAmp, xOut );
    yOut = FS_FMulAdd_f32( yWarp, warpAmp, yOut );
    zOut = FS_FMulAdd_f32( zWarp, warpAmp, zOut );

    float32v warpLengthSq = FS_FMulAdd_f32( xWarp, xWarp, FS_FMulAdd_f32( yWarp, yWarp, zWarp * zWarp ) );

    return warpLengthSq * FS_InvSqrt_f32( warpLengthSq );
}
        
STATIC INLINE float32v _Warp4D( int32v seed, float32v warpAmp, float32v x, float32v y, float32v z, float32v w, REF(float32v, xOut), REF(float32v, yOut), REF(float32v, zOut), REF(float32v, wOut) )
{
    float32v xs = FS_Floor_f32( x );
    float32v ys = FS_Floor_f32( y );
    float32v zs = FS_Floor_f32( z );
    float32v ws = FS_Floor_f32( w );

    int32v x0 = FS_Convertf32_i32( xs ) * int32v( FnPrimes_X );
    int32v y0 = FS_Convertf32_i32( ys ) * int32v( FnPrimes_Y );
    int32v z0 = FS_Convertf32_i32( zs ) * int32v( FnPrimes_Z );
    int32v w0 = FS_Convertf32_i32( ws ) * int32v( FnPrimes_W );
    int32v x1 = x0 + int32v( FnPrimes_X );
    int32v y1 = y0 + int32v( FnPrimes_Y );
    int32v z1 = z0 + int32v( FnPrimes_Z );
    int32v w1 = w0 + int32v( FnPrimes_W );

    xs = FnUtils_InterpHermite( x - xs );
    ys = FnUtils_InterpHermite( y - ys );
    zs = FnUtils_InterpHermite( z - zs );
    ws = FnUtils_InterpHermite( w - ws );

#define GRADIENT_COORD( _x, _y, _z, _w )\
    int32v hash##_x##_y##_z##_w = FnUtils_HashPrimesHB( seed, x##_x, y##_y, z##_z, w##_w );\
    float32v x##_x##_y##_z##_w = FS_Converti32_f32( hash##_x##_y##_z##_w & int32v( 0xff ) );\
    float32v y##_x##_y##_z##_w = FS_Converti32_f32( (hash##_x##_y##_z##_w >> 8) & int32v( 0xff ) );\
    float32v z##_x##_y##_z##_w = FS_Converti32_f32( (hash##_x##_y##_z##_w >> 16) & int32v( 0xff ) );\
    float32v w##_x##_y##_z##_w = FS_Converti32_f32( (hash##_x##_y##_z##_w >> 24) & int32v( 0xff ) );

    GRADIENT_COORD( 0, 0, 0, 0 );
    GRADIENT_COORD( 1, 0, 0, 0 );
    GRADIENT_COORD( 0, 1, 0, 0 );
    GRADIENT_COORD( 1, 1, 0, 0 );
    GRADIENT_COORD( 0, 0, 1, 0 );
    GRADIENT_COORD( 1, 0, 1, 0 );
    GRADIENT_COORD( 0, 1, 1, 0 );
    GRADIENT_COORD( 1, 1, 1, 0 );
    GRADIENT_COORD( 0, 0, 0, 1 );
    GRADIENT_COORD( 1, 0, 0, 1 );
    GRADIENT_COORD( 0, 1, 0, 1 );
    GRADIENT_COORD( 1, 1, 0, 1 );
    GRADIENT_COORD( 0, 0, 1, 1 );
    GRADIENT_COORD( 1, 0, 1, 1 );
    GRADIENT_COORD( 0, 1, 1, 1 );
    GRADIENT_COORD( 1, 1, 1, 1 );

#undef GRADIENT_COORD

    float32v x0w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( x0000, x1000, xs ), FnUtils_Lerp( x0100, x1100, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( x0010, x1010, xs ), FnUtils_Lerp( x0110, x1110, xs ), ys ), zs );
    float32v y0w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( y0000, y1000, xs ), FnUtils_Lerp( y0100, y1100, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( y0010, y1010, xs ), FnUtils_Lerp( y0110, y1110, xs ), ys ), zs );
    float32v z0w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( z0000, z1000, xs ), FnUtils_Lerp( z0100, z1100, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( z0010, z1010, xs ), FnUtils_Lerp( z0110, z1110, xs ), ys ), zs );
    float32v w0w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( w0000, w1000, xs ), FnUtils_Lerp( w0100, w1100, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( w0010, w1010, xs ), FnUtils_Lerp( w0110, w1110, xs ), ys ), zs );

    float32v x1w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( x0001, x1001, xs ), FnUtils_Lerp( x0101, x1101, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( x0011, x1011, xs ), FnUtils_Lerp( x0111, x1111, xs ), ys ), zs );
    float32v y1w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( y0001, y1001, xs ), FnUtils_Lerp( y0101, y1101, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( y0011, y1011, xs ), FnUtils_Lerp( y0111, y1111, xs ), ys ), zs );
    float32v z1w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( z0001, z1001, xs ), FnUtils_Lerp( z0101, z1101, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( z0011, z1011, xs ), FnUtils_Lerp( z0111, z1111, xs ), ys ), zs );
    float32v w1w = FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp( w0001, w1001, xs ), FnUtils_Lerp( w0101, w1101, xs ), ys ), FnUtils_Lerp( FnUtils_Lerp( w0011, w1011, xs ), FnUtils_Lerp( w0111, w1111, xs ), ys ), zs );                        

    float32v normalise = float32v( 1.0f / (0xff / 2.0f) );

    float32v xWarp = (FnUtils_Lerp( x0w, x1w, ws ) - float32v( 0xff / 2.0f )) * normalise;
    float32v yWarp = (FnUtils_Lerp( y0w, y1w, ws ) - float32v( 0xff / 2.0f )) * normalise;
    float32v zWarp = (FnUtils_Lerp( z0w, z1w, ws ) - float32v( 0xff / 2.0f )) * normalise;
    float32v wWarp = (FnUtils_Lerp( w0w, w1w, ws ) - float32v( 0xff / 2.0f )) * normalise;

    xOut = FS_FMulAdd_f32( xWarp, warpAmp, xOut );
    yOut = FS_FMulAdd_f32( yWarp, warpAmp, yOut );
    zOut = FS_FMulAdd_f32( zWarp, warpAmp, zOut );
    wOut = FS_FMulAdd_f32( wWarp, warpAmp, wOut );

    float32v warpLengthSq = FS_FMulAdd_f32( xWarp, xWarp, FS_FMulAdd_f32( yWarp, yWarp, FS_FMulAdd_f32( zWarp, zWarp, wWarp * wWarp ) ) );

    return warpLengthSq * FS_InvSqrt_f32( warpLengthSq );
}

#define _UseWarp2D _Warp2D(seedInc, amp, currentX * freq, currentY * freq, currentX, currentY)
#define _UseWarp3D _Warp3D(seedInc, amp, currentX * freq, currentY * freq, currentZ * freq, currentX, currentY, currentZ)
#define _UseWarp4D _Warp4D(seedInc, amp, currentX * freq, currentY * freq, currentZ * freq, currentW * freq, currentX, currentY, currentZ, currentW)


/* Define parameters from the stack */
#define warpAmp             PARAM_STACK[param_stack_idx + 1]
#define warpFreq            PARAM_STACK[param_stack_idx + 2]
#define octaves             PARAM_STACK[param_stack_idx + 3]
#define gain                PARAM_STACK[param_stack_idx + 4]
#define weightedStrength    PARAM_STACK[param_stack_idx + 5]
#define lacunarity          PARAM_STACK[param_stack_idx + 6]

#define _MAKE_WARP_PROGRESSIVE(DIM) \
STATIC INLINE bool _WarpFractalProgressive##DIM()\
{\
    int seedInc = currentSeed;\
    float freq = warpFreq;\
    float amp = warpAmp;\
    float strength = _UseWarp##DIM;\
\
    /* If no fractal */\
    if (asint(octaves) < 2) return true;\
    \
    { /* equivalent of mFractalBounding */\
        float gai_n = FS_Abs_f32( gain );\
        float am_p = gai_n;\
        float am_pFractal = 1.0f;\
        for( int i = 1; i < asint(octaves); i++ ) {\
            am_pFractal += am_p;\
            am_p *= gai_n;\
        }\
        amp *= 1.0f / am_pFractal;\
    }\
\
    for (int i = 1; i < asint(octaves); i++)\
    {\
        seedInc += 1;\
        freq *= lacunarity;\
        amp *= FnUtils_Lerp( 1.f, 1.f - strength, weightedStrength );\
        amp *= gain;\
        strength = _UseWarp##DIM;\
    }\
\
    return true;\
}

//

_MAKE_WARP_PROGRESSIVE(2D)
_MAKE_WARP_PROGRESSIVE(3D)
_MAKE_WARP_PROGRESSIVE(4D)


// Cleanup
#undef _MAKE_WARP_PROGRESSIVE
#undef _UseWarp2D
#undef _UseWarp3D
#undef _UseWarp4D
#undef warpAmp
#undef warpFreq
#undef octaves
#undef gain
#undef weightedStrength
#undef lacunarity

#endif //DOMAINWARP_H
