#include "PlayerWeapon.h"
#include "Player.h"
#include "PlayerData.h"
#include "../InputManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/MainSwordWeapon.h"
#include "../Weapon/NormalBulletWeapon.h"
#include "../Weapon/StoneArmWeapon.h"

#include "../Engine/Text.h"

namespace {
    Text* pText;
    
}

PlayerWeapon::PlayerWeapon(Player* pPlayer)
    : pPlayer_(pPlayer), pMainWeapon_(nullptr), pSubWeapon_{nullptr, nullptr}, currentSubIndex_(0)
{
    pText = new Text();
    pText->Initialize();

}

void PlayerWeapon::DrawWeapon()
{
    //UI‚Ì•\Ž¦
    pText->Draw(30, 300, currentSubIndex_);

    if (pSubWeapon_[0]) {
        const char* cstr = pSubWeapon_[0]->GetObjectName().c_str();
        pText->Draw(1000, 650, cstr);
    }
    if (pSubWeapon_[1]) {
        const char* cstr = pSubWeapon_[1]->GetObjectName().c_str();
        pText->Draw(1000, 700, cstr);
    }

}

void SceneTransitionInitialize() {
    pText = new Text();
    pText->Initialize();

}

void PlayerWeapon::SetPlayerDataWeapon()
{
    pMainWeapon_ = Instantiate<MainSwordWeapon>(pPlayer_);

    for (int i = 0; i < 2; i++) {
        int type = PlayerData::GetWeaponData(i).type_;
        if(type == PlayerData::STONE_ARM_WEAPON) SetWeapon(Instantiate<StoneArmWeapon>(pPlayer_));
        else if (type == PlayerData::NORMAL_BULLET_WEAPON) SetWeapon(Instantiate<NormalBulletWeapon>(pPlayer_));
    }
}

void PlayerWeapon::SetWeapon(WeaponBase* weapon)
{
    //‹ó‚¢‚Ä‚¢‚½‚ç’Ç‰Á‚·‚é
    if (pSubWeapon_[0] == nullptr) {
        pSubWeapon_[0] = weapon;
        return;
    }
    else if (pSubWeapon_[1] == nullptr) {
        pSubWeapon_[1] = weapon;
        return;
    }

    //ã‘‚«‚·‚é
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = weapon;
}

void PlayerWeapon::WeaponChangeIndex()
{
    if (InputManager::IsCmd(InputManager::WEAPON_UP)) {
        currentSubIndex_ = 0;
        pMainWeapon_->Invisible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Visible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Invisible();
    }
    else if (InputManager::IsCmd(InputManager::WEAPON_DOWN)) {
        currentSubIndex_ = 1;
        pMainWeapon_->Invisible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Visible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Invisible();
    }
}

void PlayerWeapon::SubWeaponRemove()
{
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = nullptr;
}
