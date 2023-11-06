#pragma once

class EnemyBase;

class DropTable
{
	int money_;
	unsigned parcent_;	//0Å`100

	EnemyBase* pEnemyBase_;

public:
	DropTable(EnemyBase* parent);
	void DropItem();

	void SetMoney(unsigned num) { money_ = num; }
	void SetParcent(unsigned num) { parcent_ = num; }
};

