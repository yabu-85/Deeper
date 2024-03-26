#pragma once
#include <vector>

struct AnimeData {
	int startFrame;
	int endFrame;
};

class AnimationController
{
	int modelNum_;			//担当してるモデル番号
	int currentAnime_;		//今再生しているAnimationのId
	float currentBlend_;	//今のBlendの強さ
	float blendDecrease_;	//currentBlendを1フレームで下げる値
	std::vector<AnimeData> animeDatas_;	//全てのデータを保存

public:
	AnimationController(int number);
	void Update();

	//アニメーションデータを追加
	void AddAnime(int s, int e);

	//アニメーションデータの取得
	AnimeData GetAnim(int id);

	//次のアニメーションを設定（BlendFactorを手動で設定
	void SetNextAnime(int id, float blendFactor, float decrease);

	//次のアニメーションを設定（BlendFactorを自動で設定
	void SetNextAnime(int id, float decrease);
};