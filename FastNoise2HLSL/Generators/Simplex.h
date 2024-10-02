#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "../Utils.h"

STATIC INLINE float32v _OpenSimplex2Gen2D( int32v seed, float32v x, float32v y )
{
    const float SQRT3 = 1.7320508075f;
    const float F2 = 0.5f * (SQRT3 - 1.0f);
    const float G2 = (3.0f - SQRT3) / 6.0f;

    float32v f = float32v( F2 ) * (x + y);
    float32v x0 = FS_Floor_f32( x + f );
    float32v y0 = FS_Floor_f32( y + f );

    int32v i = FS_Convertf32_i32( x0 ) * int32v( FnPrimes_X );
    int32v j = FS_Convertf32_i32( y0 ) * int32v( FnPrimes_Y );

    float32v g = float32v( G2 ) * (x0 + y0);
    x0 = x - (x0 - g);
    y0 = y - (y0 - g);

    mask32v i1 = x0 > y0;
    //mask32v j1 = ~i1; //NMasked funcs

    float32v x1 = FS_MaskedSub_f32( x0, float32v( 1.f ), i1 ) + float32v( G2 );
    float32v y1 = FS_NMaskedSub_f32( y0, float32v( 1.f ), i1 ) + float32v( G2 );
    float32v x2 = x0 + float32v( (G2 * 2) - 1 );
    float32v y2 = y0 + float32v( (G2 * 2) - 1 );

    float32v t0 = float32v( 0.5f ) - (x0 * x0) - (y0 * y0);
    float32v t1 = float32v( 0.5f ) - (x1 * x1) - (y1 * y1);
    float32v t2 = float32v( 0.5f ) - (x2 * x2) - (y2 * y2);

    t0 = FS_Max_f32( t0, float32v( 0 ) );
    t1 = FS_Max_f32( t1, float32v( 0 ) );
    t2 = FS_Max_f32( t2, float32v( 0 ) );

    t0 *= t0; t0 *= t0;
    t1 *= t1; t1 *= t1;
    t2 *= t2; t2 *= t2;

    float32v n0 = FnUtils_GetGradientDotFancy( FnUtils_HashPrimes( seed, i, j ), x0, y0 );
    float32v n1 = FnUtils_GetGradientDotFancy( FnUtils_HashPrimes( seed, FS_MaskedAdd_i32( i, int32v( FnPrimes_X ), i1 ), FS_NMaskedAdd_i32( j, int32v( FnPrimes_Y ), i1 ) ), x1, y1 );
    float32v n2 = FnUtils_GetGradientDotFancy( FnUtils_HashPrimes( seed, i + int32v( FnPrimes_X ), j + int32v( FnPrimes_Y ) ), x2, y2 );

    return float32v( OPEN_SIMPLEX_2_2D_FACTOR ) * FS_FMulAdd_f32( n0, t0, FS_FMulAdd_f32( n1, t1, n2 * t2 ) );
}

STATIC INLINE float32v _OpenSimplex2Gen3D( int32v seed, float32v x, float32v y, float32v z )
{
    float32v f = float32v( 2.0f / 3.0f ) * (x + y + z);
    float32v xr = f - x;
    float32v yr = f - y;
    float32v zr = f - z;

    float32v val = 0;
    for( int i = 0; ; i++ )
    {
        float32v v0xr = FS_Round_f32( xr );
        float32v v0yr = FS_Round_f32( yr );
        float32v v0zr = FS_Round_f32( zr );
        float32v d0xr = xr - v0xr;
        float32v d0yr = yr - v0yr;
        float32v d0zr = zr - v0zr;

        float32v score0xr = FS_Abs_f32( d0xr );
        float32v score0yr = FS_Abs_f32( d0yr );
        float32v score0zr = FS_Abs_f32( d0zr );
        mask32v dir0xr = FS_Max_f32( score0yr, score0zr ) <= score0xr;
        mask32v dir0yr = FS_BitwiseAndNot_m32( FS_Max_f32( score0zr, score0xr ) <= score0yr, dir0xr );
        mask32v dir0zr = !(dir0xr || dir0yr);
        // NOTE: HLSL cannot bitwise between floats we have to workaround with asuint() asfloat()
        float32v v1xr = FS_MaskedAdd_f32( v0xr, asfloat(asuint( 1.0f ) | ( asuint( -1.0f ) & asuint(d0xr) )), dir0xr );
        float32v v1yr = FS_MaskedAdd_f32( v0yr, asfloat(asuint( 1.0f ) | ( asuint( -1.0f ) & asuint(d0yr) )), dir0yr );
        float32v v1zr = FS_MaskedAdd_f32( v0zr, asfloat(asuint( 1.0f ) | ( asuint( -1.0f ) & asuint(d0zr) )), dir0zr );
        float32v d1xr = xr - v1xr;
        float32v d1yr = yr - v1yr;
        float32v d1zr = zr - v1zr;

        int32v hv0xr = FS_Convertf32_i32( v0xr ) * int32v( FnPrimes_X );
        int32v hv0yr = FS_Convertf32_i32( v0yr ) * int32v( FnPrimes_Y );
        int32v hv0zr = FS_Convertf32_i32( v0zr ) * int32v( FnPrimes_Z );

        int32v hv1xr = FS_Convertf32_i32( v1xr ) * int32v( FnPrimes_X );
        int32v hv1yr = FS_Convertf32_i32( v1yr ) * int32v( FnPrimes_Y );
        int32v hv1zr = FS_Convertf32_i32( v1zr ) * int32v( FnPrimes_Z );

        float32v t0 = FS_FNMulAdd_f32( d0zr, d0zr, FS_FNMulAdd_f32( d0yr, d0yr, FS_FNMulAdd_f32( d0xr, d0xr, float32v( 0.6f ) ) ) );
        float32v t1 = FS_FNMulAdd_f32( d1zr, d1zr, FS_FNMulAdd_f32( d1yr, d1yr, FS_FNMulAdd_f32( d1xr, d1xr, float32v( 0.6f ) ) ) );
        t0 = FS_Max_f32( t0, float32v( 0 ) );
        t1 = FS_Max_f32( t1, float32v( 0 ) );
        t0 *= t0; t0 *= t0;
        t1 *= t1; t1 *= t1;

        float32v v0 = FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, hv0xr, hv0yr, hv0zr ), d0xr, d0yr, d0zr );
        float32v v1 = FnUtils_GetGradientDot( FnUtils_HashPrimes( seed, hv1xr, hv1yr, hv1zr ), d1xr, d1yr, d1zr );

        val = FS_FMulAdd_f32( v0, t0, FS_FMulAdd_f32( v1, t1, val ) );

        if( i == 1 )
        {
            break;
        }

        xr += float32v( 0.5f );
        yr += float32v( 0.5f );
        zr += float32v( 0.5f );
        seed = ~seed;
    }

    return float32v( OPEN_SIMPLEX_2_3D_FACTOR ) * val;
}

// For completeness define 4D version as 0.f => Not Implemented
STATIC INLINE float32v _OpenSimplex2Gen4D( int32v seed, float32v x, float32v y, float32v z, float32v w ) {
    return 0.f;
}

#define OpenSimplex2Noise(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, _OpenSimplex2Gen4D, _OpenSimplex2Gen3D, _OpenSimplex2Gen2D)(__VA_ARGS__)

#endif //SIMPLEX_H
