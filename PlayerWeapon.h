#pragma once

class WeaponBase;
class Player;

class PlayerWeapon
{
    int currentSubIndex_;       //���I�����Ă�Sub��Index
    WeaponBase* pMainWeapon_;
    WeaponBase* pSubWeapon_[2];
    Player* pPlayer_;

public:
    PlayerWeapon(Player* pPlayer);
    void DrawWeapon();

    void SetMainWeapon();
    void SetWeapon(WeaponBase* weapon);
    void WeaponChangeIndex();       //Input���������ꍇcurrentSubIndex��ς���
    void SubWeaponRemove();
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }

};

