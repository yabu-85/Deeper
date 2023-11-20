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

	//red��blendFactor���P�t���[�����Ƃɉ�����l�i������
	void SetNextAnime(int id, int blendFrame, float blendFactor, float red, int startFrame = 0 );

	void Update();

};

