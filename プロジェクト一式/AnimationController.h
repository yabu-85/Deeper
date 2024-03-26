#pragma once
#include <vector>

struct AnimeData {
	int startFrame;
	int endFrame;
};

class AnimationController
{
	int modelNum_;			//�S�����Ă郂�f���ԍ�
	int currentAnime_;		//���Đ����Ă���Animation��Id
	float currentBlend_;	//����Blend�̋���
	float blendDecrease_;	//currentBlend��1�t���[���ŉ�����l
	std::vector<AnimeData> animeDatas_;	//�S�Ẵf�[�^��ۑ�

public:
	AnimationController(int number);
	void Update();

	//�A�j���[�V�����f�[�^��ǉ�
	void AddAnime(int s, int e);

	//�A�j���[�V�����f�[�^�̎擾
	AnimeData GetAnim(int id);

	//���̃A�j���[�V������ݒ�iBlendFactor���蓮�Őݒ�
	void SetNextAnime(int id, float blendFactor, float decrease);

	//���̃A�j���[�V������ݒ�iBlendFactor�������Őݒ�
	void SetNextAnime(int id, float decrease);
};