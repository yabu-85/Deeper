#include "DropTable.h"
#include "EnemyBase.h"
#include "../GameManager.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/PlayerData.h"

namespace DropTable {
	struct Table {
		unsigned money_;
		unsigned weaponParcent_;	//0�`100
		unsigned healingItemParcent_;	//0�`100
		Table(unsigned m, unsigned w, unsigned h) : money_(m), weaponParcent_(w), healingItemParcent_(h) {}
	} table_[ENEMY_MAX]{
		{ 30, 50, 50 }	//FEET
	};

	void DropItem(int type, XMFLOAT3 pos)
	{
		PlayerData::AddMoney(table_[type].money_);
		if (rand() % 100 < (int)table_[type].weaponParcent_) {
			WeaponObjectManager* ma = GameManager::GetWeaponObjectManager();
			ma->AddWeaponObject((WeaponObjectManager::WEAPON_TYPE)type, pos);
		}

	}

}