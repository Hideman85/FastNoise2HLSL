#include <vector>
#include <fstream>
// #include "omp.h"

#include <iostream>

#include "FastNoise/FastNoise.h" // SIMD
#include "./FastNoise2HLSL/FastNoiseHLSL.h"

inline void writeBitmap(const char* filename, int width, int height, const uint8_t* pixelData) {
    int paddingSize = (4 - (width * 3) % 4) % 4;
    int fileSize = 54 + (3 * width + paddingSize) * height;

    std::vector<uint8_t> fileHeader = {
        'B', 'M',
        (uint8_t)(fileSize), (uint8_t)(fileSize >> 8), (uint8_t)(fileSize >> 16), (uint8_t)(fileSize >> 24),
        0, 0, 0, 0,
        54, 0, 0, 0
    };

    std::vector<uint8_t> infoHeader = {
        40, 0, 0, 0,
        (uint8_t)(width), (uint8_t)(width >> 8), (uint8_t)(width >> 16), (uint8_t)(width >> 24),
        (uint8_t)(height), (uint8_t)(height >> 8), (uint8_t)(height >> 16), (uint8_t)(height >> 24),
        1, 0,
        24, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(fileHeader.data()), fileHeader.size());
    file.write(reinterpret_cast<const char*>(infoHeader.data()), infoHeader.size());

    std::vector<uint8_t> padding(paddingSize, 0);
    for (int y = height - 1; y >= 0; y--) {
        file.write(reinterpret_cast<const char*>(&pixelData[y * width * 3]), width * 3);
        file.write(reinterpret_cast<const char*>(padding.data()), paddingSize);
    }
}

// #define PERLIN
#define SIMPLEX
// #define TREE
// #define VALUE

constexpr uint SIZE = 1024;

int main() {
    GenNoiseSteps();
#ifdef TREE
    auto gen = FastNoise::NewFromEncodedNodeTree("IQATAAAAgD4RAAUAAAAAAABAEAAAAABABgAAAACAPgAAAAA/AAAAAAARAAUAAAAAAABAEAAAAABADQACAAAApHD9PyEAJAADAAAACQD//wQAAAAAAD8Aj8L1PgAAAAAAAAAAgD4Aj8L1PgAAAAAAAAAAAD8="); // Complex tree
#elifdef PERLIN
    auto gen = FastNoise::NewFromEncodedNodeTree("BwA="); // Perlin
#elifdef SIMPLEX
    auto gen = FastNoise::NewFromEncodedNodeTree("CQA="); // OpenSimplex2
#elifdef VALUE
    auto gen = FastNoise::NewFromEncodedNodeTree("BgA="); // Value
#endif

    float freq = 0.004f;
    uint8_t *hlsl = new uint8_t[SIZE * SIZE * 3];
    uint8_t *simd = new uint8_t[SIZE * SIZE * 3];
    int diff = 0;
    int total = SIZE * SIZE;

    // #pragma omp parallel for
    for (int z = 0; z < SIZE; z++) for (int x = 0; x < SIZE; x++) {
        int idx = (z * SIZE + x) * 3;
        float sm = gen->GenSingle2D(x * freq, (SIZE - z) * freq, 1337);
#ifdef TREE
        float hl = NodeTreeProcessing(1337, x * freq, (SIZE - z) * freq);
#elifdef PERLIN
        float hl = PerlinNoise(1337, x * freq, (SIZE - z) * freq);
#elifdef SIMPLEX
        float hl = OpenSimplex2Noise(1337, x * freq, (SIZE - z) * freq);
#elifdef VALUE
        float hl = ValueNoise(1337, x * freq, (SIZE - z) * freq);
#endif
        simd[idx + 0] = simd[idx + 1] = simd[idx + 2] = 127 + 128 * sm;
        hlsl[idx + 0] = hlsl[idx + 1] = hlsl[idx + 2] = 127 + 128 * hl;
        float curDiff = FS_Abs_f32(hl - sm);
        if (curDiff > 0.000001f) {
            diff++;
        }
    }
    float diffPercent = (float)diff * 100.f / total;
    std::cout << "diffPercent = " << diffPercent << std::endl; // Right now OpenSimplex2Noise is different for some reason
    writeBitmap("test_smid.bmp", SIZE, SIZE, simd);
    writeBitmap("test_hlsl.bmp", SIZE, SIZE, hlsl);
    delete[] hlsl; delete[] simd;
    return 0;
}
