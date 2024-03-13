#pragma once
#include <vector>

struct AnimeData {
	int startFrame;
	int endFrame;
};

class AnimationController
{
	int modelNum_;
	int currentAnime_;
	float currentBlend_;
	float blendRed_;
	std::vector<AnimeData> animeDatas_;

public:
	AnimationController(int number);
	void AddAnime(int s, int e);
	AnimeData GetAnim(int id);

	//red��blendFactor���P�t���[�����Ƃɉ�����l�i������
	void SetNextAnime(int id, float blendFactor, float red);
	void SetNextAnime(int id, float red);

	void Update();
};

