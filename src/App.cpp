#include <fstream>
#include <iostream>
#include <vector>

#include "FastNoise/FastNoise.h"

#include "../Libs/The-Forge/Common_3/Application/Interfaces/IApp.h"
#include "../Libs/The-Forge/Common_3/Resources/ResourceLoader/Interfaces/IResourceLoader.h"
#include "../Libs/The-Forge/Common_3/Utilities/RingBuffer.h"

#include "BiomeCompute.h"

RendererContext* pContext = nullptr;

Renderer *pCompute = nullptr;
Queue *pComputeQueue = nullptr;
GpuCmdRing gComputeCmdRing = {};
Semaphore *prevComputeSemaphore = nullptr;

BiomeCompute* pBiomeCompute = nullptr;

class App : public IApp {
public:
    bool Init() override {
        extern PlatformParameters gPlatformParameters;
        gPlatformParameters.mSelectedRendererApi = RendererApi::RENDERER_API_VULKAN;
        mSettings.mShowPlatformUI = false;

        // FILE PATHS
        fsSetPathForResourceDir(pSystemFileIO, RM_CONTENT, RD_SHADER_BINARIES, "CompiledShaders");
        fsSetPathForResourceDir(pSystemFileIO, RM_DEBUG, RD_DEBUG, "Debug");

        // Window and Renderer setup
        RendererContextDesc contextSettings = {};
        initRendererContext(GetName(), &contextSettings, &pContext);

        RendererDesc settings = {};

        // Need one GPU for rendering and one for compute to simplify
        if (pContext && pContext->mGpuCount >= 2)
        {
            uint32_t queueFamilyCount = 0;
            VkPhysicalDeviceMemoryProperties memProperties;
            auto SHARED_FLAG = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

            int bestGpuIndex = -1;
            int bestProfile = -1;

            struct IntegratedComputeGPU { int idx; uint32_t mem;};
            std::vector<IntegratedComputeGPU> gComputeGPUs = {};

            for (int i = 0; i < pContext->mGpuCount; i++) {
                auto profile = pContext->mGpus[i].mSettings.mGpuVendorPreset.mPresetLevel;
                if (profile > bestProfile) {
                    std::string str = "======> GPU " + std::to_string(i) + " profile " + std::to_string(profile);
                    LOGF(LogLevel::eINFO, str.c_str());
                    bestProfile = profile;
                    bestGpuIndex = i;
                }

                auto device = pContext->mGpus[i].mVk.pGpu;
                auto& props = pContext->mGpus[i].mVk.mGpuProperties.properties;

                if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
                    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                    for (VkQueueFamilyProperties& queueFamily : queueFamilies) {
                        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                            vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

                            for (uint32_t j = 0; j < memProperties.memoryTypeCount; j++) {
                                if (memProperties.memoryTypes[j].propertyFlags & SHARED_FLAG) {
                                    gComputeGPUs.push_back({i, props.limits.maxComputeSharedMemorySize});
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            }

            if (gComputeGPUs.size() > 0) {
                int bestComputeIndex = -1;
                uint32_t bestComputeMem = 0;
                for (auto &gpu : gComputeGPUs) {
                    if (gpu.idx != bestGpuIndex && gpu.mem > bestComputeMem) {
                        bestComputeIndex = gpu.idx;
                        bestComputeMem = gpu.mem;
                    }
                }

                // We have all our needs
                if (bestComputeIndex != -1) {
                    std::string str = "======> Compute GPU ";
                    str.append(pContext->mGpus[bestComputeIndex].mVk.mGpuProperties.properties.deviceName);
                    LOGF(LogLevel::eINFO, str.c_str());
                    str = "======> Graphic GPU ";
                    str.append(pContext->mGpus[bestGpuIndex].mVk.mGpuProperties.properties.deviceName);
                    LOGF(LogLevel::eINFO, str.c_str());

                    settings.pContext = pContext;

                    //  First the render gpu
//                    settings.mGpuMode = GPU_MODE_UNLINKED;
//                    settings.mGpuIndex = bestGpuIndex;
//                    initRenderer(GetName(), &settings, &pRenderer);
//                    if (!pRenderer) return false;

                    //  Second compute one
                    settings.mGpuMode = GPU_MODE_UNLINKED;
                    settings.mGpuIndex = bestComputeIndex;
                    initRenderer(GetName(), &settings, &pCompute);
                    if (!pCompute) return false;
                }
            }
        } else {
            LOGF(LogLevel::eERROR, "No supported integrated GPU");
            return false;
        }

        // if (pCompute) {
        //     Renderer* renderers[2] = {pRenderer, pCompute};
        //     initResourceLoaderInterface(renderers, 2);
        // } else {
        //     initResourceLoaderInterface(pRenderer);
        // }
        initResourceLoaderInterface(pCompute);


        // Compute process
        if (pCompute) {
            QueueDesc queueDesc = {};
            queueDesc.mType = QUEUE_TYPE_COMPUTE;
            // queueDesc.mFlag = QUEUE_FLAG_INIT_MICROPROFILE;
            addQueue(pCompute, &queueDesc, &pComputeQueue);

            GpuCmdRingDesc cmdRingDesc = {};
            cmdRingDesc.pQueue = pComputeQueue;
            cmdRingDesc.mPoolCount = 1; // For now 1 compute only
            cmdRingDesc.mCmdPerPoolCount = 1;
            cmdRingDesc.mAddSyncPrimitives = true;
            addGpuCmdRing(pCompute, &cmdRingDesc, &gComputeCmdRing);
        }



        // Just doing compute for example and then BOOM
        pBiomeCompute = tf_placement_new<BiomeCompute>(tf_calloc(1, sizeof(BiomeCompute)), pCompute);
        pBiomeCompute->LoadResources();

        waitForAllResourceLoads();

        return true;
    }

    void Draw() override {
        pBiomeCompute->LoadCompute();
        pBiomeCompute->UpdateBuffers();
        Compute(pBiomeCompute);
        pBiomeCompute->print();
        // I just needed the compute phase the rest I dont mid so lets quite in a proper way...
        int a = 1337 / 0;
    }

    void Compute(IComputeController *controller) {
        GpuCmdRingElement computeElem = getNextGpuCmdRingElement(&gComputeCmdRing, true, 1);

        // check to see if we can use the cmd buffer
        FenceStatus fenceStatus;
        getFenceStatus(pCompute, computeElem.pFence, &fenceStatus);
        if (fenceStatus == FENCE_STATUS_INCOMPLETE)
            waitForFences(pCompute, 1, &computeElem.pFence);

        Cmd* computeCmd = computeElem.pCmds[0];

        resetCmdPool(pCompute, computeElem.pCmdPool);
        beginCmd(computeCmd);

        // Do something
        controller->Compute(computeCmd);

        endCmd(computeCmd);

        FlushResourceUpdateDesc flushUpdateDesc = {};
        flushUpdateDesc.mNodeIndex = 0;
        flushResourceUpdates(&flushUpdateDesc);
        Semaphore* waitSemaphores[] = { flushUpdateDesc.pOutSubmittedSemaphore, prevComputeSemaphore };

        QueueSubmitDesc submitDesc = {};
        submitDesc.mCmdCount = 1;
        submitDesc.mSignalSemaphoreCount = 1;
        submitDesc.mWaitSemaphoreCount = waitSemaphores[1] ? TF_ARRAY_COUNT(waitSemaphores) : 1;
        submitDesc.ppCmds = &computeCmd;
        submitDesc.ppSignalSemaphores = &computeElem.pSemaphore;
        submitDesc.ppWaitSemaphores = waitSemaphores;
        submitDesc.pSignalFence = computeElem.pFence;
        submitDesc.mSubmitDone = true;
        queueSubmit(pComputeQueue, &submitDesc);

        prevComputeSemaphore = computeElem.pSemaphore;
    }

    const char *GetName() override { return "Compute"; }

    // Dont mind of the rest for now, if you willing to learn The Forge feel free to have a look at it
    bool Load(ReloadDesc *pReloadDesc) override { return true; }
    void Update(float deltaTime) override {}
    void Unload(ReloadDesc *pReloadDesc) override {}
    void Exit() override {}
};

DEFINE_APPLICATION_MAIN(App)