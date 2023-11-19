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
    int currentSubIndex_;       //���I�����Ă�Sub��Index
    float moveSpeed_;           //�ړ��X�s�[�h
    float rotateRatio_;         //��]�̔䗦
    XMFLOAT3 playerMovement_;   //���̈ړ���
    XMFLOAT3 fMove_;            //�ړ��L�[�̒l

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

    void Move(float f = 1.0f);      //���ۂ̈ړ�
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p
    void ResetMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    XMFLOAT3 GetMovement() { return playerMovement_; }
    XMVECTOR GetDirectionVec();     //�����Ă�������i���K���ς�

    void InitAvo();
    void AddMoney(int num) { money_ += num; }
    void ApplyDamage(int da);
    int GetModelHandle() { return hModel_[0]; }

    void SetWeapon(WeaponBase* weapon);
    void WeaponChangeIndex();       //Input���������ꍇcurrentSubIndex��ς���
    void SubWeaponRemove();
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }

    PlayerCommand* GetCommand() { return pCommand_; }
    Aim* GetAim() { return pAim_; }

};

