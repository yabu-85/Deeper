#include "PlayerData.h"
#include "PlayerWeapon.h"
#include "Player.h"
#include "LifeManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Engine/Text.h"
#include "../GameManager.h"

namespace PlayerData {
    PlayerStats data_;
    Text* pTextP = nullptr;

    void Initialize()
    {
        ResetData();

        pTextP = new Text();
        pTextP->Initialize();

    }

    void Draw()
    {
        if(rand() % 5 == 0) pTextP->Draw(30, 250, data_.clearStageCount_);

    }

    void SceneTransitionInitialize()
    {
    }

    void ResetData()
    {
        data_.clearStageCount_ = 0;
        data_.receiveDamage_ = 0;
        for (int i = 0; i < 2; i++) {
            data_.subWeapon_[i].durability_ = 0;
            data_.subWeapon_[i].type_ = 0;
        }
    }

    void SetWeaponData(int i, WeaponBase* weapon) {
        data_.subWeapon_[i].type_ = i;
        data_.subWeapon_[i].durability_ = weapon->GetDurability();
    }

    void SavePlayerData()
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
            if (name == "StoneArmWeapon") SetWeaponData(1, weapon);
            else if (name == "TestWeaponSub") SetWeaponData(2, weapon);
        }
    }

    void SetWeaponData(int index, const WeaponData& data) { data_.subWeapon_[index] = data; }
    WeaponData& GetWeaponData(int index) { return data_.subWeapon_[index]; }
    
    //ステージクリア数を増やす：今のところTitle以外だったら増やす
    void AddClearStageCount(SCENE_ID id) { 
        if (id == SCENE_ID_STAGE1 || id == SCENE_ID_STAGE2 || id == SCENE_ID_STAGE3) data_.clearStageCount_++;
    }

    int GetClearStageCount() { return data_.clearStageCount_; }
    int GetReceiveDamage() { return data_.receiveDamage_; }

}




