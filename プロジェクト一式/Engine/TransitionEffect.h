#pragma once

enum TRANSITION_TYPE {
	TYPE_ALPHA = 0,
	TYPE_DOWN,
	TYPE_UP,
	TYPE_MAX,
};

namespace TransitionEffect {
	void Initialize();
	void Draw();
	void Release();
	void SceneTransitionInitialize();

	void SetFade(TRANSITION_TYPE type);
	void SetAlpha(float a);
	void SetAlphaDecrease(float a);

}

