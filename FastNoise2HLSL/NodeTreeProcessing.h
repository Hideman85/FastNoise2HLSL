#ifndef NODETREEPROCESSING_H
#define NODETREEPROCESSING_H

#include "./NodeTreeGlobals.h"

#include "./PositionTransformers/DomainWarp.h"
#include "./StepTransformers/Fractal.h"
#include "./Generators/Perlin.h"
#include "./Generators/Simplex.h"
#include "./Generators/Value.h"

#define _F2D float x, float y
#define _F3D _F2D, float z
#define _F4D _F3D, float w

#define _PF2D currentX, currentY
#define _PF3D _PF2D, currentZ
#define _PF4D _PF3D, currentW

#define _SET2D currentSeed = seed; currentX = x; currentY = y;
#define _SET3D _SET2D currentZ = z;
#define _SET4D _SET3D currentW = w;

#define _A PARAM_STACK[param_stack_idx + 0]
#define _B PARAM_STACK[param_stack_idx + 1]
#define _C PARAM_STACK[param_stack_idx + 2]


#define _MAKE_POS_HELPER(DIM) \
STATIC INLINE bool _applyPositionModifier##DIM(uint TYPE) {\
    switch (TYPE) {\
        case _GET_ENUM_VAL(StepType, FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE):\
            return _WarpFractalProgressive##DIM();\
        case _GET_ENUM_VAL(StepType, MODIFIER_DOMAIN_SCALE): {\
            currentX *= _B; currentY *= _B; currentZ *= _B; currentW *= _B;\
            return true;\
        }\
    }\
    return true;\
}

#define _MAKE_BLEND_HELPER(DIM) \
STATIC INLINE bool _applyBlend##DIM(uint TYPE) {\
    switch (TYPE) {\
        case _GET_ENUM_VAL(StepType, BLEND_ADD): {\
            returnValue = _A + _B;\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_SUB): {\
            returnValue = _A - _B;\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_FADE): {\
            returnValue = _A * _C + _B * (1.0f - _C);\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_MULTIPLY): {\
            returnValue = _A * _B;\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_DIVIDE): {\
            returnValue = _A / _B;\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_POW_FLOAT): {\
            returnValue = pow(_A, _B);\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_POW_INT): {\
            returnValue = pow(_A, asint(_B));\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_MIN): {\
            returnValue = _A < _B ? _A : _B;\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, BLEND_MAX): {\
            returnValue = _A > _B ? _A : _B;\
            return true;\
        }\
    }\
    returnValue = 0.f;\
    return true;\
}

#define _MAKE_FRACTAL_HELPER(DIM) \
STATIC INLINE bool _applyFractal##DIM(uint TYPE) {\
    switch (TYPE) {\
        case _GET_ENUM_VAL(StepType, FRACTAL_FBM): return FbmFractal();\
        case _GET_ENUM_VAL(StepType, FRACTAL_PING_PONG): return PingPongFractal();\
        case _GET_ENUM_VAL(StepType, FRACTAL_RIGGED): return RidgedFractal();\
    }\
    returnValue = 0.f;\
    return true;\
}

