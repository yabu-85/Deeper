#pragma once
#include "../Engine/GameObject.h"

class Character : public GameObject
{
    int knockBackTime_;             //ノックバックの時間
    int knockBackTimeMax_;          //ノックバックの時間保存用
    float knockBackPower_;          //ノックバックの強さ
    XMFLOAT3 knockBackDirection_;   //ノックバックさせる方向

    virtual void SmallKnockBack() {};
    virtual void MediumKnockBack() {};
    virtual void LargetKnockBack() {};

protected:
    float bodyWeight_;  //めり込み時に使う重さ
    float bodyRange_;   //めり込み時に使う範囲
    XMFLOAT3 movement_; //移動量を保存するためのやつ

public:
    Character(GameObject* parent, std::string name);
    virtual ~Character();
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0;
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    //キャラクター同士の衝突の際めり込みを避ける
    void ReflectCharacter();

    //----------------------------------------------------------------

    enum KNOCK_TYPE {
        SMALL = 0,
        MEDIUM,
        LARGE,
        MAX,
    };

    //ノックバックさせる、KnockBackPowerと引数で掛けた値でノックバックさせる
    void KnockBack(float speed);

    //ノックバックの情報をセットする
    void SetKnockBack(KNOCK_TYPE type, int time, float power, XMFLOAT3 pos);

    //ゲット関数
    int GetKnockBackTime() { return knockBackTime_; }
    int GetKnockBackTimeMax() { return knockBackTimeMax_; }
    
    //----------------------------------------------------------------

    float GetBodyRange() { return bodyRange_; }
    float GetBodyWeight() { return bodyWeight_; }

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //プレイヤーだけ追加処理ある
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //・・
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };

};

