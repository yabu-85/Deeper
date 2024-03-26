#include "AnimationController.h"
#include "Engine/Model.h"

AnimationController::AnimationController(int number)
	: modelNum_(number), currentAnime_(-1), currentBlend_(0.0f), blendDecrease_(0.0f)
{
}

void AnimationController::Update()
{
	currentBlend_ -= blendDecrease_;

	//0�ȉ��ɂȂ�����u�����h�I���ɂ���
	if (currentBlend_ <= 0.0f) {
		currentBlend_ = 0.0f;
		Model::SetBlendFactor(modelNum_, currentBlend_);
		Model::SetBlend(modelNum_, false);
	}
	else {
		Model::SetBlendFactor(modelNum_, currentBlend_);
	}
}

void AnimationController::AddAnime(int s, int e)
{
	AnimeData data;
	data.startFrame = s;
	data.endFrame = e;
	animeDatas_.push_back(data);
}

AnimeData AnimationController::GetAnim(int id)
{
	return animeDatas_[id];
}

void AnimationController::SetNextAnime(int id, float blendFactor, float decrease)
{
	currentAnime_ = id;
	currentBlend_ = blendFactor;
	blendDecrease_ = decrease;

	Model::SetBlendingAnimFrame(modelNum_, animeDatas_.at(id).startFrame, animeDatas_.at(id).endFrame, Model::GetAnimFrame(modelNum_), 1.0f, blendFactor);
}

void AnimationController::SetNextAnime(int id, float decrease)
{
	//����ID�̏ꍇ��blendFactor���v�Z����
	float curent = Model::GetBlendFactor(modelNum_);
	float blend = 1.0f - curent;
	if (currentAnime_ == id) {
		blend = 1.0f;
	}

	SetNextAnime(id, blend, decrease);
}