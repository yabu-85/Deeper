#include "EnemyUi.h"
#include "EnemyBase.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"
#include "Player.h"
#include "Engine/Direct3D.h"

namespace {
	const float maxLength = 50.0f;
	const float defSizeX = 2.0f;
	const float defSizeY = 0.5f;
	const float drawSize = 0.85f;
	const int maxAlpha = 255;
	const int alphaValue = 30;
	float halfSize = 0.0f;
}

//‚±‚êˆ—Œ¸‚ç‚¹‚é‚Ì‚Å‚Í
void EnemyUi::SetGageAlpha(int value)
{
	gageAlpha_ += value;
	if (gageAlpha_ > maxAlpha) {
		gageAlpha_ = maxAlpha;
	}
	else if (gageAlpha_ < 0) {
		gageAlpha_ = 0;
	}
	Image::SetAlpha(hPict_[0], gageAlpha_);
	Image::SetAlpha(hPict_[1], gageAlpha_);
}

EnemyUi::EnemyUi(EnemyBase* parent)
	: pParent_(parent), pPlayer_(nullptr), hPict_{ -1, -1 }, parcent(1.0f), height_(0.0f), gageAlpha_(0)
{
}

EnemyUi::~EnemyUi()
{
}

void EnemyUi::Initialize(float height)
{
    height_ = height;
	pPlayer_ = (Player*)pParent_->FindObject("Player");

	hPict_[0] = Image::Load("Png/Gauge.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/GaugeFrame.png");
	assert(hPict_[1] >= 0);

	halfSize = 256.0f / (float)Direct3D::screenWidth_;

	transform_[0].scale_.x = defSizeX;
	transform_[0].scale_.y = defSizeY;
	transform_[1] = transform_[0];


}

void EnemyUi::Draw()
{
	if (parcent >= 1.0f) {
		return;
	}

	XMFLOAT3 pos = pParent_->GetPosition();
	pos.y += height_;

	XMVECTOR v2 = XMVector3TransformCoord(XMLoadFloat3(&pos), Camera::GetViewMatrix());
	v2 = XMVector3TransformCoord(v2, Camera::GetProjectionMatrix());
	float x = XMVectorGetX(v2);
	float y = XMVectorGetY(v2);

	//‰æŠp§ŒÀ‚·‚é
	if (x >= drawSize || y >= drawSize || x <= -drawSize || y <= -drawSize || XMVectorGetZ(v2) > 1.0f) {
		SetGageAlpha(-alphaValue);
	}
	else {
		XMFLOAT3 fCamPos = pPlayer_->GetPosition();
		XMVECTOR vCamPos = XMLoadFloat3(&fCamPos);
		XMVECTOR vPos = XMLoadFloat3(&pos);
		float length = XMVectorGetX(XMVector3Length(vCamPos - vPos));

		//”ÍˆÍ“à‚©‚Ç‚¤‚©
		if (length < maxLength) {
			SetGageAlpha(alphaValue);
		}
		else {
			SetGageAlpha(-alphaValue);
		}
	}

	//“§–¾“x‚PˆÈã‚È‚çHPGauge•\Ž¦
	if(gageAlpha_ > 0)
	for (int i = 0; i < 2; i++) {
		transform_[i].position_.x = x - halfSize;
		transform_[i].position_.y = y;
		Image::SetTransform(hPict_[i], transform_[i]);
		Image::Draw(hPict_[i]);
	}

}

void EnemyUi::SetParcent(float f)
{
	parcent = f;
	transform_[0].scale_.x = parcent * defSizeX;
}
