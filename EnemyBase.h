#pragma once
#include "Engine/GameObject.h"

class EnemyUi;

class EnemyBase : public GameObject
{

protected:
    EnemyUi* pEnemyUi_;

public:
    EnemyBase(GameObject* parent) : GameObject(parent), pEnemyUi_(nullptr) {};
    virtual ~EnemyBase() = 0 {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};
};

