#include "LifeManager.h"
#include "../GameManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Sprite.h"
#include "../Engine/Text.h"

namespace
{
	static const int DEFAULT_ALPHA = 255;	//���G���Ԃ����炷��
	static const int DEFAULT_LIFE = 100;	//���ʂ�Player�̃��C�t�̐�
	 
}

namespace LifeManager
{
	Sprite* damageImage_;
	int invincibleTime_;		//�_���[�W�\�����Ԃ̌v�Z�p

	int playerLife_;			//�v���C���[�̃��C�t��

	void LifeManager::Initialize()
	{
		damageImage_ = new Sprite;
		damageImage_->Load("Image/damage.png"); 
		
		playerLife_ = DEFAULT_LIFE;
		invincibleTime_ = 0;
	}

	void Update()
	{
		invincibleTime_--;

	}

	void LifeManager::Draw()
	{
		//���C�t�̕\��

		if (invincibleTime_ > 0) DamageEffectDraw();
	}

	void DamageEffectDraw()
	{
		//�؂蔲���͈͂����Z�b�g�i�摜�S�̂�\������j
		XMFLOAT3 size = damageImage_->GetTextureSize();
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;
		Transform t;
		damageImage_->Draw(t, rect, 0.1f, 1);

	}

	void LifeManager::SceneTransitionInitialize()
	{
	}

	void LifeManager::Damage(int i)
	{
		if (IsInvincible()) return;

		invincibleTime_ = INVINCIBLE;
		playerLife_ -= i;
	}

	void LifeManager::ResetLife() { playerLife_ = DEFAULT_LIFE; }
	bool IsInvincible() { return (invincibleTime_ > 0.0f); }
	bool LifeManager::IsDie() { return playerLife_ <= 0; }

}