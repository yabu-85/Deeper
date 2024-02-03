#include "PlayerWeapon.h"
#include "../Weapon/WeaponBase.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "PlayerData.h"

#include "../Weapon/TestWeaponMain.h"
#include "../Weapon/TestWeaponSub.h"

#include "../Engine/Text.h"

namespace {
    Text* pText = new Text;
    
}

PlayerWeapon::PlayerWeapon(Player* pPlayer)
    : pPlayer_(pPlayer), pMainWeapon_(nullptr), pSubWeapon_{nullptr, nullptr}, currentSubIndex_(0)
{
    pText->Initialize();

}

void PlayerWeapon::DrawWeapon()
{
    //•Ší‚Ì•\Ž¦”ñ•\Ž¦
    if (currentSubIndex_ == 0) {
        if (pSubWeapon_[0]) pSubWeapon_[0]->Visible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Invisible();
    }
    else {
        if (pSubWeapon_[1]) pSubWeapon_[1]->Visible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Invisible();
    }

    pText->Draw(1000, 600, currentSubIndex_);
    if (pSubWeapon_[0]) {
        const char* cstr = pSubWeapon_[0]->GetObjectName().c_str();
        pText->Draw(1000, 650, cstr);
    }
    if (pSubWeapon_[1]) {
        const char* cstr = pSubWeapon_[1]->GetObjectName().c_str();
        pText->Draw(1000, 700, cstr);
    }

}

void PlayerWeapon::SetPlayerDataWeapon()
{
    pMainWeapon_ = Instantiate<TestWeaponMain>(pPlayer_);
    pMainWeapon_->SetOffsetScale(XMFLOAT3(0.1f, 1.0f, 0.1f));

    int type[2] = { 0,0 };
    type[0] = PlayerData::GetWeaponData(0).type_;
    type[1] = PlayerData::GetWeaponData(1).type_;
    
    if (type[0] >= 1) {
        if(type[0] == 1) SetWeapon(Instantiate<TestWeaponSub>(pPlayer_));
        
    }
    if (type[1] >= 1) {
        if (type[1] == 1) SetWeapon(Instantiate<TestWeaponSub>(pPlayer_));

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
    if (pPlayer_->GetCommand()->CmdCenterUp()) {
        currentSubIndex_ = 0;
        return;
    }
    if (pPlayer_->GetCommand()->CmdCenterDown()) {
        currentSubIndex_ = 1;
        return;
    }
}

void PlayerWeapon::SubWeaponRemove()
{
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = nullptr;
}
