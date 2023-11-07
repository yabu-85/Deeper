#include "DropTable.h"
#include "EnemyBase.h"
#include "Player.h"
#include "GameManager.h"
#include "WeaponObjectManager.h"

DropTable::DropTable(GameManager* parent)
	: pGameManager_(parent)
{
	table_[ENEMY_TYPE::ENEMY_FEET] = { 30, 50, 50 };
	table_[ENEMY_TYPE::ENEMY_MASTERHAND] = { 50, 50, 50 };

}

void DropTable::DropItem(int type, XMFLOAT3 pos)
{
	Player* pPlayer = static_cast<Player*>(pGameManager_->FindObject("Player"));
	pPlayer->AddMoney(table_[type].money_);

	if (rand() % 100 < (int)table_[type].healingItemParcent_) {

	}

	if (rand() % 100 < (int)table_[type].weaponParcent_) {
		GameManager* gm = static_cast<GameManager*>(pGameManager_->FindObject("GameManager"));
		WeaponObjectManager* ma = gm->GetWeaponObjectManager();
		ma->AddWeaponObject((WEAPON_TYPE)type, pos);
	}

}
