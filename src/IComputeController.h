#ifndef ICOMPUTECONTROLLER_H
#define ICOMPUTECONTROLLER_H

#include "../Libs/The-Forge/Common_3/Graphics/Interfaces/IGraphics.h"

class IComputeController
{
public:
    explicit IComputeController(Renderer* pCompute) : pCompute(pCompute) {}
    virtual ~IComputeController() = default;

    virtual void LoadResources() = 0;

    void LoadCompute() {
        AddShaders();
        AddRootSignatures();
        AddDescriptorSets();
        AddPipelines();
        PrepareDescriptorSets();
    }
    void UnloadCompute() {
        RemovePipelines();
        RemoveDescriptorSets();
        RemoveRootSignatures();
        RemoveShaders();
        UnloadResources();
    }

    virtual void UpdateBuffers() = 0;
    virtual void Compute(Cmd* cmd) = 0;

protected:
    virtual void UnloadResources() = 0;

    virtual void AddDescriptorSets() = 0;
    virtual void PrepareDescriptorSets() = 0;
    virtual void RemoveDescriptorSets() = 0;

    virtual void AddShaders() = 0;
    virtual void RemoveShaders() = 0;

    virtual void AddRootSignatures() = 0;
    virtual void RemoveRootSignatures() = 0;

    virtual void AddPipelines() = 0;
    virtual void RemovePipelines() = 0;

protected:
    Renderer* pCompute = nullptr;
    Shader* pComputeShader = nullptr;
    DescriptorSet* pDescriptorSet = nullptr;
    RootSignature* pRootSignature = nullptr;
    Pipeline* pPipeline = nullptr;
};

#endif //ICOMPUTECONTROLLER_H
