#ifndef NODETREEGLOBALS_H
#define NODETREEGLOBALS_H

#include "./Definitions.h"

// Use this as a flag for non computed values
#define FLOAT_MIN -3.40282347e+38f
#define TOTAL_PARAMS MAX_STEP_PARAMS * (MAX_NB_STEPS + 1)

// Processing global values
STATIC float PARAM_STACK[TOTAL_PARAMS];
STATIC int param_stack_idx = 0;

struct StepItem {
  uint stepId;
  int nbStackedValues;
  int returnIdx;
};
STATIC StepItem STEP_STACK[(MAX_NB_STEPS + 1)];
STATIC int step_stack_idx = 0;

#define currentStep STEP_STACK[step_stack_idx]
#define returnValue PARAM_STACK[currentStep.returnIdx]

STATIC float currentX, currentY, currentZ, currentW;
STATIC int currentSeed;

STATIC INLINE void stackSeed(int idx) {
  PARAM_STACK[idx] = asfloat(currentSeed);
}
STATIC INLINE void unStackSeed(int idx) {
  currentSeed = asint(PARAM_STACK[idx]);
  PARAM_STACK[idx] = FLOAT_MIN;
}
STATIC INLINE void stackPos(int idx) {
  PARAM_STACK[idx + 0] = currentX;
  PARAM_STACK[idx + 1] = currentY;
  PARAM_STACK[idx + 2] = currentZ;
  PARAM_STACK[idx + 3] = currentW;
}
STATIC INLINE void unStackPos(int idx) {
  currentX = PARAM_STACK[idx + 0];
  currentY = PARAM_STACK[idx + 1];
  currentZ = PARAM_STACK[idx + 2];
  currentW = PARAM_STACK[idx + 3];
  PARAM_STACK[idx + 0] = FLOAT_MIN;
  PARAM_STACK[idx + 1] = FLOAT_MIN;
  PARAM_STACK[idx + 2] = FLOAT_MIN;
  PARAM_STACK[idx + 3] = FLOAT_MIN;
}

#endif //NODETREEGLOBALS_H