#define _MAKE_NOISE_HELPER(DIM) \
STATIC INLINE bool _applyNoise##DIM(uint TYPE) {\
    switch (TYPE) {\
        case _GET_ENUM_VAL(StepType, NOISE_VALUE): {\
            returnValue = ValueNoise(currentSeed, _PF##DIM);\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, NOISE_PERLIN): {\
            returnValue = PerlinNoise(currentSeed, _PF##DIM);\
            return true;\
        }\
        case _GET_ENUM_VAL(StepType, NOISE_OPENSIMPLEX2): {\
            returnValue = OpenSimplex2Noise(currentSeed, _PF##DIM);\
            return true;\
        }\
    }\
    returnValue = 0.f;\
    return true;\
}

#define _MAKE_STEP_HELPER(DIM) \
STATIC INLINE bool _getStepResult##DIM(uint stepId) {\
    uint stepType = asuint(STEPS_POINTER[stepId].Type);\
    if (stepType >= _GET_ENUM_VAL(StepType, FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE)) return true; /* Already treated */\
    if (stepType >= _GET_ENUM_VAL(StepType, FRACTAL_FBM)) return _applyFractal##DIM(stepType);\
    if (stepType >= _GET_ENUM_VAL(StepType, BLEND_ADD)) return _applyBlend##DIM(stepType);\
    if (stepType >= _GET_ENUM_VAL(StepType, NOISE_VALUE)) return _applyNoise##DIM(stepType);\
    return true;\
}

#define _MAKE_PROCESSING(DIM) \
STATIC float _treeProcessing##DIM(int seed, _F##DIM) {\
    _SET##DIM \
\
    /* Clean state */\
    for (uint i = 0; i < TOTAL_PARAMS; i++) PARAM_STACK[i] = FLOAT_MIN;\
    step_stack_idx = 0;\
    param_stack_idx = 0;\
\
    currentStep.stepId = NB_STEP_POINTER - 1;\
    currentStep.returnIdx = 0;\
    param_stack_idx++;\
\
    uint stepId, nbParams;\
    /* When we finish a function, we come back to a function that is already stacked, then we flag to decount nbParams */\
    bool hasBeenStacked = false;\
\
    while (true) {\
        stepId = currentStep.stepId;\
\
        /* Phase 0: When we finished a function we have to decount params */ \
        if (hasBeenStacked) {\
            hasBeenStacked = false;\
            param_stack_idx -= currentStep.nbStackedValues;\
        }\
\
        /* Phase 1: Determine nb params, fills STATIC params, and get to know dynamic ones */\
        int unCompletedParam = -1;\
        for (\
            nbParams = 0;\
            nbParams < MAX_STEP_PARAMS && asuint(STEPS_POINTER[stepId].Parameters[nbParams].Type) > _GET_ENUM_VAL(ParamType, PARAM_UNKNOWN);\
            ++nbParams\
        ) {\
            if (asuint(STEPS_POINTER[stepId].Parameters[nbParams].Type) == _GET_ENUM_VAL(ParamType, PARAM_STEP_RESULT)) {\
                if (unCompletedParam <= 0 && PARAM_STACK[param_stack_idx + nbParams] == FLOAT_MIN) {\
                    unCompletedParam = int(nbParams);\
                }\
            } else {\
                PARAM_STACK[param_stack_idx + nbParams] = asFloatCpp(STEPS_POINTER[stepId].Parameters[nbParams].Value);\
            }\
        }\
\
        if (unCompletedParam == -1) { /* Phase 2: If all params completed, unstack and compute */\
            uint stepType = asuint(STEPS_POINTER[stepId].Type);\
            bool isPosModifier = stepType >= _GET_ENUM_VAL(StepType, FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE);\
            bool done = isPosModifier /* already treated */ || _getStepResult2D(stepId);\
            if (isPosModifier) { /* Dont forget to unstack */\
                unStackPos(param_stack_idx + nbParams);\
                returnValue = PARAM_STACK[param_stack_idx];\
            }\
\
            /* If this is done unstack */\
            if (done) {\
                for (uint i = 0; i < currentStep.nbStackedValues; i++) PARAM_STACK[param_stack_idx + i] = FLOAT_MIN;\
\
                /* Reset data */\
                currentStep.stepId = currentStep.nbStackedValues = currentStep.returnIdx = 0;\
\
                step_stack_idx--;\
                /* Our break condition */\
                if (step_stack_idx == uint(-1)) break;\
                hasBeenStacked = true;\
            }\
        } else { /* Phase 3: If still need some dynamic params, look for it, stack current state and "recurse" */\
            if (\
                asuint(STEPS_POINTER[stepId].Type) >= _GET_ENUM_VAL(StepType, FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE)\
                && unCompletedParam == 0\
            ) { /* Phase 3.1: If this is a position transform and the last param to compute is source, then transform */\
                stackPos(param_stack_idx + nbParams);\
                nbParams += 4;\
                _applyPositionModifier##DIM(asuint(STEPS_POINTER[stepId].Type));\
            }\
            /* Stack current step */\
            if (currentStep.nbStackedValues == 0) currentStep.nbStackedValues = nbParams;\
            uint nbValues = currentStep.nbStackedValues;\
            /* Next step */\
            step_stack_idx++;\
            currentStep.stepId = asuint(STEPS_POINTER[stepId].Parameters[unCompletedParam].Value);\
            currentStep.returnIdx = param_stack_idx + uint(unCompletedParam);\
            param_stack_idx += nbValues;\
        }\
    }\
\
    /* Safe return defaulting to 0.f when something seems wrong */\
    return PARAM_STACK[0] == FLOAT_MIN ? 0.f : PARAM_STACK[0];\
}

_MAKE_POS_HELPER(2D)
_MAKE_BLEND_HELPER(2D)
_MAKE_FRACTAL_HELPER(2D)
_MAKE_NOISE_HELPER(2D)
_MAKE_STEP_HELPER(2D)
_MAKE_PROCESSING(2D)

//_MAKE_POS_HELPER(3D)
//_MAKE_BLEND_HELPER(3D)
//_MAKE_FRACTAL_HELPER(3D)
//_MAKE_NOISE_HELPER(3D)
//_MAKE_STEP_HELPER(3D)
//_MAKE_PROCESSING(3D)
//
//_MAKE_POS_HELPER(4D)
//_MAKE_BLEND_HELPER(4D)
//_MAKE_FRACTAL_HELPER(4D)
//_MAKE_NOISE_HELPER(4D)
//_MAKE_STEP_HELPER(4D)
//_MAKE_PROCESSING(4D)

#define NodeTreeProcessing(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, NF, NF, _treeProcessing2D)(__VA_ARGS__)
// #define NodeTreeProcessing(...) _GET_OVERRIDE(__VA_ARGS__, NF, NF, NF, NF, _treeProcessing2D)(__VA_ARGS__)

// Cleanup
#undef _MAKE_POS_HELPER
#undef _MAKE_BLEND_HELPER
#undef _MAKE_FRACTAL_HELPER
#undef _MAKE_NOISE_HELPER
#undef _MAKE_STEP_HELPER
#undef _MAKE_PROCESSING
#undef _F2D
#undef _F3D
#undef _F4D
#undef _PF2D
#undef _PF3D
#undef _PF4D
#undef _A
#undef _B
#undef _C
#undef returnValue
#undef currentStack
#undef FLOAT_MIN

#endif //NODETREEPROCESSING_H
