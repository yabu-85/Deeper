#include "TitleUIManager.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

TitleUIManager::TitleUIManager()
	:UIManager(), hPict_{ -1, -1 }
{
	const char* fileName[] = { "Image/Title.png", "Image/TitleBG.png" };
	const int png = sizeof(fileName) / sizeof(fileName[0]);
	for (int i = 0; i < png; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}
}

TitleUIManager::~TitleUIManager()
{
	UIManager::~UIManager();

}

void TitleUIManager::Draw()
{
	Transform title;
	title.position_.y = 0.5f;
	Image::SetTransform(hPict_[0], title);
	Image::Draw(hPict_[0]);

	float screenWidth = (float)Direct3D::screenWidth_;		//ÉXÉNÉäÅ[ÉìÇÃïù
	float screenHeight = (float)Direct3D::screenHeight_;
	XMFLOAT3 size = Image::GetTextureSize(hPict_[1]);
	Transform bg;
	bg.scale_.x = (screenWidth / size.x);
	bg.scale_.y = (screenHeight / size.y);
	Image::SetTransform(hPict_[1], bg);
	Image::Draw(hPict_[1]);

	UIManager::Draw();

}
