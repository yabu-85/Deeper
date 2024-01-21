#include "DropTable.h"
#include "EnemyBase.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"
#include "PlayerData.h"

DropTable::DropTable()
{
	//Monew, weapon, health
	table_[ENEMY_TYPE::ENEMY_FEET] = { 30, 50, 50 };
	table_[ENEMY_TYPE::ENEMY_MASTERHAND] = { 50, 50, 50 };
}

void DropTable::DropItem(int type, XMFLOAT3 pos)
{
	PlayerData::AddMoney(table_[type].money_);

	if (rand() % 100 < (int)table_[type].weaponParcent_) {
		WeaponObjectManager* ma = GameManager::GetWeaponObjectManager();
		ma->AddWeaponObject((WeaponObjectManager::WEAPON_TYPE)type, pos);
	}

}
