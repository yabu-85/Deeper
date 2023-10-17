#pragma once
#include "Engine/GameObject.h"

class EnemyBase : public GameObject
{
    
protected:

public:
    EnemyBase(GameObject* parent);
    virtual ~EnemyBase() = 0;
    virtual void Initialize() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() override = 0;
    virtual void Release() override = 0;

};

