#pragma once

namespace PlayerData
{
	struct WeaponData {
		int type_;
		unsigned durability_;
	};

    struct PlayerStats {
		WeaponData subWeapon_[2];
		
	};

	void SavePlayerData();
	void SetWeaponData(int index, const WeaponData& data);
	WeaponData& GetWeaponData(int index);

};

