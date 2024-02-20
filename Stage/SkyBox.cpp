#include "SkyBox.h"
#include "../GameManager.h"
#include "../Engine/Model.h"

namespace {
	float boxSize = 10.0f;
}

SkyBox::SkyBox(GameObject* parent)
	: GameObject(parent, "Warp"), hModel_(-1)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Initialize()
{
	transform_.scale_ = { boxSize, boxSize, boxSize };

}

void SkyBox::Update()
{
}

void SkyBox::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_, 7);

}

void SkyBox::Release()
{
}

void SkyBox::LoadModel(std::string fileName)
{
	hModel_ = Model::Load(fileName);
	assert(hModel_ >= 0);

}
