#pragma once
#include "../Engine/Transform.h"
#include "../Enemy/EnemyManager.h"

class WeaponBase;
class Player;

/// <summary>
/// �v���C���[������Weapon�摜�\����A���O��������
/// </summary>
class PlayerWeapon
{
    enum SELECT_TYPE {
        SELECT_VALID = 0,
        SELECT_INVALID,
        NO_SELECT_VALID,
        NO_SELECT_INVALID,
        SELECT_MAX,
    };
    int hPictSelect_[SELECT_MAX];   //�摜����
    Transform boxTrans_[2];         //�摜��Transform
    int drawPictSelect_[2];         //�{�b�N�X�̂ǂ��\�����邩�̏��

    //WeaponImage
    int hPictWeapon_[(int)ENEMY_TYPE::ENEMY_MAX];       //�摜����
    int drawPictWeapon_[(int)ENEMY_TYPE::ENEMY_MAX];    //�ǂ̉摜��\�����邩�̏��
    Transform weaponTrans_[2];                          //�摜��Transform      

    int currentSubIndex_;       //���I�����Ă�Sub��Index
    WeaponBase* pMainWeapon_;
    WeaponBase* pSubWeapon_[2];
    Player* pPlayer_;

    //SetWeapon����g��
    void SetWeaponData(int index, WeaponBase* weapon);

public:
    PlayerWeapon(Player* pPlayer);
    void SceneTransitionInitialize();
    
    void DrawWeapon();                  //WeaponBox/Image�̕\��
    void SetPlayerDataWeapon();         //PlayerData�̏������Weapon���Z�b�g
    void SetWeapon(WeaponBase* weapon); //Weapon���Z�b�g
    void WeaponChangeIndex();           //Input���������ꍇcurrentSubIndex��ς���
    void SubWeaponRemove();             //currentSubIndex�̕��������

    //�Q�b�g�֐�
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }
    WeaponBase* GetSubWeapon(int index) { return pSubWeapon_[index]; }

};

