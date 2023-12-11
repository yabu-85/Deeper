#pragma once
#include "BaseAction.h"

//���o�̒T��
class VisionSearchAction : public BaseAction
{
public:
    VisionSearchAction(Character* obj, float ratio);
    virtual ~VisionSearchAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;
};

//���o�̒T��
class AuditorySearchAction : public BaseAction
{
    AuditorySearchAction(Character* obj, float ratio);
    virtual ~AuditorySearchAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;
};

