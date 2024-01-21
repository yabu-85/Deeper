#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    float bodyWeight_;  //めり込み時に使う重さ
    float bodyRange_;   //めり込み時に使う範囲

    XMFLOAT3 movement_; //

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    void SetMovement(XMFLOAT3 move) { movement_ = move; }
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }

    //キャラクター同士の衝突の際めり込みを避ける
    void ReflectCharacter(Character* pCharacter);

};

