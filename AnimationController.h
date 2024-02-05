#pragma once
#include <vector>

class AnimationController
{
	struct AnimeData {
		int startFrame;
		int endFrame;
	};
	std::vector<AnimeData> animeDatas_;

	int modelNum_;
	int currentAnime_;
	float currentBlend_;
	float blendRed_;

public:
	AnimationController(int number);
	void AddAnime(int s, int e);

	//redはblendFactorを１フレームごとに下げる値（整数で
	void SetNextAnime(int id, int blendFrame, float blendFactor, float red);

	void Update();

};

