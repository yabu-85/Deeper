#pragma once
#include "BaseAction.h"

//‹Šo‚Ì’Tõ
class VisionSearchAction : public BaseAction
{
public:
    VisionSearchAction(Character* obj, float ratio);
    virtual ~VisionSearchAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;
};

//’®Šo‚Ì’Tõ
class AuditorySearchAction : public BaseAction
{
    AuditorySearchAction(Character* obj, float ratio);
    virtual ~AuditorySearchAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;
};

