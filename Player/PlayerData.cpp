#include "PlayerData.h"
#include "PlayerWeapon.h"
#include "../GameManager.h"
#include "Player.h"
#include "../Weapon/WeaponBase.h"
#include "../Engine/Text.h"

namespace PlayerData {
    PlayerStats data_;
    Text* pText = nullptr;

    void Initialize()
    {
        data_.clearStageCount_ = 0;
        data_.money_ = 0;

        for (int i = 0; i < 2; i++) {
            data_.subWeapon_[i].durability_ = 0;
            data_.subWeapon_[i].type_ = 0;
        }

        pText = new Text();
        pText->Initialize();

    }

    void Draw()
    {
        pText->Draw(30, 250, data_.clearStageCount_);

    }

    void SavePlayerData()
    {
        Player* player = GameManager::GetPlayer();
        if (player == nullptr) return;

        PlayerWeapon* plaWeapon = player->GetPlayerWeapon();
        if (plaWeapon == nullptr) return;

        WeaponBase* weapon[2] = { nullptr, nullptr };
        for (int i = 0; i < 2; i++) {
            weapon[i] = plaWeapon->GetSubWeapon(i);
            if (weapon[i] == nullptr) continue;
            if (weapon[i]->GetObjectName() == "TestWeaponSub") {
                data_.subWeapon_[i].type_ = 1;
                data_.subWeapon_[i].durability_ = weapon[i]->GetDurability();
            }
        }
    }

    void SetWeaponData(int index, const WeaponData& data) { data_.subWeapon_[index] = data; }
    WeaponData& GetWeaponData(int index) { return data_.subWeapon_[index]; }
    
    void AddClearStageCount(SCENE_ID id) { if (id != SCENE_ID_TITLE) data_.clearStageCount_++; }
    void AddMoney(int i) { data_.money_ += i; }

}




