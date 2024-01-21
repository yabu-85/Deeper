#pragma once
#include "Engine/SceneManager.h"

namespace PlayerData
{
	struct WeaponData {
		int type_;
		unsigned durability_;
	};

    struct PlayerStats {
		int clearStageCount_;
		int money_;
		WeaponData subWeapon_[2];
		
	};

	void Initialize();
	void Draw();

	void SavePlayerData();
	void SetWeaponData(int index, const WeaponData& data);
	WeaponData& GetWeaponData(int index);
	
	void AddClearStageCount(SCENE_ID id);
	void AddMoney(int i);

};

