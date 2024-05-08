#include "EnemyUi.h"
#include "EnemyBase.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Image.h"
#include "../Player/Player.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Easing.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"

namespace {
	static const float MAX_DRAW_LENGTH = 50.0f;	//HP�̍ő�`�拗��
	static const float DEFAULT_SIZE_X = 2.0f;	//HP�̕`��T�C�Y
	static const float DEFAULT_SIZE_Y = 0.5f;	//HP�̕`��T�C�Y
	static const float DRAW_RANGE = 0.9f;		//HP�̕`��͈́i�X�N���[���j
	static const int MAX_ALPHA = 255;
	static const int ALPHA_VALUE = 30;
	static const float PngSizeX = 256.0f;
	static float halfSize = 0.0f;
}

//���ꏈ�����点��̂ł�
void EnemyUi::SetGaugeAlpha(int value)
{
	gaugeAlpha_ += value;
	if (gaugeAlpha_ > MAX_ALPHA) {
		gaugeAlpha_ = MAX_ALPHA;
	}
	else if (gaugeAlpha_ < 0) {
		gaugeAlpha_ = 0;
	}
	Image::SetAlpha(hPict_[0], gaugeAlpha_);
	Image::SetAlpha(hPict_[1], gaugeAlpha_);
}

EnemyUi::EnemyUi(EnemyBase* parent)
	: pParent_(parent), hPict_{ -1, -1, -1 }, parcent(1.0f), height_(0.0f), gaugeAlpha_(0), foundParcent_(0.0f), isDraw_(true), isBossGauge_(false)
{
}

EnemyUi::~EnemyUi()
{
}

void EnemyUi::Initialize(float height)
{
    height_ = height;

	std::string fileName[] = { "Gauge", "GaugeFrame", "TargetFound" };
	for (int i = 0; i < MAX; i++) {
		hPict_[i] = Image::Load("Image/" + fileName[i] + ".png");
		assert(hPict_[i] >= 0);
	}

	halfSize = PngSizeX / (float)Direct3D::screenWidth_;

	transform_[0].scale_.x = DEFAULT_SIZE_X;
	transform_[0].scale_.y = DEFAULT_SIZE_Y;
	transform_[1] = transform_[0];
}

void EnemyUi::Draw()
{
	//�\�����Ȃ�
	if (!isDraw_) {
		SetGaugeAlpha(-ALPHA_VALUE);
		if (gaugeAlpha_ > 0) {
			for (int i = 0; i < 2; i++) {
				Image::SetTransform(hPict_[i], transform_[i]);
				Image::Draw(hPict_[i]);
			}
		}
		return;
	}

	//�G�̈ʒu
	XMFLOAT3 pos = pParent_->GetPosition();
	pos.y += height_; 
	
	//�X�N���[���|�W�V����
	XMFLOAT3 scrPos = Camera::CalcScreenPosition(pos);

	//�^�[�Q�b�g�����̕\��
	if (foundParcent_ > 0.0f) {
		foundParcent_ -= 0.01f;
		int alpha = (int)(Easing::QuintIn(foundParcent_) * (float)MAX_ALPHA);
		Transform foundTrans;
		foundTrans.position_ = scrPos;
		Image::SetAlpha(hPict_[FOUND], alpha);
		Image::SetTransform(hPict_[FOUND], foundTrans);
		Image::Draw(hPict_[FOUND]);
	}

	if (isBossGauge_) {
		for (int i = 0; i < 2; i++) {
			float xSize = 2.0f;
			float ySize = 1.5f;
			Transform t = transform_[i];
			t.position_.x = -halfSize * xSize;
			t.position_.y = 0.85f;
			t.scale_.x *= xSize;
			t.scale_.y *= ySize;
			Image::SetTransform(hPict_[i], t);
			Image::Draw(hPict_[i]);
		}

		return;
	}

	//HP�ő�̏ꍇ�E���ɕ\������Ă���ꍇ�����I���
	if (parcent >= 1.0f) return;

	//��p��������
	if (!Camera::IsScreenPositionWithinScreen(scrPos, DRAW_RANGE)) {
		SetGaugeAlpha(-ALPHA_VALUE);
	}
	//�����Ő���
	else {
		float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), pos);
		if (dist < MAX_DRAW_LENGTH) SetGaugeAlpha(ALPHA_VALUE);
		else SetGaugeAlpha(-ALPHA_VALUE);
	}

	//�����x�P�ȏ�Ȃ�HPGauge�\��
	if (gaugeAlpha_ > 0) {
		for (int i = 0; i < 2; i++) {
			transform_[i].position_.x = scrPos.x - halfSize;
			transform_[i].position_.y = scrPos.y;
			Image::SetTransform(hPict_[i], transform_[i]);
			Image::Draw(hPict_[i]);
		}
	}
}

void EnemyUi::SetParcent(float f)
{
	parcent = f;
	transform_[0].scale_.x = parcent * DEFAULT_SIZE_X;
}

void EnemyUi::SetIsDraw(bool b)
{
	isDraw_ = b;
}

void EnemyUi::InitTargetFoundUi()
{
	foundParcent_ = 1.0f;
}

void EnemyUi::SetAlphaMax()
{
	gaugeAlpha_ = MAX_ALPHA;
}
