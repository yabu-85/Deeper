#include "WeaponObject.h"
#include "Engine/Model.h"

WeaponObject::WeaponObject(GameObject* parent)
	: GameObject(parent, "WeaponObject"), hModel_(-1), type_(-1)
{
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
	transform_.position_.y += 1.0f;
	transform_.position_.x = 50.0f + rand() % 10;
	transform_.position_.z = 50.0f + rand() % 10;
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
}

void WeaponObject::LoadModel(std::string _name)
{
	hModel_ = Model::Load("Model/" + _name + ".fbx");
	assert(hModel_ >= 0);
}
