#pragma once
#include "Engine/GameObject.h"
#include <vector>

class EnemyUi;
class DropTable;

class EnemyBase : public GameObject
{

protected:
    int hp_;
    int maxHp_;
    EnemyUi* pEnemyUi_;
    DropTable* pDropTable_;

public:
    EnemyBase(GameObject* parent) : GameObject(parent), pEnemyUi_(nullptr), pDropTable_(nullptr), hp_(0), maxHp_(0) {};
    virtual ~EnemyBase() = 0 {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    virtual void ApplyDamage(int da) = 0;

};

