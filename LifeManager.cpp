#include "LifeManager.h"
#include "GameManager.h"
#include "Engine/Transform.h"
#include "Engine/Global.h"
#include "Engine/Sprite.h"
#include "Engine/Text.h"

//�萔
namespace
{
	static const int NORMAL_PLAYER_LIFE = 3; //���ʂ�Player�̃��C�t�̐�

}

namespace LifeManager
{
	//Player�̌��݂̃��C�t�̑���
	int playerLife_;

	//�摜
	Sprite* lifeImage[1]; //���C�t���Ƃ̉摜�ԍ�
	Sprite* damageImage_;              //�_���[�W���󂯂����̉摜�ԍ�
	Transform imageTransform_;		   //�摜�̈ʒu�E�g�嗦

	//�e�L�X�g
	Text* pLifeText_;                  //���C�t�̐��\������p
	XMFLOAT2 textPositiom_;            //�e�L�X�g�̃|�W�V����

	//�g��k��
	XMVECTOR  beforeScale_;		       //��Ԃ���O�̊g�嗦�ۑ�
	XMVECTOR  targetScale_;            //���̖ڕW�Ƃ���g�嗦

	//�_���[�W�摜�̓����x
	float damageImageAlpha_;

	//�_���[�W�摜��\�����邩�ǂ���
	bool  isDrawDamageImage_;

	//������
	void LifeManager::Initialize()
	{
		//�e���C�t�̉摜���[�h
		for (int i = 0; i < 1; i++) { lifeImage[i] = new Sprite; }

		//�_���[�W�̉摜���[�h
		damageImage_ = new Sprite;
		damageImage_->Load("Image/damage.png");

		//�e�L�X�g�̏�����
		pLifeText_ = new Text;
		pLifeText_->Initialize("Text/NumberFont.png", 128, 256, 10);

		//�摜�̃|�W�V�����ݒ�
		imageTransform_;

		//�e�L�X�g�̃|�W�V�����ݒ�
		textPositiom_ = XMFLOAT2(500.0f, 200.0f);

		playerLife_ = NORMAL_PLAYER_LIFE;
		damageImageAlpha_ = 1;
		isDrawDamageImage_ = false;
	}

	//�V�[���J�ڂ̎��̏�����
	void LifeManager::SceneTransitionInitialize()
	{
		//�e�L�X�g�̏�����
		(pLifeText_, new Text);
		pLifeText_->Initialize("Text/NumberFont.png", 128, 256, 10);
	}

	//�_���[�W�H��������ɌĂԃ��\�b�h
	void LifeManager::Damage()
	{
		//���C�t�����
		playerLife_ -= 1;

		//��������ł��Ȃ��̂Ȃ�_���[�W�֘A������
		if (!IsDie())
		{
			(damageImageAlpha_, 1);
			(isDrawDamageImage_, true);
		}
	}

	//HPUI�`��
	void LifeManager::Draw()
	{
		//�V�[�����G���h���[���Ȃ�`�悵�Ȃ�
		//if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_ENDROLES) return;

		//�摜
		//�e�N�X�`���̃T�C�Y�擾
		XMFLOAT3 size = damageImage_->GetTextureSize();

		//�؂蔲���͈͂����Z�b�g�i�摜�S�̂�\������j
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		//�\���ʒu�p
		Transform t;

		//�`��
		//damageImage_->Draw(t, rect, 150, 1);

		//�e�L�X�g
		//pLifeText_->Draw((int)textPositiom_.x, (int)textPositiom_.y, "asdfghj");

		//�����_���[�W�摜��`�悷��̂Ȃ�
		if (true || isDrawDamageImage_)
			DamageEffectDraw();

	}

	//�_���[�W�󂯂����̉��o��`��
	void DamageEffectDraw()
	{
		//�����x���X�ɉ�����
		damageImageAlpha_ -= 0.05f;

		//�摜
		//�e�N�X�`���̃T�C�Y�擾
		XMFLOAT3 size = damageImage_->GetTextureSize();

		//�؂蔲���͈͂����Z�b�g�i�摜�S�̂�\������j
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		//�\���ʒu�p
		Transform t;

		//�`��
		damageImage_->Draw(t, rect, damageImageAlpha_, 1);

		//�������S�ɓ����ɂȂ��Ă���̂Ȃ�`�悵�Ȃ�����
		if (damageImageAlpha_ < 0) (isDrawDamageImage_, false);
	}

	//���C�t�����Z�b�g
	void LifeManager::ResetLife() { playerLife_ = NORMAL_PLAYER_LIFE; }

	//���񂾂ǂ���
	bool LifeManager::IsDie() { return (playerLife_ <= 0); }
}