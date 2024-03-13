#pragma once
#include "../Engine/Transform.h"
#include "../Weapon/WeaponObjectManager.h"

class WeaponBase;
class Player;

class PlayerWeapon
{
    enum SELECT_TYPE {
        SELECT_VALID = 0,
        SELECT_INVALID,
        NO_SELECT_VALID,
        NO_SELECT_INVALID,
        SELECT_MAX,
    };
    int hPictSelect_[SELECT_MAX];   //画像総数
    Transform boxTrans_[2];         //画像のTransform
    int drawPictSelect_[2];         //ボックスのどれを表示するかの情報

    //WeaponImage
    int hPictWeapon_[(int)WeaponObjectManager::WEAPON_TYPE::WT_MAX];    //画像総数
    int drawPictWeapon_[(int)WeaponObjectManager::WEAPON_TYPE::WT_MAX]; //どの画像を表示するかの情報
    Transform weaponTrans_[2];                                          //画像のTransform      

    int currentSubIndex_;       //今選択してるSubのIndex
    WeaponBase* pMainWeapon_;
    WeaponBase* pSubWeapon_[2];
    Player* pPlayer_;

    //SetWeaponから使う
    void SetWeaponData(int index, WeaponBase* weapon);

public:
    PlayerWeapon(Player* pPlayer);
    void SceneTransitionInitialize();
    
    void DrawWeapon();                  //WeaponBox/Imageの表示
    void SetPlayerDataWeapon();         //PlayerDataの情報を基にWeaponをセット
    void SetWeapon(WeaponBase* weapon); //Weaponをセット
    void WeaponChangeIndex();           //Inputがあった場合currentSubIndexを変える
    void SubWeaponRemove();             //currentSubIndexの武器を消す

    //ゲット関数
    WeaponBase* GetMainWeapon() { return pMainWeapon_; }
    WeaponBase* GetSubWeapon() { return pSubWeapon_[currentSubIndex_]; }
    WeaponBase* GetSubWeapon(int index) { return pSubWeapon_[index]; }

};

