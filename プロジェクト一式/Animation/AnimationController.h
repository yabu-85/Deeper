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

	//アニメーションデータを追加
	void AddAnim(int s, int e);
	
	//通知追加
	void AddAnimNotify(int animId, OneFrame* action);

	//アニメーションデータの取得
	AnimData GetAnim(int id);

	//アニメーションの合計時間を取得
	int GetAnimTime(int id);

	//次のアニメーションを設定（BlendFactorを手動で設定
	void SetNextAnim(int id, float blendFactor, float decrease);

	//次のアニメーションを設定（BlendFactorを自動で設定
	void SetNextAnim(int id, float decrease);
};
