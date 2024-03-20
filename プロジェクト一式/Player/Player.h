#pragma once
#include "../Character/Character.h"
#include "../AnimationController.h"

class SphereCollider;
class Aim;
class StateManager;
class PlayerWeapon;

enum class PLAYER_ANIMATION {
    IDLE = 0,
    RUN,
    RORING,
    BACK_STEP,
    DAMAGE_SMALL,
    DAMAGE_MID,
    DEAD,
    ATTACK1,
    ATTACK2,
    ATTACK3,
    STONE_ATTACK,
    MAX,
};

const AnimeData PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::MAX]{
    {0, 120},
    {548, 590},
    {120, 175},
    {500, 546},
    {175, 210},
    {210, 260},
    {260, 495},
    {595, 650},
    {650, 700},
    {666, 700},
    {700, 800},
};

class Player : public Character
{
    int hModel_;
    int time_;
    float moveSpeed_;           //移動スピード
    float gradually_;           //移動スピードの加減の値
    float rotateRatio_;         //回転の比率
    XMFLOAT3 playerMovement_;   //Normalizeの移動量を保存する用
    XMFLOAT3 apperPos_;

    SphereCollider* pCollider_[2];
    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerWeapon* pPlayerWeapon_;
    AnimationController* pAnimationController_;

    void Damage() override;
    void Dead() override;
    void CalcRotate(XMFLOAT3 pos, float ratio);

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //Aim
    void TargetRotate(XMFLOAT3 pos, float ratio = 1.0f);
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
    XMVECTOR GetDirectionVec();     //向いている方向（正規化済み
    void ResetMovement() override;

    //回避
    void Avo();

    int GetModelHandle() { return hModel_; }
    SphereCollider* GetSphereCollider(int i) { return pCollider_[i]; }
    Aim* GetAim() { return pAim_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    StateManager* GetStateManager() { return pStateManager_; }
};

