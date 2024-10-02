#ifndef NODETREEDEFINITION_H
#define NODETREEDEFINITION_H

// Defining the step logic provided by the Encoded Node Tree

// Fractals count the max params with FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE up to 7 but for std140 use 8
#define MAX_STEP_PARAMS 8
#define MAX_NB_STEPS 20

#ifdef __cplusplus
#define _ENUM(NAME, ...) enum class NAME : uint { __VA_ARGS__ };
#define _ENUM_VAL(NAME, VAL) NAME = VAL,
#define _GET_ENUM_VAL(TYPE, NAME) static_cast<uint>(TYPE::NAME)
#define Get(X) nodeTreeUniform.X
#define asFloatCpp asfloat
#else
#define _ENUM(NAME, ...) __VA_ARGS__
#define _ENUM_VAL(NAME, VAL) STATIC const uint NAME = VAL;
#define _GET_ENUM_VAL(TYPE, NAME) NAME
#define asFloatCpp
#endif

_ENUM(
  ParamType,
  _ENUM_VAL(PARAM_UNKNOWN, 0)
  _ENUM_VAL(PARAM_STEP_RESULT, 1)
  _ENUM_VAL(PARAM_CONSTANT, 2)
)

_ENUM(
  StepType,
  _ENUM_VAL(UNKNOWN, 0)
  /* Coherent noises */
  _ENUM_VAL(NOISE_VALUE, 1)
  _ENUM_VAL(NOISE_PERLIN, 2)
  _ENUM_VAL(NOISE_OPENSIMPLEX2, 3)
  /* Blends */
  _ENUM_VAL(BLEND_ADD, 10)
  _ENUM_VAL(BLEND_SUB, 11)
  _ENUM_VAL(BLEND_FADE, 12)
  _ENUM_VAL(BLEND_MULTIPLY, 13)
  _ENUM_VAL(BLEND_DIVIDE, 14)
  _ENUM_VAL(BLEND_POW_FLOAT, 15)
  _ENUM_VAL(BLEND_POW_INT, 16)
  _ENUM_VAL(BLEND_MIN, 17)
  _ENUM_VAL(BLEND_MAX, 18)
  /* Fractals */
  _ENUM_VAL(FRACTAL_FBM, 30)
  _ENUM_VAL(FRACTAL_PING_PONG, 31)
  _ENUM_VAL(FRACTAL_RIGGED, 32)
  /* Position Modifiers */
  /* Assuming simple WarpGradient as 1 octave */
  _ENUM_VAL(FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE, 50)
  _ENUM_VAL(FRACTAL_DOMAIN_WARP_FRACTAL_INDEPENDANT, 51)
  _ENUM_VAL(MODIFIER_DOMAIN_SCALE, 52)
)

#ifdef __cplusplus
union ParamValue {
  uint stepId;
  float floatValue;
  int intValue;
  uint uintValue;
};
typedef struct alignas(16) StepParameter {
  ParamType Type;
  ParamValue Value;
  float PADDING[2];
} StepParameter;
typedef struct alignas(16) Step {
  StepType Type;
  uint PADDING[3];
  StepParameter Parameters[MAX_STEP_PARAMS];
} Step;
typedef struct alignas(16) NodeTreeUniform {
  uint NbSteps;
  uint BufferIdx;
  float Frequency = 0.0004f;
  int Seed = 1337;
  Step Steps[MAX_NB_STEPS];
} NodeTreeUniform;
#else
STRUCT(StepParameter) {
  DATA(uint, Type, None);
  DATA(float, Value, None);
};
STRUCT(Step) {
  DATA(uint, Type, None);
  DATA(StepParameter, Parameters[MAX_STEP_PARAMS], None);
};
// #define __USE_UBO__
#ifdef __USE_UBO__
CBUFFER(NodeTreeUniform, UPDATE_FREQ_NONE, b3, binding=3)
{
  DATA(uint, NbSteps, None);
  DATA(uint, BufferIdx, None);
  DATA(float, Frequency, None);
  DATA(int, Seed, None);
  DATA(Step, Steps[MAX_NB_STEPS], None);
};
#else
STRUCT(NodeTreeUniform)
{
  DATA(uint, NbSteps, None);
  DATA(uint, BufferIdx, None);
  DATA(float, Frequency, None);
  DATA(int, Seed, None);
  DATA(Step, Steps[MAX_NB_STEPS], None);
};

NodeTreeUniform tree;

#define Get(X) tree.X

void GenNoiseSteps() {
    int step = 0;
    tree.Steps[step].Type = NOISE_OPENSIMPLEX2;

    step++;
    tree.Steps[step].Type = BLEND_POW_INT;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[1].Value = asfloat(3);

    step++;
    tree.Steps[step].Type = BLEND_FADE;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 2);
    tree.Steps[step].Parameters[1].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[1].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[2].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[2].Value = 0.5f;

    step++;
    tree.Steps[step].Type = FRACTAL_FBM;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[1].Value = 0.48f;
    tree.Steps[step].Parameters[2].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[2].Value = 0.f;
    tree.Steps[step].Parameters[3].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[3].Value = asfloat(2);
    tree.Steps[step].Parameters[4].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[4].Value = 1.98f;

    step++;
    tree.Steps[step].Type = FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[1].Value = 0.25f;
    tree.Steps[step].Parameters[2].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[2].Value = 2.f;
    tree.Steps[step].Parameters[3].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[3].Value = asfloat(5);
    tree.Steps[step].Parameters[4].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[4].Value = 0.5f;
    tree.Steps[step].Parameters[5].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[5].Value = 0.f;
    tree.Steps[step].Parameters[6].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[6].Value = 2.f;
    uint phase1 = step;

    step++;
    tree.Steps[step].Type = NOISE_VALUE;

    step++;
    tree.Steps[step].Type = FRACTAL_DOMAIN_WARP_FRACTAL_PROGRESSIVE;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[1].Value = 0.25f;
    tree.Steps[step].Parameters[2].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[2].Value = 2.f;
    tree.Steps[step].Parameters[3].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[3].Value = asfloat(5);
    tree.Steps[step].Parameters[4].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[4].Value = 0.5f;
    tree.Steps[step].Parameters[5].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[5].Value = 0.f;
    tree.Steps[step].Parameters[6].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[6].Value = 2.f;

    step++;
    tree.Steps[step].Type = MODIFIER_DOMAIN_SCALE;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[1].Value = 0.25f;

    step++;
    tree.Steps[step].Type = BLEND_FADE;
    tree.Steps[step].Parameters[0].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[0].Value = asfloat(step - 1);
    tree.Steps[step].Parameters[1].Type = PARAM_STEP_RESULT;
    tree.Steps[step].Parameters[1].Value = asfloat(phase1);
    tree.Steps[step].Parameters[2].Type = PARAM_CONSTANT;
    tree.Steps[step].Parameters[2].Value = 0.5f;

    tree.NbSteps = step + 1;
    tree.Frequency = 0.0004f;
    tree.Seed = 1337;
}
#endif
#endif

// Define this global for c++
#ifdef __cplusplus
static NodeTreeUniform nodeTreeUniform;
#endif

#endif //NODETREEDEFINITION_H
