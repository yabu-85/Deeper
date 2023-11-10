#include "TargetConditionCount.h"
#include "EnemyBase.h"
#include "EnemySpawnCtrl.h"
#include <vector>

TargetConditionCount::TargetConditionCount(int count) : Condition(), countThreshold_(count)
{
}

TargetConditionCount::Status TargetConditionCount::Update()
{
	std::vector<EnemyBase*> eneList;

	return Status();
}
