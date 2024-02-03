#include "StageBase.h"

StageBase::StageBase(GameObject* parent, std::string name)
	:GameObject(parent, name), isCleared_(false)
{
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
