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
	int modelNum_;			//�S�����Ă郂�f���ԍ�
	int currentAnim_;		//���Đ����Ă���Animation��Id
	float currentBlend_;	//����Blend�̋���
	float blendDecrease_;	//currentBlend��1�t���[���ŉ�����l
	GameObject* pGameObject_;			//�e
	std::vector<AnimData> animDatas_;	//�S�Ẵf�[�^��ۑ�

public:
	AnimationController(int number, GameObject* obj);
	~AnimationController();
	void Update();
	void AddAnim(int s, int e);
	void AddAnimNotify(int animId, OneFrame* action);

	//���̃A�j���[�V������ݒ�iBlendFactor���蓮�Őݒ�
	void SetNextAnim(int id, float blendFactor, float decrease);
	//���̃A�j���[�V������ݒ�iBlendFactor�������Őݒ�
	void SetNextAnim(int id, float decrease);

	AnimData GetAnim(int id);
	int GetAnimTime(int id);
	int GetCurrentAnim() { return currentAnim_; }
};
