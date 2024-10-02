#ifndef VALUE_H
#define VALUE_H

#include "../Utils.h"

STATIC INLINE float32v _ValueGen2D( int32v seed, float32v x, float32v y )
{
    float32v xs = FS_Floor_f32( x );
    float32v ys = FS_Floor_f32( y );

    int32v x0 = FS_Convertf32_i32( xs ) * int32v( FnPrimes_X );
    int32v y0 = FS_Convertf32_i32( ys ) * int32v( FnPrimes_Y );
    int32v x1 = x0 + int32v( FnPrimes_X );
    int32v y1 = y0 + int32v( FnPrimes_Y );

    xs = FnUtils_InterpHermite( x - xs );
    ys = FnUtils_InterpHermite( y - ys );

    return FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0 ), FnUtils_GetValueCoord( seed, x1, y0 ), xs ),
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1 ), FnUtils_GetValueCoord( seed, x1, y1 ), xs ), ys );
}

STATIC INLINE float32v _ValueGen3D( int32v seed, float32v x, float32v y, float32v z )
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

    return FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z0 ), FnUtils_GetValueCoord( seed, x1, y0, z0 ), xs ),
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z0 ), FnUtils_GetValueCoord( seed, x1, y1, z0 ), xs ), ys ),
        FnUtils_Lerp(                                                                                
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z1 ), FnUtils_GetValueCoord( seed, x1, y0, z1 ), xs ),    
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z1 ), FnUtils_GetValueCoord( seed, x1, y1, z1 ), xs ), ys ), zs );
}

STATIC INLINE float32v _ValueGen4D( int32v seed, float32v x, float32v y, float32v z, float32v w )
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

    return FnUtils_Lerp( FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z0, w0 ), FnUtils_GetValueCoord( seed, x1, y0, z0, w0 ), xs ),
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z0, w0 ), FnUtils_GetValueCoord( seed, x1, y1, z0, w0 ), xs ), ys ),
        FnUtils_Lerp( 
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z1, w0 ), FnUtils_GetValueCoord( seed, x1, y0, z1, w0 ), xs ),    
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z1, w0 ), FnUtils_GetValueCoord( seed, x1, y1, z1, w0 ), xs ), ys ), zs ),
        FnUtils_Lerp( FnUtils_Lerp(
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z0, w1 ), FnUtils_GetValueCoord( seed, x1, y0, z0, w1 ), xs ),
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z0, w1 ), FnUtils_GetValueCoord( seed, x1, y1, z0, w1 ), xs ), ys ),
        FnUtils_Lerp( 
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y0, z1, w1 ), FnUtils_GetValueCoord( seed, x1, y0, z1, w1 ), xs ),    
        FnUtils_Lerp( FnUtils_GetValueCoord( seed, x0, y1, z1, w1 ), FnUtils_GetValueCoord( seed, x1, y1, z1, w1 ), xs ), ys ), zs ), ws );
}

#define ValueNoise(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, _ValueGen4D, _ValueGen3D, _ValueGen2D)(__VA_ARGS__)

#endif