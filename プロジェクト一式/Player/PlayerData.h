#pragma once
#include "../Engine/SceneManager.h"

class WeaponBase;

namespace PlayerData
{
	enum WEAPON_TYPE {
		MAIN_SWORD_WEAPON = 1,
		NORMAL_BULLET_WEAPON,
		STONE_ARM_WEAPON,
		MAX,
	};

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

	void SetWeaponData(int index, int type, WeaponBase* weapon);
	WeaponData& GetWeaponData(int index);

	void AddClearStageCount(SCENE_ID id);
	int GetClearStageCount();
	int GetReceiveDamage();

};

