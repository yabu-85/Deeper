#pragma once
#include "../Engine/GameObject.h"

class RecoveryItem : public GameObject
{
    int hModel_;
    int recoveryAmount_;
    float gravity_;

public:
    RecoveryItem(GameObject* parent);
    ~RecoveryItem();
    void Initialize();
    void Update();
    void Draw();
    void Release();
    void OnCollision(GameObject* pTarget) override;

};

