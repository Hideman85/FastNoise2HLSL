### Reworking an implementation of FastNoise2 in HLSL

The main files are in the folder `FastNoise2HLSL` and can be included using `#extension GL_GOOGLE_include_directive : require`

The rest of the setup is for testing the compute shader over the SIMD implementation, compare the outputs for coherence and the speed of the two methods. For that purpose I made a quick setup of [The Forge](https://github.com/ConfettiFX/The-Forge) rendering engine as I'm using it in my project. Feel free to have a look to that engine if you like.

In The Forge, the final compute shader is in `Shaders/noise.comp.fsl` and the binding is made on a iGPU in the `src/App.cpp` file.

### Notes

> HLSL does not support function overloading

I am using variadic macro as workaround, definition [here](https://github.com/Hideman85/FastNoise2HLSL/blob/master/FastNoise2HLSL/Definitions.h#L9-L21) and usage example [here](https://github.com/Hideman85/FastNoise2HLSL/blob/master/FastNoise2HLSL/Generators/Perlin.h#L105-L108)
