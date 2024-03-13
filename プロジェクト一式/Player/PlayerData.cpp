#include "PlayerData.h"
#include "PlayerWeapon.h"
#include "Player.h"
#include "LifeManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Engine/Text.h"
#include "../GameManager/GameManager.h"

namespace PlayerData {
    PlayerStats data_;
}

void PlayerData::Initialize()
{
    ResetData();

}

void PlayerData::Draw()
{
}

void PlayerData::SceneTransitionInitialize()
{
}

void PlayerData::ResetData()
{
    data_.clearStageCount_ = 0;
    data_.receiveDamage_ = 0;
    for (int i = 0; i < 2; i++) {
        data_.subWeapon_[i].durability_ = 0;
        data_.subWeapon_[i].type_ = 0;
    }
}

void PlayerData::SetWeaponData(int index, int type, WeaponBase* weapon) {
    data_.subWeapon_[index].type_ = type;
    data_.subWeapon_[index].durability_ = weapon->GetDurability();
}

void PlayerData::SavePlayerData()
{
    Player* player = GameManager::GetPlayer();
    if (player == nullptr) return;

    PlayerWeapon* plaWeapon = player->GetPlayerWeapon();
    if (plaWeapon == nullptr) return;

    //HP
    data_.receiveDamage_ = LifeManager::GetReceiveDamage();

    //Weapon
    for (int i = 0; i < 2; i++) {
        WeaponBase* weapon = plaWeapon->GetSubWeapon(i);
        if (weapon == nullptr) continue;

        std::string name = weapon->GetObjectName();
        if (name == "StoneArmWeapon") 
            SetWeaponData(i, static_cast<int>(STONE_ARM_WEAPON), weapon);
        else if (name == "TestWeaponSub") 
            SetWeaponData(i, static_cast<int>(NORMAL_BULLET_WEAPON), weapon);
    }
}

//ステージクリア数を増やす：今のところTitle以外だったら増やす
void PlayerData::AddClearStageCount(SCENE_ID id) {
    if (id == SCENE_ID_STAGE1 || id == SCENE_ID_STAGE2 || id == SCENE_ID_STAGE3) data_.clearStageCount_++;
}

PlayerData::WeaponData& PlayerData::GetWeaponData(int index) { return data_.subWeapon_[index]; }
int PlayerData::GetClearStageCount() { return data_.clearStageCount_; }
int PlayerData::GetReceiveDamage() { return data_.receiveDamage_; }




