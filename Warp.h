#pragma once
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"

class Warp : public GameObject
{
    bool isPlayerHit_;
    int hModel_;

    SCENE_ID warpScene_;

public:
    Warp(GameObject* parent);
    ~Warp();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override;
    void OutCollision() override;
    void SetWarpScene(SCENE_ID s) { warpScene_ = s; }

};

