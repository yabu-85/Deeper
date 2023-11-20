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

void AnimationController::SetNextAnime(int id, int blendFrame, float blendFactor, float red, int startFrame)
{
	currentAnime_ = id;
	currentBlend_ = blendFactor;
	blendRed_ = red;

}

void AnimationController::Update()
{
	currentBlend_ -= blendRed_;
	if (currentBlend_ < 0) currentBlend_ = 0.0f;
	
	Model::SetBlendFactor(modelNum_, currentBlend_);

}
