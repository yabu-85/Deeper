#pragma once
#include "../Character.h"

class Aim;
class StateManager;
class PlayerCommand;
class PlayerWeapon;
class AnimationController;
class LifeManager;

class Player : public Character
{
    enum MAIN_STATE {
        APPER = 0,
        HEAR,
        DEAD,
        UPDATE,
    }state_;
    void ApperUpdate();
    void HearUpdate();
    void DeadUpdate();

    int hModel_;
    int time_;
    float moveSpeed_;           //移動スピード
    float gradually_;           //移動スピードの加減の値
    float rotateRatio_;         //回転の比率
    XMFLOAT3 playerMovement_;   //Normalizeの移動量を保存する用
    XMFLOAT3 apperPos_;

    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerCommand* pCommand_;
    PlayerWeapon* pPlayerWeapon_;
    AnimationController* pAnimationController_;
    LifeManager* pLifeManager_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override;
    void OnAttackCollision(GameObject* pTarget) override;

    //Aim
    void TargetRotate(XMFLOAT3 pos);
    void Rotate();
    void Rotate(float ratio);
    void AimTargetRotate();
    void AimTargetRotate(float ratio);

    //移動
    XMFLOAT3 GetInputMove();        //Inputの移動量取得
    void FrontMove(float f = 1.0f); //前への移動
    void BackMove(float f = 1.0f); //前への移動
    void Move(float f = 1.0f);      //移動
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用
    void ReverseMove(XMFLOAT3 move);

    XMVECTOR GetDirectionVec();     //向いている方向（正規化済み
    void ResetKeyMovement() { movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    void ResetMovement() override;

    //回避
    void InitAvo();

    //ダメージ
    void ReceivedDamage();

    //ゲット系
    int GetModelHandle() { return hModel_; }
    Aim* GetAim() { return pAim_; }
    PlayerCommand* GetCommand() { return pCommand_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    LifeManager* GetLifeManager() { return pLifeManager_; }

};

