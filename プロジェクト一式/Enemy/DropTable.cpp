#include "DropTable.h"
#include "EnemyBase.h"
#include "../GameManager/GameManager.h"
#include "../Scene/StageBase.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../Item/RecoveryItem.h"
#include "../Player/Player.h"
#include "../Engine/GameObject.h"

namespace DropTable {
	struct Table {
		unsigned weaponParcent_;	//0～100
		unsigned healingItemParcent_;	//0～100・未実装
		Table(unsigned w, unsigned h) : weaponParcent_(w), healingItemParcent_(h) {}
	} table_[ENEMY_MAX]{
		{ 50, 50 },	//FEET
		{ 50, 50 },	//THROW
		{ 50, 50 },	//Melee
		{ 100, 50 },	//SwordBoss
	};

	//回復はプレイヤーのHPが低いとドロリツ高く
	//高い時は低く
	// 
	//weaponは場にある数とかを参考にすると良きかも

}

void DropTable::DropItem(ENEMY_TYPE type, XMFLOAT3 pos)
{
	Player* player = GameManager::GetPlayer();
	float hpPar = 1.0f - (float)player->GetHP() / (float)player->GetMaxHP();
	int itemPar = (int)((float)table_[type].healingItemParcent_ * hpPar);
	int par = rand() % 100;

	if (par < itemPar) {
		RecoveryItem* item = Instantiate<RecoveryItem>(GameManager::GetStage());
		item->SetPosition(pos);
	}
	if ((unsigned)(rand() % 100) < table_[type].weaponParcent_) {
		WeaponObjectManager* ma = GameManager::GetWeaponObjectManager();
		ma->AddWeaponObject(type, pos);
	}
}