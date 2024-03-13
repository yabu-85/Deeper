#pragma once
#include "../Character/Character.h"
#include "../AnimationController.h"

class SphereCollider;
class Aim;
class StateManager;
class PlayerWeapon;

enum PLAYER_ANIMATION {
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

const AnimeData PLAYER_ANIMATION_DATA[PLAYER_ANIMATION::MAX]{
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
    float moveSpeed_;           //�ړ��X�s�[�h
    float gradually_;           //�ړ��X�s�[�h�̉����̒l
    float rotateRatio_;         //��]�̔䗦
    XMFLOAT3 playerMovement_;   //Normalize�̈ړ��ʂ�ۑ�����p
    XMFLOAT3 apperPos_;

    SphereCollider* pCollider_[2];
    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerWeapon* pPlayerWeapon_;
    AnimationController* pAnimationController_;

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
    void ResetKeyMovement() { movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
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

