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

AnimationController::AnimeData AnimationController::GetAnim(int id)
{
	return animeDatas_[id];
}

void AnimationController::SetNextAnime(int id, float blendFactor, float red)
{
	//同じIDの場合はblendFactorを再計算する
	if (currentAnime_ == id) blendFactor = 1.0f - blendFactor;

	currentAnime_ = id;
	currentBlend_ = blendFactor;
	blendRed_ = red;

	//アニメーションのフレーム数をセット
	Model::SetBlendingAnimFrame(modelNum_, animeDatas_.at(id).startFrame, animeDatas_.at(id).endFrame,
								Model::GetAnimFrame(modelNum_), 1.0f, blendFactor);
	
}

void AnimationController::Update()
{
	currentBlend_ -= blendRed_;

	//0以下になったらブレンド終わりにする
	if (currentBlend_ <= 0.0f) {
		currentBlend_ = 0.0f;
		Model::SetBlendFactor(modelNum_, currentBlend_);
		Model::SetBlend(modelNum_, false);
	}
	else {
		Model::SetBlendFactor(modelNum_, currentBlend_);
	}

}
