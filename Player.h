#pragma once
#include "Engine/GameObject.h"

class Aim;
class StateManager;
class PlayerCommand;
class WeaponBase;

class Player :
    public GameObject
{
    int hModel_[2];
    int money_;
    int hp_;
    int maxHp_;
    int currentSubIndex_;       //今選択してるSubのIndex
    float moveSpeed_;           //移動スピード
    float rotateRatio_;         //回転の比率
    XMFLOAT3 playerMovement_;   //今の移動量
    XMFLOAT3 moveVec_;          //移動キーの値を取得
    Transform upTrans_;         //上半身のTransform

    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerCommand* pCommand_;

    WeaponBase* pMainWeapon_;
    WeaponBase* pSubWeapon_[2];

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Rotate();
    void Move(float f = 1.0f);      //実際の移動
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用
    void InitAvo();
    void ResetMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }

    bool IsMoveKeyPushed(XMFLOAT3& key);    //向いている角度を考慮した値
    bool IsMove();                          //移動量が少しでも入ってればtrue

    void AddMoney(int num) { money_ += num; }
    void ApplyDamage(int da);
    void SetUpRotate(XMFLOAT3 rot) { upTrans_.rotate_ = rot; }

    XMVECTOR GetMoveVec() { return XMLoadFloat3(&moveVec_); }  //移動方向（正規化済み
    XMVECTOR GetDirectionVec();                                 //向いている方向（正規化済み
    XMFLOAT3 GetUpRotate() { return upTrans_.rotate_; }
    int GetModelHandle() { return hModel_[0]; }

    PlayerCommand* GetCommand() { return pCommand_; }
    Aim* GetAim() { return pAim_; }
   
    void SetWeapon(WeaponBase* weapon);
    void WeaponChange();    //Inputがあった場合currentSubIndexを変える
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }
};

