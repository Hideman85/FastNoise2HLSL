#ifndef FRACTAL_H
#define FRACTAL_H

#include "../NodeTreeGlobals.h"
#include "../Utils.h"

//

STATIC INLINE bool FbmFractal()
{
    // Define parameters from the stack
    #define noise               PARAM_STACK[param_stack_idx + 0]
    #define gain                PARAM_STACK[param_stack_idx + 1]
    #define weightedStrength    PARAM_STACK[param_stack_idx + 2]
    #define octaves             PARAM_STACK[param_stack_idx + 3]
    #define lacunarity          PARAM_STACK[param_stack_idx + 4]

    // Reserve 3 more space in the stack for the sum, the current octave and the amp
    currentStep.nbStackedValues += 3;
    #define sum                 PARAM_STACK[param_stack_idx + 5]
    #define currentOctave       PARAM_STACK[param_stack_idx + 6]
    #define amp                 PARAM_STACK[param_stack_idx + 7]

    // Init the new stack params
    if (sum == FLOAT_MIN) {
        sum = 0.0f;
        currentOctave = 1.0f;

        float gai_n = FS_Abs_f32( gain );
        float am_p = gai_n;
        float am_pFractal = 1.0f;
        for( int i = 1; i < asint(octaves); i++ ) {
            am_pFractal += am_p;
            am_p *= gai_n;
        }
        amp = 1.0f / am_pFractal;

        // Save state
        stackSeed(param_stack_idx + 8);
        stackPos(param_stack_idx + 9);
    }
    currentStep.nbStackedValues += 5;

    // Save previous iteration
    sum += noise * amp;

    if(int(currentOctave) < asint(octaves))
    {
        currentOctave++;
        currentSeed++;
        amp *= FnUtils_Lerp( 1.f, (noise + 1.f) * 0.5f, weightedStrength );
        amp *= gain;

        // Update pos
        currentX *= lacunarity;
        currentY *= lacunarity;
        currentZ *= lacunarity;
        currentW *= lacunarity;

        // Not finished, next step would be to recompute noise from the new pos
        noise = FLOAT_MIN;
        return false;
    }

    // Once done save result in its slot
    returnValue = sum;

    #undef noise
    #undef gain
    #undef weightedStrength
    #undef octaves
    #undef lacunarity
    #undef sum
    #undef currentOctave
    #undef amp

    // Revert back the pos and seed
    unStackSeed(param_stack_idx + 8);
    unStackPos(param_stack_idx + 9);
    return true;
}

//

STATIC INLINE bool RidgedFractal()
{
    // Define parameters from the stack
    #define noise               PARAM_STACK[param_stack_idx + 0]
    #define gain                PARAM_STACK[param_stack_idx + 1]
    #define weightedStrength    PARAM_STACK[param_stack_idx + 2]
    #define octaves             PARAM_STACK[param_stack_idx + 3]
    #define lacunarity          PARAM_STACK[param_stack_idx + 4]

    // Reserve 3 more space in the stack for the sum, the current octave and the amp
    currentStep.nbStackedValues += 3;
    #define sum                 PARAM_STACK[param_stack_idx + 5]
    #define currentOctave       PARAM_STACK[param_stack_idx + 6]
    #define amp                 PARAM_STACK[param_stack_idx + 7]

    // Init the new stack params
    if (sum == FLOAT_MIN) {
        sum = 0.0f;
        currentOctave = 1.0f;

        float gai_n = FS_Abs_f32( gain );
        float am_p = gai_n;
        float am_pFractal = 1.0f;
        for( int i = 1; i < asint(octaves); i++ ) {
            am_pFractal += am_p;
            am_p *= gai_n;
        }
        amp = 1.0f / am_pFractal;

        // Save state
        stackSeed(param_stack_idx + 8);
        stackPos(param_stack_idx + 9);
    }
    currentStep.nbStackedValues += 5;

    // Save sum
    sum += (FS_Abs_f32(noise) * -2.f + 1.f) * amp;

    if(int(currentOctave) < asint(octaves))
    {
        currentOctave++;
        currentSeed++;
        amp *= FnUtils_Lerp( 1.f, 1.f - noise, weightedStrength );
        amp *= gain;

        // Update pos
        currentX *= lacunarity;
        currentY *= lacunarity;
        currentZ *= lacunarity;
        currentW *= lacunarity;

        // Not finished, next step would be to recompute noise from the new pos
        noise = FLOAT_MIN;
        return false;
    }


    // Once done save result in its slot
    returnValue = sum;

    #undef noise
    #undef gain
    #undef weightedStrength
    #undef octaves
    #undef lacunarity
    #undef sum
    #undef currentOctave
    #undef amp

    // Revert back the pos and seed
    unStackSeed(param_stack_idx + 8);
    unStackPos(param_stack_idx + 9);
    return true;
}

//

STATIC INLINE float32v _PingPong( float32v t )
{
    t -= FS_Round_f32( t * float32v( 0.5f ) ) * float32v( 2 );
    return FS_Select_f32( t < float32v( 1 ), t, float32v( 2 ) - t );
}

STATIC INLINE bool PingPongFractal()
{
    // Define parameters from the stack
    #define noise               PARAM_STACK[param_stack_idx + 0]
    #define gain                PARAM_STACK[param_stack_idx + 1]
    #define weightedStrength    PARAM_STACK[param_stack_idx + 2]
    #define pingPongStrength    PARAM_STACK[param_stack_idx + 3]
    #define octaves             PARAM_STACK[param_stack_idx + 4]
    #define lacunarity          PARAM_STACK[param_stack_idx + 5]

    // Reserve 3 more space in the stack for the sum, the current octave and the amp
    currentStep.nbStackedValues += 3;
    #define sum                 PARAM_STACK[param_stack_idx + 6]
    #define currentOctave       PARAM_STACK[param_stack_idx + 7]
    #define amp                 PARAM_STACK[param_stack_idx + 8]

    // Init the new stack params
    if (sum == FLOAT_MIN) {
        sum = 0.0f;
        currentOctave = 1.0f;

        float gai_n = FS_Abs_f32( gain );
        float am_p = gai_n;
        float am_pFractal = 1.0f;
        for( int i = 1; i < asint(octaves); i++ ) {
            am_pFractal += am_p;
            am_p *= gai_n;
        }
        amp = 1.0f / am_pFractal;

        // Save state
        stackSeed(param_stack_idx + 9);
        stackPos(param_stack_idx + 10);
    }
    currentStep.nbStackedValues += 5;

    // Save previous iteration
    sum += (_PingPong(noise) + 1.f) * amp;

    if(int(currentOctave) < asint(octaves))
    {
        currentOctave++;
        currentSeed++;
        amp *= FnUtils_Lerp( 1.f, (noise + 1.f) * 0.5f, weightedStrength );
        amp *= gain;

        // Update pos
        currentX *= lacunarity;
        currentY *= lacunarity;
        currentZ *= lacunarity;
        currentW *= lacunarity;

        // Not finished, next step would be to recompute noise from the new pos
        noise = FLOAT_MIN;
        return false;
    }

    // Once done save result in its slot
    returnValue = sum;

    #undef noise
    #undef gain
    #undef weightedStrength
    #undef octaves
    #undef lacunarity
    #undef sum
    #undef currentOctave
    #undef amp

    // Revert back the pos and seed
    unStackSeed(param_stack_idx + 9);
    unStackPos(param_stack_idx + 10);
    return true;
}

#endif //FRACTAL_H
