#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    int hp_;
    int maxHp_;

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da);
};

