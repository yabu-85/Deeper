#include "TransitionEffect.h"
#include "Direct3D.h"
#include "Sprite.h"
#include "Global.h"
#include <string>

namespace TransitionEffect {
	Sprite* pSprite_[TYPE_MAX];
	RECT rect_;
	Transform transform_;

	TRANSITION_TYPE type_ = TYPE_MAX;
	float alpha_ = 0.0f;
	float alphaDecrease_ = 0.01f;

	void Initialize() {
		const std::string pathFirst = "Image/";
		const std::string pathEnd = ".png";
		const std::string fileName[TYPE_MAX] = { "BlackFade", "WhiteFade", "BlackFade" };
		for (int i = 0; i < TYPE_MAX; i++) {
			pSprite_[i] = new Sprite();
			pSprite_[i]->Load(pathFirst + fileName[i] + pathEnd);
		}
	}

	void Draw() {
		if (type_ == TYPE_MAX) return;
		
		pSprite_[(int)type_]->Draw(transform_, rect_, alpha_, 0);
		alpha_ -= alphaDecrease_;
	}

	void Release()
	{
		for (int i = 0; i < TYPE_MAX; i++) SAFE_DELETE(pSprite_[i]);

	}

	void SceneTransitionInitialize()
	{
		alpha_ = 0.0f;
		type_ = TYPE_MAX;

	}

	void SetFade(TRANSITION_TYPE type)
	{
		XMFLOAT3 size = pSprite_[(int)type]->GetTextureSize();
		rect_.left = 0;
		rect_.top = 0;
		rect_.right = (LONG)size.x;
		rect_.bottom = (LONG)size.y;

		transform_.scale_.x = Direct3D::screenWidth_ / size.x;
		transform_.scale_.y = Direct3D::screenHeight_ / size.y;
		transform_.Calclation();

		type_ = type;
		alpha_ = 1.0f;

	}

	void SetAlpha(float a) { alpha_ = a; }
	void SetAlphaDecrease(float a) { alphaDecrease_ = a; }

}