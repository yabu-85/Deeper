#include "DropTable.h"
#include "EnemyBase.h"
#include "Player.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"

DropTable::DropTable()
{
	table_[ENEMY_TYPE::ENEMY_FEET] = { 30, 50, 50 };
	table_[ENEMY_TYPE::ENEMY_MASTERHAND] = { 50, 50, 50 };
}

void DropTable::DropItem(int type, XMFLOAT3 pos)
{
	Player* pPlayer = static_cast<Player*>(GameManager::GetParent()->FindObject("Player"));
	pPlayer->AddMoney(table_[type].money_);

	if (rand() % 100 < (int)table_[type].healingItemParcent_) {

	}

	if (rand() % 100 < (int)table_[type].weaponParcent_) {
		WeaponObjectManager* ma = GameManager::GetWeaponObjectManager();
		ma->AddWeaponObject((WEAPON_TYPE)type, pos);
	}

}
