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
    XMFLOAT3 moveVec_;          //�ړ��L�[�̒l���擾
    Transform upTrans_;         //�㔼�g��Transform

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
    void Move(float f = 1.0f);      //���ۂ̈ړ�
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p
    void InitAvo();
    void ResetMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }

    bool IsMoveKeyPushed(XMFLOAT3& key);    //�����Ă���p�x���l�������l
    bool IsMove();                          //�ړ��ʂ������ł������Ă��true

    void AddMoney(int num) { money_ += num; }
    void ApplyDamage(int da);
    void SetUpRotate(XMFLOAT3 rot) { upTrans_.rotate_ = rot; }

    XMVECTOR GetMoveVec() { return XMLoadFloat3(&moveVec_); }  //�ړ������i���K���ς�
    XMVECTOR GetDirectionVec();                                 //�����Ă�������i���K���ς�
    XMFLOAT3 GetUpRotate() { return upTrans_.rotate_; }
    int GetModelHandle() { return hModel_[0]; }

    PlayerCommand* GetCommand() { return pCommand_; }
    Aim* GetAim() { return pAim_; }
   
    void SetWeapon(WeaponBase* weapon);
    void WeaponChange();    //Input���������ꍇcurrentSubIndex��ς���
    void SubWeaponRemove();
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }
};

