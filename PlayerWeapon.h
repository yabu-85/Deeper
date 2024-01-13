#pragma once

class WeaponBase;
class Player;

class PlayerWeapon
{
    int currentSubIndex_;       //¡‘I‘ğ‚µ‚Ä‚éSub‚ÌIndex
    WeaponBase* pMainWeapon_;
    WeaponBase* pSubWeapon_[2];
    Player* pPlayer_;

public:
    PlayerWeapon(Player* pPlayer);
    void DrawWeapon();

    void SetMainWeapon();
    void SetWeapon(WeaponBase* weapon);
    void WeaponChangeIndex();       //Input‚ª‚ ‚Á‚½ê‡currentSubIndex‚ğ•Ï‚¦‚é
    void SubWeaponRemove();
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }

};

