#include "EnemySpawnCtrl.h"
#include "MasterHand.h"

void EnemySpawnCtrl::Initialize(GameObject* parent)
{
	pParent_ = parent;

	enemyList_.push_back(Instantiate<MasterHand>(pParent_));
	enemyList_.at(0)->SetPosition(XMFLOAT3(10.0f, 2.0f, 0.0f));

}

void EnemySpawnCtrl::Release()
{
	enemyList_.clear();

}

int EnemySpawnCtrl::SpawnEnemy(int type)
{
	//Ç‹Ç∑ÇΩÅ[ÇÕÇÒÇ«
	if (type == ENEMY_MASTERHAND) {
		enemyList_.push_back(Instantiate<MasterHand>(pParent_));
		return enemyList_.size() - 1;
	}

	return 0;
}

std::vector<EnemyBase*>& EnemySpawnCtrl::GetAllEnemy()
{
	//Ç‹Ç∏éÄÇÒÇ≈ÇΩÇÁÉäÉXÉgÇ©ÇÁè¡Ç∑
	for (int i = 0; i < enemyList_.size();) {
		if (enemyList_.at(i)->IsDead())
			enemyList_.erase(enemyList_.begin() + i);
		else
			i++;
	}

	return enemyList_;
}
