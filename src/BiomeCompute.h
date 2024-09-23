#ifndef BIOMECOMPUTE_H
#define BIOMECOMPUTE_H

#include "../Libs/The-Forge/Common_3/Utilities/Interfaces/ITime.h"

#include "IComputeController.h"

class BiomeCompute : public IComputeController {
public:
    explicit BiomeCompute(Renderer* pCompute) : IComputeController(pCompute) {};

    void LoadResources() override;
    void UnloadResources() override;

    void AddDescriptorSets() override;
    void PrepareDescriptorSets() override;
    void RemoveDescriptorSets() override;

    void AddShaders() override;
    void RemoveShaders() override;

    void AddRootSignatures() override;
    void RemoveRootSignatures() override;

    void AddPipelines() override;
    void RemovePipelines() override;

    void UpdateBuffers() override;
    void Compute(Cmd* cmd) override;

    void print();

private:
    HiresTimer highresTimer;
    Buffer* pComputeBuffer = nullptr;
};

#endif //BIOMECOMPUTE_H
