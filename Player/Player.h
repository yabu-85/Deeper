#pragma once
#include "../Character.h"

class Aim;
class StateManager;
class PlayerCommand;
class PlayerWeapon;

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
    float moveSpeed_;           //�ړ��X�s�[�h
    float gradually_;           //�ړ��X�s�[�h�̉����̒l
    float rotateRatio_;         //��]�̔䗦
    XMFLOAT3 playerMovement_;   //Normalize�̈ړ��ʂ�ۑ�����p
    XMFLOAT3 apperPos_;

    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerCommand* pCommand_;
    PlayerWeapon* pPlayerWeapon_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override;
    void OnAttackCollision(GameObject* pTarget) override;

    void TargetRotate(XMFLOAT3 pos);
    void Rotate();
    void Rotate(float ratio);
    void AimTargetRotate();
    void AimTargetRotate(float ratio);

    XMFLOAT3 GetInputMove();        //Input�̈ړ��ʎ擾
    void FrontMove(float f = 1.0f); //�O�ւ̈ړ�
    void BackMove(float f = 1.0f); //�O�ւ̈ړ�
    void Move(float f = 1.0f);      //�ړ�
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p

    XMVECTOR GetDirectionVec();     //�����Ă�������i���K���ς�
    void ResetKeyMovement() { movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    void ResetMovement() override;

    void InitAvo();

    int GetModelHandle() { return hModel_; }
    Aim* GetAim() { return pAim_; }
    PlayerCommand* GetCommand() { return pCommand_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }

};

