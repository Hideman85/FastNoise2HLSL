#ifndef FASTNOISECONSTANTS_H
#define FASTNOISECONSTANTS_H

#define ROOT2 1.4142135623730950488f
#define GRADIENT_VEC_2D_X 1 + ROOT2, -1 - ROOT2, 1 + ROOT2, -1 - ROOT2, 1, -1, 1, -1, 1 + ROOT2, -1 - ROOT2, 1 + ROOT2, -1 - ROOT2, 1, -1, 1, -1
#define GRADIENT_VEC_2D_Y 1, 1, -1, -1, 1 + ROOT2, 1 + ROOT2, -1 - ROOT2, -1 - ROOT2, 1, 1, -1, -1, 1 + ROOT2, 1 + ROOT2, -1 - ROOT2, -1 - ROOT2

#define GRADIENT_VEC_3D_X 1, -1, 1, -1, 1, -1, 1, -1, 0, 0, 0, 0, 1, 0, -1, 0
#define GRADIENT_VEC_3D_Y 1, 1, -1, -1, 0, 0, 0, 0, 1, -1, 1, -1, 1, -1, 1, -1
#define GRADIENT_VEC_3D_Z 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, -1, 0, 1, 0, -1

#define GRADIENT_VEC_4D_X1 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 1, -1, 1, -1, 1, -1
#define GRADIENT_VEC_4D_X2 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1
#define GRADIENT_VEC_4D_Y1 1, -1, 1, -1, 1, -1, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0
#define GRADIENT_VEC_4D_Y2 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1
#define GRADIENT_VEC_4D_Z1 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1
#define GRADIENT_VEC_4D_Z2 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, -1, -1, -1, -1
#define GRADIENT_VEC_4D_W1 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1
#define GRADIENT_VEC_4D_W2 1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0

#define ROOT3 1.7320508075688772935f
#define GRADIENT_FANCY_VEC_2D_X ROOT3, ROOT3, 2, 2, 1, -1, 0, 0, -ROOT3, -ROOT3, -2, -2, -1, 1, 0, 0
#define GRADIENT_FANCY_VEC_2D_Y 1, -1, 0, 0, ROOT3, ROOT3, 2, 2, -1, 1, 0, 0, -ROOT3, -ROOT3, -2, -2

#define PERLIN_2D_FACTOR 0.579106986522674560546875f
#define PERLIN_3D_FACTOR 0.964921414852142333984375f
#define PERLIN_4D_FACTOR 0.964921414852142333984375f

#define OPEN_SIMPLEX_2_2D_FACTOR 49.918426513671875f
#define OPEN_SIMPLEX_2_3D_FACTOR 32.69428253173828125f

#endif //FASTNOISECONSTANTS_H
