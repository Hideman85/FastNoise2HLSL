#ifndef PERLIN_H
#define PERLIN_H

#include "../Utils.h"

STATIC INLINE float32v _PerlinGen2D( int32v seed, float32v x, float32v y )
{
    float32v xs = FS_Floor_f32( x );
    float32v ys = FS_Floor_f32( y );

    int32v x0 = FS_Convertf32_i32( xs ) * int32v( FnPrimes_X );
    int32v y0 = FS_Convertf32_i32( ys ) * int32v( FnPrimes_Y );
    int32v x1 = x0 + int32v( FnPrimes_X );
    int32v y1 = y0 + int32v( FnPrimes_Y );

    float32v xf0 = xs = x - xs;
    float32v yf0 = ys = y - ys;
    float32v xf1 = xf0 - float32v( 1 );
    float32v yf1 = yf0 - float32v( 1 );

    xs = FnUtils_InterpQuintic( xs );
    ys = FnUtils_InterpQuintic( ys );

    return float32v( PERLIN_2D_FACTOR ) * FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0 ), xf0, yf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0 ), xf1, yf0 ), xs ),
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1 ), xf0, yf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1 ), xf1, yf1 ), xs ), ys );
}

STATIC INLINE float32v _PerlinGen3D( int32v seed, float32v x, float32v y, float32v z )
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

    float32v xf0 = xs = x - xs;
    float32v yf0 = ys = y - ys;
    float32v zf0 = zs = z - zs;
    float32v xf1 = xf0 - float32v( 1 );
    float32v yf1 = yf0 - float32v( 1 );
    float32v zf1 = zf0 - float32v( 1 );

    xs = FnUtils_InterpQuintic( xs );
    ys = FnUtils_InterpQuintic( ys );
    zs = FnUtils_InterpQuintic( zs );

    return float32v( PERLIN_3D_FACTOR ) * FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z0 ), xf0, yf0, zf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z0 ), xf1, yf0, zf0 ), xs ),
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z0 ), xf0, yf1, zf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z0 ), xf1, yf1, zf0 ), xs ), ys ),
        FnUtils_Lerp( 
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z1 ), xf0, yf0, zf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z1 ), xf1, yf0, zf1 ), xs ),    
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z1 ), xf0, yf1, zf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z1 ), xf1, yf1, zf1 ), xs ), ys ), zs );
}

STATIC INLINE float32v _PerlinGen4D( int32v seed, float32v x, float32v y, float32v z, float32v w )
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

    float32v xf0 = xs = x - xs;
    float32v yf0 = ys = y - ys;
    float32v zf0 = zs = z - zs;
    float32v wf0 = ws = w - ws;
    float32v xf1 = xf0 - float32v( 1 );
    float32v yf1 = yf0 - float32v( 1 );
    float32v zf1 = zf0 - float32v( 1 );
    float32v wf1 = wf0 - float32v( 1 );

    xs = FnUtils_InterpQuintic( xs );
    ys = FnUtils_InterpQuintic( ys );
    zs = FnUtils_InterpQuintic( zs );
    ws = FnUtils_InterpQuintic( ws );

    return float32v( PERLIN_4D_FACTOR ) * FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z0, w0 ), xf0, yf0, zf0, wf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z0, w0 ), xf1, yf0, zf0, wf0 ), xs ),
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z0, w0 ), xf0, yf1, zf0, wf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z0, w0 ), xf1, yf1, zf0, wf0 ), xs ), ys ),
        FnUtils_Lerp(                                                                                                                                                     
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z1, w0 ), xf0, yf0, zf1, wf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z1, w0 ), xf1, yf0, zf1, wf0 ), xs ),    
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z1, w0 ), xf0, yf1, zf1, wf0 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z1, w0 ), xf1, yf1, zf1, wf0 ), xs ), ys ), zs ),
        FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z0, w1 ), xf0, yf0, zf0, wf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z0, w1 ), xf1, yf0, zf0, wf1 ), xs ),
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z0, w1 ), xf0, yf1, zf0, wf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z0, w1 ), xf1, yf1, zf0, wf1 ), xs ), ys ),
        FnUtils_Lerp(                                                                                                                                                     
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y0, z1, w1 ), xf0, yf0, zf1, wf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y0, z1, w1 ), xf1, yf0, zf1, wf1 ), xs ),    
        FnUtils_Lerp( FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x0, y1, z1, w1 ), xf0, yf1, zf1, wf1 ), FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, x1, y1, z1, w1 ), xf1, yf1, zf1, wf1 ), xs ), ys ), zs ), ws );
}

#define PerlinNoise(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, _PerlinGen4D, _PerlinGen3D, _PerlinGen2D)(__VA_ARGS__)

#endif //PERLIN_H
