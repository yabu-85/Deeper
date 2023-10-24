#include "EnemyUi.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Direct3D.h"

EnemyUi::EnemyUi(EnemyBase* parent)
	:hPict_{-1, -1}, pParent_(parent)
{
}

void EnemyUi::Initialize()
{
	hPict_[0] = Image::Load("Png/Gauge.png");
	assert(hPict_[0] >= 0);
	
	hPict_[1] = Image::Load("Png/GaugeFrame.png");
	assert(hPict_[1] >= 0);
}

void EnemyUi::Draw()
{
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Image::SetTransform(hPict_[1], transform_);
	Image::Draw(hPict_[1]);
}
