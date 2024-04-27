#include "AnimationController.h"
#include "../Engine/Model.h"
#include "AnimationNotify.h"

AnimationController::AnimationController(int number, GameObject* obj)
	: modelNum_(number), currentAnim_(-1), currentBlend_(0.0f), blendDecrease_(0.0f), pGameObject_(obj)
{
}

AnimationController::~AnimationController()
{
	for (int i = 0; i < animDatas_.size(); i++) {
		for (OneFrame* frame : animDatas_[i].frameList) delete frame;
		
	}

}

void AnimationController::Update()
{
	currentBlend_ -= blendDecrease_;

	//0以下になったらブレンド終わりにする
	if (currentBlend_ <= 0.0f) {
		currentBlend_ = 0.0f;
		Model::SetBlendFactor(modelNum_, currentBlend_);
		Model::SetBlend(modelNum_, false);
	}
	else {
		Model::SetBlendFactor(modelNum_, currentBlend_);
	}

	if (currentAnim_ != -1) {
		int animTime = (int)Model::GetAnimFrame(modelNum_);
		for (OneFrame* action : animDatas_[currentAnim_].frameList) {
			action->CalcFrame(animTime, pGameObject_);
		}
	}
}

void AnimationController::AddAnim(int s, int e)
{
	AnimData data;
	data.startFrame = s;
	data.endFrame = e;
	animDatas_.push_back(data);
}

void AnimationController::AddAnimNotify(int AnimId, OneFrame* action)
{
	animDatas_[AnimId].frameList.push_back(action);
}

AnimData AnimationController::GetAnim(int id)
{
	return animDatas_[id];
}

int AnimationController::GetAnimTime(int id)
{
	return  animDatas_[id].endFrame - animDatas_[id].startFrame;
}

void AnimationController::SetNextAnim(int id, float blendFactor, float decrease)
{
	currentAnim_ = id;
	currentBlend_ = blendFactor;
	blendDecrease_ = decrease;

	Model::SetBlendingAnimFrame(modelNum_, animDatas_.at(id).startFrame, animDatas_.at(id).endFrame, (int)Model::GetAnimFrame(modelNum_), 1.0f, blendFactor);
}

void AnimationController::SetNextAnim(int id, float decrease)
{
	//同じIDの場合はblendFactorを計算する
	float curent = Model::GetBlendFactor(modelNum_);
	float blend = 1.0f - curent;
	if (currentAnim_ == id) {
		blend = 1.0f;
	}

	SetNextAnim(id, blend, decrease);
}