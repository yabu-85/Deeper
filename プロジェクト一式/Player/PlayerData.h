#pragma once
#include "../Engine/SceneManager.h"

class WeaponBase;

/// <summary>
/// シーン移動しても情報を共有するためのクラス
/// プレイヤー自体の情報と、Weaponの情報を持つ
/// </summary>
namespace PlayerData
{
	enum class WEAPON_TYPE {
		MAIN_SWORD_WEAPON = 1,
		NORMAL_BULLET_WEAPON,
		STONE_ARM_WEAPON,
		WEAPON_MAX,
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

