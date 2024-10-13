#include "../Libs/The-Forge/Common_3/Resources/ResourceLoader/Interfaces/IResourceLoader.h"

#include <fstream>
#include <vector>

#include "BiomeCompute.h"

#include <iostream>

#include "FastNoise/FastNoise.h"

#define WITH_SMID

inline void writeBitmap(const char* filename, int width, int height, const std::vector<uint8_t>& pixelData);

constexpr int SIZE = 4096;
constexpr int NB_COMPUTE_THREAD = 16; // Defined in compute shader
constexpr int AREA = SIZE * SIZE;

void BiomeCompute::print() {
    // return;
    LOGF(LogLevel::eINFO, "Compute shader took : %f ms", getHiresTimerUSec(&highresTimer, true) / 1000.0f);
    std::vector<uint8_t> computeData(AREA * 3);
#ifdef WITH_SMID
    std::vector<uint8_t> expectedData(AREA * 3);
#endif

    float* data = (float*)pComputeBuffer->pCpuMappedAddress;

#ifdef WITH_SMID
    std::vector<float> regionData(AREA);
//    auto elevGen = FastNoise::NewFromEncodedNodeTree("BwA="); // Perlin
    auto elevGen = FastNoise::NewFromEncodedNodeTree("CQA="); // OpenSimplex2
    // auto elevGen = FastNoise::NewFromEncodedNodeTree("IQATAAAAgD4RAAUAAAAAAABAEAAAAABABgAAAACAPgAAAAA/AAAAAAARAAUAAAAAAABAEAAAAABADQACAAAApHD9PyEAJAADAAAACQD//wQAAAAAAD8Aj8L1PgAAAAAAAAAAgD4Aj8L1PgAAAAAAAAAAAD8="); // Complex tree

    HiresTimer highresTimer2;
    initHiresTimer(&highresTimer2);

    elevGen->GenUniformGrid2D(regionData.data(), 0, 0, SIZE, SIZE, 0.004f, 1337);

    LOGF(LogLevel::eINFO, "SIMD took : %f ms", getHiresTimerUSec(&highresTimer2, true) / 1000.0f);

    int diff = 0;
#endif

    // #pragma omp parallel for
    for (int z = 0; z < SIZE; z++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = z * SIZE + x;
            int pxOffset = idx * 3;
            computeData[pxOffset] = computeData[pxOffset + 1] = computeData[pxOffset + 2] = 128 + 127 * data[idx];
#ifdef WITH_SMID
            expectedData[pxOffset] = expectedData[pxOffset + 1] = expectedData[pxOffset + 2] = 128 + 127 * regionData[idx];
            if (abs(regionData[idx] - data[idx]) > 0.0000001f) diff++;
#endif
        }
    }

    writeBitmap("test_comp.bmp", SIZE, SIZE, computeData);
#ifdef WITH_SMID
    writeBitmap("test_simd.bmp", SIZE, SIZE, expectedData);
    std::cout << "diff: " << (diff * 100.f) / (SIZE * SIZE) << std::endl;
#endif
}


void BiomeCompute::LoadResources() {
    SyncToken token = {};
    BufferLoadDesc desc = {};
    desc.mDesc.mDescriptors = DESCRIPTOR_TYPE_RW_BUFFER_RAW;
    desc.mDesc.mFlags = BUFFER_CREATION_FLAG_PERSISTENT_MAP_BIT | BUFFER_CREATION_FLAG_HOST_VISIBLE | BUFFER_CREATION_FLAG_HOST_COHERENT;
    desc.mDesc.mMemoryUsage = RESOURCE_MEMORY_USAGE_GPU_TO_CPU;
    desc.mDesc.mStartState = RESOURCE_STATE_SHADER_RESOURCE;
    desc.mDesc.mFormat = TinyImageFormat_R32_SFLOAT;
    desc.mDesc.mSize = AREA * sizeof(float);
    desc.mDesc.mElementCount = AREA;
    desc.mDesc.mStructStride = sizeof(float);
    desc.mDesc.mNodeIndex = pCompute->mUnlinkedRendererIndex;
    desc.ppBuffer = &pComputeBuffer;
    addResource(&desc, &token);
    waitForToken(&token);
}

void BiomeCompute::UnloadResources() {
    removeResource(pComputeBuffer);
}

void BiomeCompute::AddDescriptorSets() {
    DescriptorSetDesc setDesc = { pRootSignature, DESCRIPTOR_UPDATE_FREQ_NONE, 1 };
    setDesc.mNodeIndex = pCompute->mUnlinkedRendererIndex;
    addDescriptorSet(pCompute, &setDesc, &pDescriptorSet);
}

void BiomeCompute::PrepareDescriptorSets() {
    DescriptorData desc[1] = {};
    desc[0].pName = "ElevationBuffer";
    desc[0].ppBuffers = &pComputeBuffer;
    updateDescriptorSet(pCompute, 0, pDescriptorSet, 1, desc);
}

void BiomeCompute::RemoveDescriptorSets() {
    removeDescriptorSet(pCompute, pDescriptorSet);
}

void BiomeCompute::AddShaders() {
    ShaderLoadDesc desc = {};
    desc.mStages[0].pFileName = "noise.comp";
    addShader(pCompute, &desc, &pComputeShader);
}

void BiomeCompute::RemoveShaders() {
    removeShader(pCompute, pComputeShader);
}

void BiomeCompute::AddRootSignatures() {
    RootSignatureDesc desc = { &pComputeShader, 1 };
    addRootSignature(pCompute, &desc, &pRootSignature);
};

void BiomeCompute::RemoveRootSignatures() {
    removeRootSignature(pCompute, pRootSignature);
}

void BiomeCompute::AddPipelines() {
    PipelineDesc pipelineDesc = {};
    pipelineDesc.pName = "ComputePipeline";
    pipelineDesc.mType = PIPELINE_TYPE_COMPUTE;
    ComputePipelineDesc& computePipelineSettings = pipelineDesc.mComputeDesc;
    computePipelineSettings.pShaderProgram = pComputeShader;
    computePipelineSettings.pRootSignature = pRootSignature;
    addPipeline(pCompute, &pipelineDesc, &pPipeline);
};

void BiomeCompute::RemovePipelines() {
    removePipeline(pCompute, pPipeline);
}

void BiomeCompute::UpdateBuffers() {
}

void BiomeCompute::Compute(Cmd* cmd) {
    // Start timer
    initHiresTimer(&highresTimer);

    BufferBarrier barriers[] = {
        {pComputeBuffer, RESOURCE_STATE_UNORDERED_ACCESS, RESOURCE_STATE_UNORDERED_ACCESS }
    };
    cmdResourceBarrier(cmd, 1, barriers, 0, nullptr, 0, nullptr);

    cmdBindPipeline(cmd, pPipeline);
    cmdBindDescriptorSet(cmd, 0, pDescriptorSet);
    cmdDispatch(cmd, (int)(SIZE / NB_COMPUTE_THREAD), (int)(SIZE / NB_COMPUTE_THREAD), 1);

    cmdResourceBarrier(cmd, 1, barriers, 0, nullptr, 0, nullptr);
};


inline void writeBitmap(const char* filename, int width, int height, const std::vector<uint8_t>& pixelData) {
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