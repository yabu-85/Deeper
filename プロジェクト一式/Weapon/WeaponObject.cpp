#include "WeaponObject.h"
#include "../Engine/Model.h"
#include "../Engine/Global.h"
#include "../UI/InteractionUI.h"

WeaponObject::WeaponObject(GameObject* parent)
	: GameObject(parent, "WeaponObject"), hModel_(-1), type_(-1), pInteractionUI_(nullptr)
{
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
	pInteractionUI_ = new InteractionUI(this);
	pInteractionUI_->Initialize();
	pInteractionUI_->SetLongPress(true);

}

void WeaponObject::Update()
{
}

void WeaponObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WeaponObject::Release()
{
	pInteractionUI_->Release();
	SAFE_DELETE(pInteractionUI_);

}

void WeaponObject::LoadModel(std::string _name)
{
	hModel_ = Model::Load("Model/" + _name + ".fbx");
	assert(hModel_ >= 0);
}
