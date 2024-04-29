#pragma once
#include <vector>
class OneFrame;
class GameObject;

struct AnimData {
	int startFrame;
	int endFrame;
	std::vector<OneFrame*> frameList;
};

class AnimationController
{
	int modelNum_;			//担当してるモデル番号
	int currentAnim_;		//今再生しているAnimationのId
	float currentBlend_;	//今のBlendの強さ
	float blendDecrease_;	//currentBlendを1フレームで下げる値
	GameObject* pGameObject_;			//親
	std::vector<AnimData> animDatas_;	//全てのデータを保存

public:
	AnimationController(int number, GameObject* obj);
	~AnimationController();
	void Update();
	void AddAnim(int s, int e);
	void AddAnimNotify(int animId, OneFrame* action);

	//次のアニメーションを設定（BlendFactorを手動で設定
	void SetNextAnim(int id, float blendFactor, float decrease);
	//次のアニメーションを設定（BlendFactorを自動で設定
	void SetNextAnim(int id, float decrease);

	AnimData GetAnim(int id);
	int GetAnimTime(int id);
	int GetCurrentAnim() { return currentAnim_; }
};
