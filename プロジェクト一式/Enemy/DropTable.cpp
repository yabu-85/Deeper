#include "DropTable.h"
#include "EnemyBase.h"
#include "../GameManager/GameManager.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/PlayerData.h"

namespace DropTable {
	struct Table {
		unsigned weaponParcent_;	//0〜100
		unsigned healingItemParcent_;	//0〜100・未実装
		Table(unsigned w, unsigned h) : weaponParcent_(w), healingItemParcent_(h) {}
	} table_[ENEMY_MAX]{
		{ 50, 50 },	//FEET
		{ 50, 50 },	//THROW
		{ 50, 50 },	//Melee
	};

	//回復はプレイヤーのHPが低いとドロリツ高く
	//高い時は低く
	// 
	//weaponは場にある数とかを参考にすると良きかも

}

void DropTable::DropItem(ENEMY_TYPE type, XMFLOAT3 pos)
{
	if ((unsigned)(rand() % 100) < table_[type].weaponParcent_) {
		WeaponObjectManager* ma = GameManager::GetWeaponObjectManager();
		ma->AddWeaponObject((WeaponObjectManager::WEAPON_TYPE)type, pos);
	}
}