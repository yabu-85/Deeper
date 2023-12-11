#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    int hp_;            //���݂�HP
    int maxHp_;         //�ő�HP
    XMVECTOR movement_; //���̈ړ���

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da);
    XMVECTOR GetMovement() { return movement_; }
};

