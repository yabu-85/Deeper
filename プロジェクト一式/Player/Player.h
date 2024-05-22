#pragma once
#include "../Character/Character.h"

class SphereCollider;
class Aim;
class StateManager;
class PlayerWeapon;
class AnimationController;

enum class PLAYER_ANIMATION {
    IDLE = 0,
    RUN,
    ROLLING,
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

const int PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::MAX][2] {
    {0, 120},     //Idle
    { 548, 590 }, //Run
    { 120, 175 }, //Rolling
    { 500, 546 }, //Back
    { 175, 210 }, //DamageSmall
    { 210, 260 }, //DamageMid
    { 260, 495 }, //Dead
    { 595, 665 }, //Attack1
    { 665, 720 }, //Attack2
    { 675, 720 }, //Attack3
    { 725, 825 }, //Stone
};

class Player : public Character
{
    int hModel_;
    float gradually_;           //�ړ��X�s�[�h�̉����̒l
    XMFLOAT3 playerMovement_;   //Normalize�̈ړ��ʂ�ۑ�����p

    SphereCollider* pCollider_[2];
    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerWeapon* pPlayerWeapon_;
    AnimationController* pAnimationController_;

    void Damage() override;
    void Dead() override;
    void SmallKnockBack() override;
    void MediumKnockBack() override;
    void LargeKnockBack() override;

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

    //�ړ�
    XMFLOAT3 GetInputMove();        //Input�̈ړ��ʎ擾
    void FrontMove(float f = 1.0f); //�O�ւ̈ړ�
    void BackMove(float f = 1.0f); //�O�ւ̈ړ�
    void Move(float f = 1.0f);      //�ړ�
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p
    XMVECTOR GetDirectionVec();     //�����Ă�������i���K���ς�
    void ResetMovement() override;

    //���
    void Avo();

    int GetModelHandle() { return hModel_; }
    SphereCollider* GetSphereCollider(int i) { return pCollider_[i]; }
    Aim* GetAim() { return pAim_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    StateManager* GetStateManager() { return pStateManager_; }
};

