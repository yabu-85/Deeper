#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    float bodyWeight_;  //めり込み時に使う重さ
    float bodyRange_;   //めり込み時に使う範囲

    XMFLOAT3 movement_; //移動量を保存するためのやつ

public:
    Character(GameObject* parent, std::string name);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    float GetBodyRange() { return bodyRange_; }

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //プレイヤーだけ追加処理ある
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //・・
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };

    //キャラクター同士の衝突の際めり込みを避ける
    void ReflectCharacter();

};

