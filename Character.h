#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    float bodyWeight_;  //めり込み時に使う重さ

    XMVECTOR movement_; //今の移動量

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    XMVECTOR GetMovement() { return movement_; }

    //キャラクター同士の衝突の際めり込みを避ける
    void ReflectCharacter(Character* pCharacter);

};

