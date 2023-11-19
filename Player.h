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
    XMFLOAT3 fMove_;            //移動キーの値

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
    void Rotate(float ratio);

    void Move(float f = 1.0f);      //実際の移動
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用
    void ResetMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    XMFLOAT3 GetMovement() { return playerMovement_; }
    XMVECTOR GetDirectionVec();     //向いている方向（正規化済み

    void InitAvo();
    void AddMoney(int num) { money_ += num; }
    void ApplyDamage(int da);
    int GetModelHandle() { return hModel_[0]; }

    void SetWeapon(WeaponBase* weapon);
    void WeaponChangeIndex();       //Inputがあった場合currentSubIndexを変える
    void SubWeaponRemove();
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }

    PlayerCommand* GetCommand() { return pCommand_; }
    Aim* GetAim() { return pAim_; }

};

