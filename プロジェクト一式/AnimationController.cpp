#include "AnimationController.h"
#include "Engine/Model.h"

AnimationController::AnimationController(int number)
	: modelNum_(number), currentAnime_(-1), currentBlend_(0.0f), blendRed_(0.0f)
{
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

void AnimationController::SetNextAnime(int id, float blendFactor, float red)
{
	currentAnime_ = id;
	currentBlend_ = blendFactor;
	blendRed_ = red;

	Model::SetBlendingAnimFrame(modelNum_, animeDatas_.at(id).startFrame, animeDatas_.at(id).endFrame, Model::GetAnimFrame(modelNum_), 1.0f, blendFactor);
}

void AnimationController::SetNextAnime(int id, float red)
{
	//����ID�̏ꍇ��blendFactor���v�Z����
	float weight = 1.0f - Model::GetBlendFactor(modelNum_);
	if (currentAnime_ == id) weight = 1.0f - weight;
	
	SetNextAnime(id, weight, red);
}

void AnimationController::Update()
{
	currentBlend_ -= blendRed_;

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
