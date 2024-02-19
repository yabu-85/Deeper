#pragma once
#include "../Engine/SceneManager.h"

namespace PlayerData
{
	struct WeaponData {
		int type_;
		unsigned durability_;
	};

    struct PlayerStats {
		int clearStageCount_;
		int receiveDamage_;
		WeaponData subWeapon_[2];
		
	};

	void Initialize();
	void Draw();
	void SceneTransitionInitialize();
	void ResetData();

	void SavePlayerData();
	void SetWeaponData(int index, const WeaponData& data);
	WeaponData& GetWeaponData(int index);
	void AddClearStageCount(SCENE_ID id);
	int GetClearStageCount();
	int GetReceiveDamage();


};

