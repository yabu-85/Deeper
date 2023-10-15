#pragma once
#include "Engine/GameObject.h"

class EnemyBase :
    public GameObject
{

public:
    EnemyBase(GameObject* parent);
    ~EnemyBase();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

