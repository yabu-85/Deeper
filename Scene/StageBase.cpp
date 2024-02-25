#include "StageBase.h"
#include "../WaveManager.h"

StageBase::StageBase(GameObject* parent, std::string name)
	:GameObject(parent, name), isCleared_(false)
{
	OutputDebugString("StageBase : Stage Constract\n");

}

void StageBase::Initialize()
{
}

void StageBase::Update()
{
}

void StageBase::Draw()
{
}

void StageBase::Release()
{
}

bool StageBase::IsClearStage()
{
	if (!isCleared_ && WaveManager::IsClearStage()) {
		isCleared_ = true;
		return true;
	}
	return false;
}
