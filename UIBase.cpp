#include "UIBase.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

namespace {
	const int PNG_COUNT = 3;
}

UIBase::UIBase()
	:hPict_{-1, -1, -1}, alpha_{255}, isBound_(false)
{
}

void UIBase::Initialize(std::string name)
{
	const std::string fileName[] = { "Png/ButtonFrame1.png", "Png/ButtonFrame2.png", "Png/" + name + ".png" };
	for (int i = 0; i < PNG_COUNT; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	transform_.scale_ = XMFLOAT3(0.5f, 0.5f, 0.0f);
	transform_.position_.y = -0.0f;

}

void UIBase::Draw()
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);

	//‰Ÿ‚µ‚Ä‚È‚¢Žž‚Ífalse(0)‚¾‚©‚ç‚P‚ª•\Ž¦‚³‚ê‚é
	Image::SetTransform(hPict_[isBound_], transform_);
	Image::Draw(hPict_[isBound_]);

	Image::SetTransform(hPict_[2], transform_);
	Image::Draw(hPict_[2]);

}