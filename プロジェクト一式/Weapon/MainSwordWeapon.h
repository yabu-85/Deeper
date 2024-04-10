#pragma once
#include "WeaponBase.h"
#include "../State/StateBase.h"

class Player;
class SphereCollider;
class PolyLine;
class DamageController;

class MainSwordWeapon : public WeaponBase
{
    int polyCreatTime_;     //�|���S���̍쐬����
    XMFLOAT3 wandPos_;      //�v���C���[�̎�̃|�W�V����
    XMFLOAT3 direction_;    //����̐i�s����

    Player* pPlayer_;
    SphereCollider* sphere_[2];
    PolyLine* pPolyLine_;
    DamageController* pDamageController_;

public:
    MainSwordWeapon(GameObject* parent);
    ~MainSwordWeapon() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget) override;
    void ChangeAttackState() override;
    void ResetState() override;

    //�U������R���C�_�[�̌v�Z
    void CalcSwordTrans();

    //PolyLine��i�X�����悤��
    void SetPolyCreatTime();

    void DamageInfoReset();
    void SetDamageInfoCombo1();
    void SetDamageInfoCombo2();
    void SetDamageInfoCombo3();
};

//-------------------State----------------------

class MainSwordWeaponCombo1 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo1(StateManager* owner);
    const char* GetName() const override { return "Combo1"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class MainSwordWeaponCombo2 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo2(StateManager* owner);
    const char* GetName() const override { return "Combo2"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};

class MainSwordWeaponCombo3 : public StateBase
{
    bool next_;
    int time_;
public:
    MainSwordWeaponCombo3(StateManager* owner);
    const char* GetName() const override { return "Combo3"; }
    void Update() override;
    void OnEnter() override;
    void OnExit() override;
};